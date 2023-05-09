/*------------------------------------------------------------------------------
  Copyright (C) 2010-2019 Conexant Systems Inc.
  All rights reserved.

  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Conexant Systems, Inc.
  ("Conexant")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Conexant Systems Inc.

  Except as otherwise specifically provided through an express agreement
  with Conexant that governs the confidentiality, possession, use
  and distribution of the information contained in this file, CONEXANT
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: os_file.c

  Description: General file manipulation. Implementation is platform dependent.

------------------------------------------------------------------------------*/



//#include "base_module_ex.h"
//#include "AudioSmart_definitions.h"
#include "autoconf.h"
#include "os_file.h"
#include <stdio.h>



#define SFS_ON_CAPE 1
#define SFS_ON_MCU  2

#if (defined CONFIG_CAF_FLAVOR_SW) || (defined SIMULATION)
DATA_SEGMENT(XS)
char  file_ro_base_path[MAX_FILE_LEN] = "";
char  file_rw_base_path[MAX_FILE_LEN] = "";
#endif

#if defined(CONFIG_TARGET_NIUE) && defined(CONFIG_MCU_DRV_SFS)
#define USE_SFS   SFS_ON_MCU
#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA)) && defined(CONFIG_MCU_DRV_SFS)
#define USE_SFS   SFS_ON_MCU
#else
#define USE_SFS   0
#endif

#if (USE_SFS == SFS_ON_MCU)
inline int McuFileDelete    (FileHandle *fp, uint32_t id);
inline int McuFileCreate    (FileHandle *fp, uint32_t id);
inline int McuFileOpen      (FileHandle *fp, uint32_t id);
inline int McuFileClose     (FileHandle *fp);
inline int McuFileRead      (FileHandle *fp,       void * data, uint32_t len32b);
inline int McuFileWrite     (FileHandle *fp, const void * data, uint32_t len32b);
inline int McuFileReadY     (FileHandle *fp,       void __Y * data, uint32_t len32b);
inline int McuFileWriteY    (FileHandle *fp, const void __Y * data, uint32_t len32b);

#define SFS_DELETE_FILE(h,file_id)    McuFileDelete(h, file_id)
#define SFS_CREATE_FILE(h,file_id)    McuFileCreate(h, file_id)
#define SFS_OPEN_FILE(h,file_id)      McuFileOpen  (h, file_id)
#define SFS_CLOSE_FILE(h)             McuFileClose (h)
#define SFS_READ_FILE(h,data,len)     McuFileRead  (h, data, len)
#define SFS_WRITE_FILE(h,data,len)    McuFileWrite (h, data, len)
#define SFS_READ_FILE_Y(h,data,len)   McuFileReadY (h, data, len)
#define SFS_WRITE_FILE_Y(h,data,len)  McuFileWriteY(h, data, len)

#else // SFS_ON_CAPE
//TODO: Like Sicily
#endif

/*------------------------------------------------------------------------------
  Function:   OsFileOpen()
 
  Purpose:    Opens a file
 
  Arguments:  fp         - Pointer to FileHandle, where the function fills
                           the file handle (success) or NULL (error).
              fileid     - File ID of 4 letters compacted with macro _ID().
                           Corresponds to file name
              mode       - READ_MODE for read, WRITE_MODE for write

  Returned:   An error code, where 0 indicates success.
------------------------------------------------------------------------------*/
CODE_SEGMENT(CONFIG_CAF_CONTROL_CODE_SEG)
GLOBAL int32_t  OsFileOpen   (IO_ FileHandle *fp, IN_ uint32_t fileid,  IN_ uint32_t mode  ) {
  int32_t  err;

  if (NULL == fp) {
    err = 0;//MODULE_RC_FILE_OPEN_ERROR;
  } else {
#if (USE_SFS == 0)

    FILE    *pfile;
    char    *file_name;
    char     file_id[5];
#if (defined CONFIG_CAF_FLAVOR_SW) || (defined SIMULATION)
    char     file_path[MAX_FILE_LEN + 4 + 4];
    char    *file_base_path;

    if ((mode&WRITABLE_SECTION) == 0)
    {
      file_base_path = file_ro_base_path;
    }
    else
    {
      file_base_path = file_rw_base_path;
    }
#endif

    file_id[4] = '\0' ;
    IdHex2Str(file_id, fileid);

#if (defined CONFIG_CAF_FLAVOR_SW) || (defined SIMULATION)
    STRCPY (file_path, MAX_FILE_LEN, file_base_path);
    STRNCAT(file_path, sizeof(file_path), file_id, sizeof(file_id));
#if (defined CONFIG_OS_LINUX) || (defined CONFIG_OS_ANDROID)
    STRNCAT(file_path, sizeof(file_path), ".caf", sizeof(file_id));
#endif
    file_name = &(file_path[0]);
#else
    file_name = &(file_id[0]);
#endif

    if (0 == (mode&WRITABLE_FILE)) {//read
      FILE_OPEN(pfile, file_name, "rb");
    } else {//write
      FILE_OPEN(pfile, file_name, "wb");
    }

    fp->ptr = (void *)pfile;

    err = (NULL == pfile) ? MODULE_RC_FILE_OPEN_ERROR : 0;

#elif (USE_SFS == SFS_ON_CAPE || USE_SFS == SFS_ON_MCU)
    if (FOR_WRITE == mode) {
            McuFileDelete(fp, fileid);
      err = McuFileCreate(fp, fileid);
    } else {
      err = McuFileOpen(fp, fileid);
    }
#endif
  }

  return err;
}

