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

  File Name: os_file.h

  Description: OS dependent file operations

------------------------------------------------------------------------------*/
#ifndef CAF_INCLUDE_OS_FILE_H_
#define CAF_INCLUDE_OS_FILE_H_

#include <stdint.h>
#include "autoconf.h"
#include "AudioSmart_sendcmd_common.h"  //for Command

#define GLOBAL
#define STATIC              static

#define IN_                 const
#define OUT
#define IO_

#define __Y

//typedef unsigned  size_t;

#define CODE_SEGMENT(pm_seg)
#define DATA_SEGMENT(dm_seg)

// CEILING arguments: x- value for rounding up, significance- The multiple to round to
#define CEILING(x, significance) ((((x)-1)/(significance) + 1) * (significance)) 

#define INT_SIZE_OF(objtype,int_type) (((sizeof(objtype)-1)/sizeof(int_type))+1)

#define INT16_SIZE_OF(obj_type)         INT_SIZE_OF(obj_type, int16_t)
#define INT32_SIZE_OF(obj_type)         INT_SIZE_OF(obj_type, int32_t)
#define INT64_SIZE_OF(obj_type)         INT_SIZE_OF(obj_type, int64_t)
#define INT128_SIZE_OF(objtype)       (((sizeof(objtype)-1)/(sizeof(int64_t)*2))+1)

#define ARRAY_LENGTH(obj_type)          (sizeof(obj_type)/sizeof(obj_type[0]))

#if (defined SIMULATION)
#define MAX_FILE_LEN 256 //or any other lengh
#else
#define MAX_FILE_LEN 64 //or any other lengh
#endif

#define WRITABLE_FILE    0x01
#define WRITABLE_SECTION 0x02

#define FOR_READ_IN_RO   ((0*WRITABLE_SECTION) | (0*WRITABLE_FILE))
#define FOR_READ_IN_RW   ((1*WRITABLE_SECTION) | (0*WRITABLE_FILE))
#define FOR_WRITE        ((1*WRITABLE_SECTION) | (1*WRITABLE_FILE))

extern char file_ro_base_path[MAX_FILE_LEN];
extern char file_rw_base_path[MAX_FILE_LEN];

typedef struct sfs_file_info_s
{
    uint32_t sta;       /// starting address of the file in FLASH.
    uint32_t fsz;       /// file size in bytes.
    uint32_t pos;       /// current relative position within the file, in byte.
    uint32_t part_num;  /// partition number
}sfs_file_info_t;


#if defined(CONFIG_MCU_DRV_SFS) && (defined(CONFIG_TARGET_NIUE) || defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA))
#define OS_FILE_MAX_CMD_SIZE 5
typedef COMMAND_OF_SIZE(OS_FILE_MAX_CMD_SIZE) CommandOsFile;
#else
#define OS_FILE_MAX_CMD_SIZE 0
#endif

typedef struct FileHandleTag {
  void *ptr;
#if (OS_FILE_MAX_CMD_SIZE > 0)
  sfs_file_info_t info;
  CommandOsFile   command;
#endif
} FileHandle;

inline void     IdHex2Str    (char *dest, uint32_t id){
  //#define _ID(a,b,c,d)  ((uint32_t)((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26)))
  dest[0] = ((id>>  8) & 0x3F) + 0x20;
  dest[1] = ((id>> 14) & 0x3F) + 0x20;
  dest[2] = ((id>> 20) & 0x3F) + 0x20;
  dest[3] = ((id>> 26) & 0x3F) + 0x20;
}



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
GLOBAL int32_t  OsFileOpen   (IO_ FileHandle *fp, IN_ uint32_t fileid,  IN_ uint32_t mode  );

/*------------------------------------------------------------------------------
  Function:   OsFileClose()
 
  Purpose:    Closes a file
 
  Arguments:  fp         - Pointer to FileHandle

  Returned:   An error code, where 0 indicates success.
------------------------------------------------------------------------------*/
GLOBAL int32_t  OsFileClose  (IO_ FileHandle *fp);

/*------------------------------------------------------------------------------
  Function:   OsFileRead()
 
  Purpose:    Read from a file
 
  Arguments:  fp         - Pointer to FileHandle
              data       - pointr to int32_t buffer to store the data read from the file
              n_32bit    - number of 32bit elements to be read
  Returned:   total number of elements read
------------------------------------------------------------------------------*/
GLOBAL int32_t  OsFileRead   (IO_ FileHandle *fp, OUT int32_t *data  ,  IN_ uint32_t n_32bit);

/*------------------------------------------------------------------------------
  Function:   OsFileWrite()
 
  Purpose:    Write to a file
 
  Arguments:  fp         - Pointer to FileHandle
              data       - pointr to int32_t buffer that stores the data to be written
              n_32bit    - number of 32bit elements to be written
  Returned:   0 on success, else number of elements not written to file
------------------------------------------------------------------------------*/
GLOBAL int32_t  OsFileWrite  (IO_ FileHandle   *fp, IN_ int32_t *data  ,  IN_ uint32_t n_32bit);

#endif //CAF_INCLUDE_OS_FILE_H_