/*------------------------------------------------------------------------------
  Function:   OsFileClose()
 
  Purpose:    Closes a file
 
  Arguments:  fp         - Pointer to FileHandle

  Returned:   An error code, where 0 indicates success.
------------------------------------------------------------------------------*/
CODE_SEGMENT(CONFIG_CAF_CONTROL_CODE_SEG)
GLOBAL int32_t  OsFileClose  (IO_ FileHandle *fp){
  int32_t  err;

  if        (NULL == fp) {
    err = 0;//MODULE_RC_FILE_OPEN_ERROR;
#if (USE_SFS == 0)
  } else if (NULL == fp->ptr) {
    err = MODULE_RC_FILE_OPEN_ERROR;
#endif
  } else {
#if (USE_SFS == 0)
    FILE * pfile = (FILE *)fp->ptr;
    err = fclose(pfile);
    fp->ptr = NULL;
#elif (USE_SFS == SFS_ON_CAPE || USE_SFS == SFS_ON_MCU)
    SFS_CLOSE_FILE(fp);
    err = 0;
#else
    err = -ENOSYS;
#endif
  }

  return err;
}

/*------------------------------------------------------------------------------
  Function:   OsFileRead()
 
  Purpose:    Read from a file
 
  Arguments:  fp         - Pointer to FileHandle
              data       - pointr to int32_t buffer to store the data read from the file
              n_32bit    - number of 32bit elements to be read
  Returned:   total number of elements read
------------------------------------------------------------------------------*/
CODE_SEGMENT(CONFIG_CAF_CONTROL_CODE_SEG)
GLOBAL int32_t  OsFileRead  (IO_ FileHandle *fp, OUT int32_t *data,  IN_ uint32_t n_32bit){
  size_t   n_actual;

#if (USE_SFS == 0)
  FILE    *pfile = (FILE *)(fp->ptr);
  n_actual = fread (data, sizeof(int32_t), n_32bit, pfile);
#elif (USE_SFS == SFS_ON_CAPE || USE_SFS == SFS_ON_MCU)
#if defined(CONFIG_TARGET_NIUE)
  int32_t iCount,i;
  n_actual = 0;
  if (n_32bit > 1024) {
    iCount = n_32bit / 1024;
    for (i = 0; i < iCount; i++) {
      n_actual += McuFileRead(fp, data + i*1024, 1024); // the return value is 4bytes
    }

    iCount = n_32bit % 1024;
    n_actual += McuFileRead(fp, data + i*1024, iCount); // the return value is 4bytes
  } else {
    n_actual = McuFileRead(fp, data, n_32bit);
  }
#else
  int ret_val = SFS_READ_FILE(fp, data, n_32bit);
  n_actual = (ret_val > 0) ? (ret_val) : 0;
#endif
#else
  n_actual = 0;
#endif

  return (int32_t)n_actual;
}


/*------------------------------------------------------------------------------
  Function:   OsFileWrite()
 
  Purpose:    Write to a file
 
  Arguments:  fp         - Pointer to FileHandle
              data       - pointr to int32_t buffer that stores the data to be written
              n_32bit    - number of 32bit elements to be written
  Returned:   number of elements  written to file
------------------------------------------------------------------------------*/
CODE_SEGMENT(CONFIG_CAF_CONTROL_CODE_SEG)
GLOBAL int32_t  OsFileWrite  (IO_ FileHandle *fp, IN_ int32_t *data,  IN_ uint32_t n_32bit){
  size_t   n_actual;

#if (USE_SFS == 0)
  FILE    *pfile = (FILE *)(fp->ptr);
  n_actual = fwrite (data, sizeof(int32_t), n_32bit, pfile);
#elif (USE_SFS == SFS_ON_CAPE || USE_SFS == SFS_ON_MCU)
  int ret_val = SFS_WRITE_FILE(fp, data, n_32bit);
  n_actual = (ret_val > 0) ? (ret_val) : 0;
#else
  n_actual = 0;
#endif
  return (int32_t)n_actual;
}



#if (USE_SFS == SFS_ON_MCU)

#define WAIT_MCU_SFS_TIMEOUT      5000    //create operation will erase block.

typedef struct
{
  uint32_t        file_id;
  uint32_t        fsz_in_blk;
  sfs_file_info_t *pfile_info;
} McuMgrFileCreate;

typedef struct
{
  uint32_t        file_id;
  sfs_file_info_t *pfile_info;
} McuMgrFileOpen;

typedef struct
{
  sfs_file_info_t *pfile_info;
}McuMgrFileClose;

typedef struct
{
  sfs_file_info_t *pfile_info;
  uint32_t        *buffer;
  uint32_t        len_in_4bytes;
} McuMgrFileAccess;

typedef struct
{
  uint32_t file_id;
} McuMgrFileDelete;

typedef struct
{
  sfs_file_info_t *pfile_info;
  uint32_t        offset;
  uint32_t        fromwhere;
} McuMgrFileSeek;



#if defined(CONFIG_TARGET_NIUE)
#define MCU_MGR_FILE_CREATE       169
#define MCU_MGR_FILE_OPEN         170
#define MCU_MGR_FILE_CLOSE        171
#define MCU_MGR_FILE_DELETE       172
#define MCU_MGR_FILE_ACCESS       173
#define MCU_MGR_FILE_SEEK         174
#define MCU_MGR_FILE_READ         CMD_GET(MCU_MGR_FILE_ACCESS)
#define MCU_MGR_FILE_WRITE        CMD_SET(MCU_MGR_FILE_ACCESS)

#define ACPU_MEMMAP_BLKRAM_0_BASE      0x3FF00000
#define ACPU_MEMMAP_BLKRAM_0_SIZE      0x20000
#define ACPU_MEMMAP_BLKRAM_7_BASE      0x3FFE0000
#define ACPU_MEMMAP_BLKRAM_7_SIZE      0x20000
#define ACPU_MEMMAP_HIFI_TCM_BASE      0x3FC00000
#define ACPU_MEMMAP_HIFI_TCM_SIZE      0x200000

/* Check addr within AXI SRAM range */
#define ADDR_WITHIN_AXI_SRAM_RANGE(addr) \
    ((addr) >= (ACPU_MEMMAP_BLKRAM_0_BASE) && (addr) <= (ACPU_MEMMAP_BLKRAM_7_BASE + ACPU_MEMMAP_BLKRAM_7_SIZE - 1))

/* Check addr within AXI TCM range */
#define ADDR_WITHIN_AXI_TCM_RANGE(addr) \
    ((addr) >= (ACPU_MEMMAP_HIFI_TCM_BASE) && (addr) <= (ACPU_MEMMAP_HIFI_TCM_BASE + ACPU_MEMMAP_HIFI_TCM_SIZE - 1))

/* Covert SRAM address from the M33 to the DSP and vice versa */
#define AHB_AXI_ADDR_SRAM(addr)    (((addr) & 0x000fffff) | 0x3ff00000)
#define AXI_AHB_ADDR_SRAM(addr)    (((addr) & 0x000fffff) | 0x04000000)

/* Covert TCM address from the M33 to the DSP */
#define AHB_AXI_ADDR_TCM(addr)     (((addr) & 0x000fffff) | 0x3FC00000)
#define AXI_AHB_ADDR_TCM(addr)     (((addr) & 0x000fffff) | 0x28000000)

uint32_t uiSharableSRAMAddr      = 0x3FF60048;
uint32_t uiSharableSRAMDataAddr  = 0x3FF60048 + 64;

#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))
#ifndef PARTITION_MAIN
#define PARTITION_MAIN 0
#endif



#endif


inline int McuFileDelete (FileHandle *fp, uint32_t id)
{
  int       ret_val;
#if defined(CONFIG_TARGET_NIUE)
  FileHandle * sfp = (FileHandle *)uiSharableSRAMAddr;
  Command * cmd2mcu = (Command *)((uint32_t*)uiSharableSRAMAddr + 4); // fp->command
  McuMgrFileDelete *fileDelete = (McuMgrFileDelete *)((uint32_t*)uiSharableSRAMAddr + 6);  // command->data[0]

  // copy file info to be transferred
  memcpy16(&(sfp->info), &(fp->info), INT16_SIZE_OF(sfs_file_info_t));

  cmd2mcu->head.num_32b_words = sizeof(McuMgrFileDelete) / 4;
  cmd2mcu->head.command_id    = MCU_MGR_FILE_DELETE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ');
  
  fileDelete->file_id    = id;

  ret_val = send_cmd_to_mcu(WAIT_MCU_SFS_TIMEOUT, cmd2mcu);
#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))

  Command         *cmd2mcu    = (Command *)(&(fp->command));
  sfs_file_info_t *pfile_info = (sfs_file_info_t *)(&(fp->info));

  cmd2mcu->head.num_32b_words = INT32_SIZE_OF(McuMgrFileDelete);;
  cmd2mcu->head.command_id    = SW_SFS_CMD_FILE_DELETE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ') | FWID_SW;

  cmd2mcu->data[0] = id;

  ret_val = SENDCMD_SYNC(cmd2mcu);
#endif

  return ret_val;
}

inline int McuFileCreate (FileHandle *fp, uint32_t id)
{
  int       ret_val;
#if defined(CONFIG_TARGET_NIUE)
  FileHandle * sfp = (FileHandle *)uiSharableSRAMAddr;
  Command * cmd2mcu = (Command *)((uint32_t*)uiSharableSRAMAddr + 4); // fp->command
  McuMgrFileCreate *fileCreate = (McuMgrFileCreate *)((uint32_t*)uiSharableSRAMAddr + 6);  // command->data[0]
  
  memset16(sfp, 0, INT16_SIZE_OF(FileHandle));

  cmd2mcu->head.num_32b_words = sizeof(McuMgrFileCreate) / 4;
  cmd2mcu->head.command_id    = MCU_MGR_FILE_CREATE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ');

  fileCreate->file_id    = id;
  fileCreate->fsz_in_blk = 1;  
  
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)&(sfp->info)))
  {
    fileCreate->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_TCM((uint32_t)&(sfp->info));
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)&(sfp->info)))
  {
    fileCreate->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_SRAM((uint32_t)&(sfp->info));
  }

  ret_val = send_cmd_to_mcu(WAIT_MCU_SFS_TIMEOUT, cmd2mcu);  
  memcpy16(&(fp->info), &(sfp->info), INT16_SIZE_OF(sfs_file_info_t));
#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))

  Command         *cmd2mcu    = (Command *)(&(fp->command));
  sfs_file_info_t *pfile_info = (sfs_file_info_t *)(&(fp->info));

  cmd2mcu->head.num_32b_words = INT32_SIZE_OF(McuMgrFileCreate);;
  cmd2mcu->head.command_id    = SW_SFS_CMD_FILE_CREATE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ') | FWID_SW;

  cmd2mcu->data[0] = id;
  cmd2mcu->data[1] = 1; //fsz_in_blk


  cmd2mcu->data[2] = (uint32_t)(pfile_info);


  ret_val = SENDCMD_SYNC(cmd2mcu);
#endif
  return ret_val;
}





inline int McuFileOpen   (FileHandle *fp, uint32_t id)
{
  int       ret_val = 0;
#if defined(CONFIG_TARGET_NIUE)
  FileHandle * sfp = (FileHandle *)uiSharableSRAMAddr;
  Command * cmd2mcu = (Command *)((uint32_t*)uiSharableSRAMAddr + 4); // fp->command
  McuMgrFileOpen *fileOpen = (McuMgrFileOpen *)((uint32_t*)uiSharableSRAMAddr + 6);  // command->data[0]

  memset16(sfp, 0, INT16_SIZE_OF(FileHandle));

  cmd2mcu->head.num_32b_words = sizeof(McuMgrFileOpen) / 4;
  cmd2mcu->head.command_id    = MCU_MGR_FILE_OPEN;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ');
  
  fileOpen->file_id      = id;
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)&(sfp->info)))
  {
    fileOpen->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_TCM((uint32_t)&(sfp->info));
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)&(sfp->info)))
  {
    fileOpen->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_SRAM((uint32_t)&(sfp->info));
  }

  ret_val = send_cmd_to_mcu(WAIT_MCU_SFS_TIMEOUT, cmd2mcu);
  memcpy16(&(fp->info), &(sfp->info), INT16_SIZE_OF(sfs_file_info_t));

#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))
  Command         *cmd2mcu    = (Command *)(&(fp->command));
  sfs_file_info_t *pfile_info = (sfs_file_info_t *)(&(fp->info));

  cmd2mcu->head.num_32b_words = INT32_SIZE_OF(McuMgrFileOpen);;
  cmd2mcu->head.command_id    = SW_SFS_CMD_FILE_OPEN;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ') | FWID_SW;

  cmd2mcu->data[0] = id;


  cmd2mcu->data[1] = (uint32_t)(pfile_info);

  ret_val = SENDCMD_SYNC(cmd2mcu);

  
#endif
  return ret_val;
}

inline int McuFileClose  (FileHandle *fp)
{
  int       ret_val;
#if defined(CONFIG_TARGET_NIUE)
  FileHandle * sfp = (FileHandle *)uiSharableSRAMAddr;
  Command * cmd2mcu = (Command *)((uint32_t*)uiSharableSRAMAddr + 4); // fp->command
  McuMgrFileClose *fileClose = (McuMgrFileClose *)((uint32_t*)uiSharableSRAMAddr + 6);  // command->data[0]

  // copy file info to be transferred
  memcpy16(&(sfp->info), &(fp->info), INT16_SIZE_OF(sfs_file_info_t));

  cmd2mcu->head.num_32b_words = sizeof(McuMgrFileClose) / 4;
  cmd2mcu->head.command_id    = MCU_MGR_FILE_CLOSE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ');
  
  //fileClose->file_id      = id;
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)&(sfp->info)))
  {
    fileClose->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_TCM((uint32_t)&(sfp->info));
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)&(sfp->info)))
  {
    fileClose->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_SRAM((uint32_t)&(sfp->info));
  }

  ret_val = send_cmd_to_mcu(WAIT_MCU_SFS_TIMEOUT, cmd2mcu);
  memcpy16(&(fp->info), &(sfp->info), INT16_SIZE_OF(sfs_file_info_t));
#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))
  Command         *cmd2mcu    = (Command *)(&(fp->command));
  sfs_file_info_t *pfile_info = (sfs_file_info_t *)(&(fp->info));

  cmd2mcu->head.num_32b_words = INT32_SIZE_OF(McuMgrFileClose);;
  cmd2mcu->head.command_id    = SW_SFS_CMD_FILE_CLOSE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ') | FWID_SW;

  cmd2mcu->data[0] = (uint32_t)(pfile_info);


  ret_val = SENDCMD_SYNC(cmd2mcu);
#else
  ret_val = MODULE_RC_CMD_CODE_UNSUPPORTED;
#endif
  return ret_val;
}

inline int McuFileRead  (FileHandle *fp,       void * data, uint32_t len32b)
{
  int       ret_val;
#if defined(CONFIG_TARGET_NIUE)
  FileHandle * sfp = (FileHandle *)uiSharableSRAMAddr;
  Command * cmd2mcu = (Command *)((uint32_t*)uiSharableSRAMAddr + 4); // fp->command
  McuMgrFileAccess *fileAccess = (McuMgrFileAccess *)((uint32_t*)uiSharableSRAMAddr + 6);  // command->data[0]
  void *viDataBuf = (void*)uiSharableSRAMDataAddr;

  // copy file info to be transferred
  memcpy16(&(sfp->info), &(fp->info), INT16_SIZE_OF(sfs_file_info_t));

  cmd2mcu->head.num_32b_words = sizeof(McuMgrFileAccess) / 4;
  cmd2mcu->head.command_id    = MCU_MGR_FILE_READ;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ');
  
  //fileAccess->file_id    = id;
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)viDataBuf))
  {
    fileAccess->buffer = (uint32_t*)AXI_AHB_ADDR_TCM((uint32_t)viDataBuf);
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)viDataBuf))
  {
    fileAccess->buffer = (uint32_t*)AXI_AHB_ADDR_SRAM((uint32_t)viDataBuf);
  }

  fileAccess->len_in_4bytes = len32b;
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)&(sfp->info)))
  {
    fileAccess->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_TCM((uint32_t)&(sfp->info));
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)&(sfp->info)))
  {
    fileAccess->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_SRAM((uint32_t)&(sfp->info));
  }

  ret_val = send_cmd_to_mcu(WAIT_MCU_SFS_TIMEOUT, cmd2mcu);
  if (ret_val > 0)
  {
    memcpy16(&(fp->info), &(sfp->info), INT16_SIZE_OF(sfs_file_info_t));
    memcpy32(data, viDataBuf, ret_val);
  }
#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))
  Command         *cmd2mcu    = (Command *)(&(fp->command));
  sfs_file_info_t *pfile_info = (sfs_file_info_t *)(&(fp->info));

  cmd2mcu->head.num_32b_words = INT32_SIZE_OF(McuMgrFileAccess);;
  cmd2mcu->head.command_id    = SW_SFS_CMD_FILE_READ;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ') | FWID_SW;


  cmd2mcu->data[0] = (uint32_t)(pfile_info);
  cmd2mcu->data[1] = (uint32_t)(data);  //iadma of qspi is a non-security master.
  cmd2mcu->data[2] = len32b;

  ret_val = SENDCMD_SYNC(cmd2mcu);
#else
  ret_val = MODULE_RC_CMD_CODE_UNSUPPORTED;
#endif
  return ret_val;
}



inline int McuFileWrite (FileHandle *fp, const void * data, uint32_t len32b)
{
  int       ret_val;
#if defined(CONFIG_TARGET_NIUE)
  FileHandle * sfp = (FileHandle *)uiSharableSRAMAddr;
  Command * cmd2mcu = (Command *)((uint32_t*)uiSharableSRAMAddr + 4); // fp->command
  McuMgrFileAccess *fileAccess = (McuMgrFileAccess *)((uint32_t*)uiSharableSRAMAddr + 6);  // command->data[0]
  void *viDataBuf = (void*)uiSharableSRAMDataAddr;

  // copy file info to be transferred
  memcpy16(&(sfp->info), &(fp->info), INT16_SIZE_OF(sfs_file_info_t));

  cmd2mcu->head.num_32b_words = sizeof(McuMgrFileAccess) / 4;
  cmd2mcu->head.command_id    = MCU_MGR_FILE_WRITE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ');

  memcpy32(viDataBuf, data, len32b);
  
  //fileAccess->file_id    = id;
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)viDataBuf))
  {
    fileAccess->buffer = (uint32_t*)AXI_AHB_ADDR_TCM((uint32_t)viDataBuf);
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)viDataBuf))
  {
    fileAccess->buffer = (uint32_t*)AXI_AHB_ADDR_SRAM((uint32_t)viDataBuf);
  }
  fileAccess->len_in_4bytes = len32b;
  if (ADDR_WITHIN_AXI_TCM_RANGE((uint32_t)&(sfp->info)))
  {
    fileAccess->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_TCM((uint32_t)&(sfp->info));
  }
  else if (ADDR_WITHIN_AXI_SRAM_RANGE((uint32_t)&(sfp->info)))
  {
    fileAccess->pfile_info = (sfs_file_info_t*)AXI_AHB_ADDR_SRAM((uint32_t)&(sfp->info));
  }

  ret_val = send_cmd_to_mcu(WAIT_MCU_SFS_TIMEOUT, cmd2mcu);
  if (ret_val > 0)
  {
    memcpy16(&(fp->info), &(sfp->info), INT16_SIZE_OF(sfs_file_info_t));
  }
#elif (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))
  Command         *cmd2mcu    = (Command *)(&(fp->command));
  sfs_file_info_t *pfile_info = (sfs_file_info_t *)(&(fp->info));

  cmd2mcu->head.num_32b_words = INT32_SIZE_OF(McuMgrFileAccess);
  cmd2mcu->head.command_id    = SW_SFS_CMD_FILE_WRITE;
  cmd2mcu->head.reply         = 0;
  cmd2mcu->head.mgr_module_id = _ID('M','C','U',' ') | FWID_SW;

  cmd2mcu->data[0] = (uint32_t)(pfile_info);
  cmd2mcu->data[1] = (uint32_t)(data);  //iadma of qspi is a non-security master.
  cmd2mcu->data[2] = len32b;

  ret_val = SENDCMD_SYNC(cmd2mcu);
#else
  ret_val = MODULE_RC_CMD_CODE_UNSUPPORTED;
#endif
  return ret_val;
}



#endif

