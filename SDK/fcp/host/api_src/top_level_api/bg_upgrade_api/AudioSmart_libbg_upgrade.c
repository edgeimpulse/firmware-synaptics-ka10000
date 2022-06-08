/*------------------------------------------------------------------------------
  Copyright (C) 2018-2020 Synaptics Inc.
  All rights reserved.

  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Synaptics Inc.
  ("Synaptics")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Synaptics Inc.

  Except as otherwise specifically provided through an express agreement
  with Synaptics that governs the confidentiality, possession, use
  and distribution of the information contained in this file, SYNAPTICS
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: AudioSmart_libbg_upgrade.cpp

------------------------------------------------------------------------------*/
#include "AudioSmart_definitions.h"
#include "AudioSmart_sendcmd.h"
#include "AudioSmart_sendcmd_common.h" 
#include "AudioSmart_libbg_upgrade.h"

#ifdef _DEBUG
#include <stdio.h> // for printf
#define DUMP(f_, ...) printf((f_), __VA_ARGS__)
#else
#define DUMP(f_, ...)
#endif

typedef uint16_t blknr;
typedef uint32_t dword;

typedef enum _CXHID_OPERATION
{
  CXHID_OPERATION_READ,
  CXHID_OPERATION_WRITE
} CXHID_OPERATION;

#define CXHID_STATUS_OK  0 
#define CXHID_STATUS_SUCCESS(status)  (CXHID_STATUS_OK == (status))

typedef struct _HID_DEV_VERSION
{
  uint32_t  Major;
  uint32_t  Minor;
  uint32_t  Variant;
  uint32_t  Build;      /*  Irrelevant in case of patch version */
}
HID_DEV_VERSION, *PHID_DEV_VERSION;

/*------------------------------------------------------------------------------
  Type:       ModuleReturnCode

  Purpose:    Return codes of module's functions
------------------------------------------------------------------------------*/
#define MODULE_RC_FIRST -1024

#if defined(_WINDOWS) 
#define __PACKED__  
//#include <string.h> 
#else
#define __PACKED__ __attribute__((packed))
//#include <string.h> 
#endif

#if defined(_WINDOWS) 
#pragma pack(push,1)
#endif

#define I2C_PAYLOAD_SIZE    4096
#define I2C_PAYLOAD_SIZE_B  (I2C_PAYLOAD_SIZE/sizeof(dword))
#define SFS_LOG2BLKSIZ      12
//#define EFS_BLOCKSIZE       4096      // size of a SFS block (min allocation unit)
#define SFS_BLK2POS(n)      ((n)<<SFS_LOG2BLKSIZ)

#define EFS_BOOTLOADER_HEADERSIZE    sizeof(struct efs_partition_bootloader_s)

#if defined(_WINDOWS) 
#pragma pack(pop)
#endif

#define TIME_OUT_MS           100   //50 ms
#define MANUAL_TIME_OUT_MS    5000  //1000 ms
#define POLLING_INTERVAL_MS   1     // 1 ms
#define RESET_INTERVAL_MS     200   //200 ms
#define IMG_BLOCK_SIZE        2048
#define IMG_BLOCK_SIZE_RENO   4096
#define MAX_RETRIES           3
#define MAX_I2C_SUB_ADDR_SIZE 4
#define ALIGNMENT_SIZE        (sizeof(long))
#define MAX_WRITE_BUFFER      (sizeof(struct i2c_verify_img_cmd_s))
#define MAX_READ_BUFFER       (sizeof(struct i2c_ping_rpl_s) )
#define ALIG_SIZE(_x_)        ((sizeof(_x_) + ALIGNMENT_SIZE-1)/ALIGNMENT_SIZE)
#define MAX_SMALL_WRITE       0x10
#define _SFS_VERSION(a,b,c,d) ((uint32_t)((a)<<24)|((b)<<16)|((c)<<8)|((d)<<0))
#define MAX_I2C_WRITE_LEN     (120)
#define MAX_I2C_READ_LEN      16

#define CONFIG_MAGIC_NUM_OFFSET_100 
#ifdef CONFIG_MAGIC_NUM_OFFSET_100
#define MAGIC_NUM_OFFSET     (0x100)
#else 
#define MAGIC_NUM_OFFSET     (0x400)
#endif 

#define HEADER_NOT_FF_SIZE   (MAGIC_NUM_OFFSET + 0x4) // include CRC magic



#define SW_SPIMEM_TUNNEL_GET_INFO          CMD_GET(SW_SPIMEM_TUNNEL_CMD)
#define SW_SPIMEM_TUNNEL_SET_INFO          CMD_SET(SW_SPIMEM_TUNNEL_CMD)
#define SW_SPIMEM_TUNNEL_CMD_SET_CONFIG    CMD_SET(SW_SPIMEM_TUNNEL_CMD_CONFIG)
#define SW_SPIMEM_TUNNEL_CMD_READ_BUFFER   CMD_GET(SW_SPIMEM_TUNNEL_CMD_DATA)
#define SW_SPIMEM_TUNNEL_CMD_WRITE_BUFFER  CMD_SET(SW_SPIMEM_TUNNEL_CMD_DATA)
#define SW_SPIMEM_TUNNEL_CMD_READ          CMD_GET(SW_SPIMEM_TUNNEL_CMD_APPLY)
#define SW_SPIMEM_TUNNEL_CMD_WRITE         CMD_SET(SW_SPIMEM_TUNNEL_CMD_APPLY)
#define SW_SPIMEM_TUNNEL_CMD_ERASE_BLOCK   CMD_SET(SW_SPIMEM_TUNNEL_CMD_ERASE)

#define TUNNEL_SLAVE_ADD_LEN_7BIT          0

typedef enum {
  SPIMEM_ID,
  SPIMEM_STATUS,
  SPIMEM_CONFIG
} SpimemTunnelCmd;

#define SPIMEM_UNPROTECT 0x00
#define SPIMEM_PROTECT   0x01

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define CRC_FMASK     0xEDB88320  // 1110 1101 1011 1000 1000 0011 0010 0000
#define CRC_BMASK     0xDB710641  // 1101 1011 0111 0001 0000 0110 0100 0001
#define CRC_SEED_FWRD  ((dword)0x12345678)
#define CRC_SEED_BACK  (('S'<<24)|('C'<<16)|('R'<<8)|('C'<<0))
#define dwsize(a)      (((a)+sizeof(dword)-1)/sizeof(dword))
#define dwsizeof(a)    dwsize(sizeof(a))

#define MAGIC2CHARS(m)      ((m)>>0)&0xff, ((m)>>8)&0xff,  ((m)>>16)&0xff, ((m)>>24)&0xff
#define FILEID2CHARS(i)     (((i)>>nID0)&0x3F)+' ', \
                             (((i)>>nID1)&0x3F)+' ', \
                             (((i)>>nID2)&0x3F)+' ', \
                             (((i)>>nID3)&0x3F)+' '

// from efs from here
#ifndef BUILD_CHUNK_UTILITY
typedef uint16_t blknr;
typedef unsigned int                      dword;
#ifndef PACKED
#define PACKED
#endif
typedef unsigned short          word;
typedef unsigned char           byte;
#endif

#define EFS_MAGIC(a,b,c,d)                (((a)<<(0*8))|((b)<<(1*8))|((c)<<(2*8))|((d)<<(3*8)))

#define EFS_MAGIC_LOAD                    EFS_MAGIC('E','F','L','C')  // Magic for loader with CRC checking
#define EFS_MAGIC_LOAD_NC                 EFS_MAGIC('E','F','L','N')  // Magic for loader without CRC checking
#define EFS_MAGIC_SYSTEM                  EFS_MAGIC('E','F','S','C')  // Magic for system with CRC checking
#define EFS_MAGIC_SYSTEM_NC               EFS_MAGIC('E','F','S','N')  // Magic for system without CRC checking

#define EFS_MAGIC_SYSTEM_FLASHED          EFS_MAGIC('E','F','S','1')  // Magic for system Flased
#define EFS_MAGIC_SYSTEM_1ST_AFTER_BL     EFS_MAGIC('E','F','S','2')  // Magic for system 1st passed bootloader after being Flased
#define EFS_MAGIC_SYSTEM_VALIDATED        EFS_MAGIC('E','F','S','V')  // Magic for system validated - this is validated to be a good partition
#define EFS_MAGIC_SYSTEM_NC_FLASHED       EFS_MAGIC('E','F','N','1')  // Magic for system Flased
#define EFS_MAGIC_SYSTEM_NC_1ST_AFTER_BL  EFS_MAGIC('E','F','N','2')  // Magic for system 1st passed bootloader after being Flased
#define EFS_MAGIC_SYSTEM_NC_VALIDATED     EFS_MAGIC('E','F','N','V')  // Magic for system validated - this is validated to be a good partition

#define EFS_BLOCKSIZE                     4096                      // size of a SFS block (min allocation unit)
#define EFS_MAGIC_INVALIDATED             0x00000000

#define EFS_TYPE_SYSTEM                   2

struct efs_record_s
  {
  dword body[248/4];
  dword crc1;
  dword crc2;
  } PACKED;

struct partition
{
  byte type;
  byte attribute;
  word reserved;
  blknr start;
  blknr size;
} PACKED;

#define MAX_PART_NUM                      8
struct partition_table
{
  byte total;
  byte num_system;
  byte num_persist;
  byte boot;
  dword reserved;
  struct partition system[MAX_PART_NUM];
} PACKED;

#define AES_IV_SIZE    4
#define VER_SIZE	4
struct efs_header_common_s
{
  // basic information for a partition
  dword magic;        // MagicNumber
  dword crc;          // CRC32 for HW CRC verification

  // start address(i.e., 256 bytes) and size of image. CRC32
  dword start;        // Start/Entry address
  dword size;         // Size

  word  vid;			// Vendor ID
  word  pid;			// Product ID
  dword bt_ver;		// Bootloader version

  dword addr_load;
  dword addr_run;

  byte  validation;   // validation check
  byte  security;     // security usage
  word  signature;    // size of signature

  blknr base;         // base block number for bootloader partition
  blknr bsize;        // size in block number

  dword iv[AES_IV_SIZE];

  dword version[VER_SIZE]; // firmware version 0.0.0.0~

} PACKED;

struct efs_partition_bootloader_s
{
  struct efs_header_common_s hdr;
  struct partition_table table;
} PACKED;

struct efs_partition_system_s
{
  struct efs_header_common_s hdr;
  blknr sfs_offset;
  blknr sfs_size;
} PACKED;

union efs_header_u
{
  // common efs header
  struct efs_header_common_s hdr;
  // bootloader partition header
  struct efs_partition_bootloader_s boot;
  // system partition header
  struct efs_partition_system_s system;

  dword efs_dword[256/4];
  struct efs_record_s    rec;
} PACKED;

// from efs to here

struct Spimem_s 
{
  uint32_t BufferSize;
  int32_t  IsTunnelConfigured;
};
Spimem_s Spimem; 
/* Static memory */
static int32_t firmware_mgr_reset(void)
{
  Command cmd = {0};
  //golem::sendcmd  CTRL CONTROL_MGR_RESET {1, 1}
  int32_t err_no = -CXERR_NOERR;
  err_no = SENDCMD(&cmd, ID_CTRL, CONTROL_MGR_RESET, 1, 1);
  // Even if there is an error, not indicating it as the CAPEs themselves can be clock gated in some cases.
//  if (err_no < 0)
//      return -CXERR_MGR_RESET_ERROR;
  return -CXERR_NOERR;
}

/******************************* Static Functions **************************************/

// return number of NON 0xffffffff data within specified range.
static uint16_t num_non_ff(uint16_t range, const uint32_t *data)
{
  const uint32_t *cur = data + range - 1;
  for (; cur >= data; cur--)
  {
    if (*cur != 0xffffffff)
    {
      break;
    }
  }
  if ((cur - data + 1) == 0)
    return 0;

  return (uint16_t)(cur - data + 1);
}

/*
* Convert a 4-byte number from a ByteOrder into another ByteOrder.
*/
static uint32_t byte_swap_ulong(uint32_t i)
{
  return ((i & 0xff) << 24) + ((i & 0xff00) << 8) + ((i & 0xff0000) >> 8) + ((i >> 24) & 0xff);
}

static void __memcpy(uint8_t *destaddr, uint8_t *srcaddr, int32_t len)
{
  uint8_t *dest = destaddr;
  uint8_t *src  = srcaddr;
  while(len-- > 0)
  {
    *dest++ = *src++;
  }
  //return destaddr;
}

static int __memcmp(uint8_t* src, uint8_t* dest, int n)
{
 while (n--)
   if (*src++ != *dest++)
     return 1;

  return 0;
}

static int32_t SpimemClose(void)
{
  int32_t err_no = -CXERR_NOERR;
  int32_t status = SENDCMD_RC_SUCCESS;
  Command cmd = {0};
  status = SENDCMD(&cmd, (ID_MM|FWID_SW), SW_SPIMEM_TUNNEL_CMD_SET_CONFIG, 0, 1, 0, 0, 0);
  if (status != SENDCMD_RC_SUCCESS)
  {
    err_no = -CXERR_SPI_CLOSE_ERROR;
  }
  else
  {
    Spimem.IsTunnelConfigured = false;
    Spimem.BufferSize = 0;
  }

  return err_no;
}

static int32_t SpimemOpen(uint32_t *pBbufferSize, uint32_t stepSize)
{
  int32_t err_no = -CXERR_NOERR;
  // If the default configuration is required then assume that header information (or smaller amount)
  // should be read. In case of bigger buffers (like firmware upgrade) the recommended buffer
  // size should be indicated explicitly.
  uint32_t bufferSize = (NULL == pBbufferSize ? EFS_BLOCKSIZE : *pBbufferSize);
  uint32_t step = (0 == stepSize ? bufferSize / 4 : min(stepSize, bufferSize));

  int32_t status = CXHID_STATUS_OK;

  bool isConfigured = false;
  while (CXHID_STATUS_SUCCESS(status) && bufferSize > 0 && !isConfigured)
  {
    Command cmd = {0};

    //LogNumber(MSG_CON, "CxAudioDev20805::SpimemOpen(bufferSize)", bufferSize);
    status = SENDCMD(&cmd, (ID_MM|FWID_SW), SW_SPIMEM_TUNNEL_CMD_SET_CONFIG, 0, 1, bufferSize, bufferSize);

    if (CXHID_STATUS_SUCCESS(status))
    {
      isConfigured = true;
    }
    else if ((status == SENDCMD_RC_CMD_ERROR) && (MODULE_RC_ALLOC_FAILURE == cmd.head.num_32b_words))
    {
      // Decrease the buffer size and try again
      bufferSize -= min(bufferSize, step);
      status = CXHID_STATUS_OK;
    }
  }

  Spimem.IsTunnelConfigured = (CXHID_STATUS_SUCCESS(status) && 0 != bufferSize) ? true : false;
  Spimem.BufferSize = (CXHID_STATUS_SUCCESS(status) ? bufferSize : 0);

  if (NULL != pBbufferSize)
  {
    *pBbufferSize = Spimem.BufferSize;
  }

  if (!isConfigured || 0 == bufferSize)
  {
    //status = CXHID_STATUS_CAPE_COMMAND_ERROR;
    err_no = -CXERR_INVALID_PARAMETER;
  }

  return err_no;
}

static int32_t SpimemEraseBlock(uint32_t blockNo)
{
  Command cmd = {0};
  int32_t err_no = -CXERR_NOERR;
  err_no = SENDCMD(&cmd, (ID_MM|FWID_SW), SW_SPIMEM_TUNNEL_CMD_ERASE_BLOCK, 0, SFS_BLK2POS(blockNo));
  if (err_no != (int32_t)0)
    return -CXERR_SPI_ERASE_ERROR;
  return err_no;
}

static int32_t SpimemProtect(int32_t mode)
{
  Command cmd = {0};
  int32_t err_no = -CXERR_NOERR;

  err_no = SpimemOpen(NULL, 0);
  if (err_no != (int32_t)0)
    return err_no;
  err_no = SENDCMD(&cmd, (ID_MM|FWID_SW), SW_SPIMEM_TUNNEL_CMD_PROTECT, 0, mode);
  if (err_no != (int32_t)0)
    return -CXERR_SPI_PROTECT_ERROR;
  return err_no;
}

static int32_t SpimemOperation(
  CXHID_OPERATION memOperation,
  uint32_t memAddr,
  uint32_t memSize,
  uint8_t *bufAddr,
  uint32_t bufSize)
{
  int32_t status = CXHID_STATUS_OK;
  bool isReadOperation = CXHID_OPERATION_READ == memOperation ? true : false;

  // Check operation type
  if (CXHID_STATUS_SUCCESS(status))
  {
    if (CXHID_OPERATION_READ != memOperation && CXHID_OPERATION_WRITE != memOperation)
    {
      //status = CXHID_STATUS_INVALID_OPERATION;
      status = -CXERR_INVALID_PARAMETER;
      {
        DUMP("ERROR:SpimemOperation:-CXERR_INVALID_PARAMETER\r\n");
      }
    }
  }

  if (CXHID_STATUS_SUCCESS(status) && !Spimem.IsTunnelConfigured)
  {
    status = SpimemOpen(NULL, 0);
    //if(status == (int32_t)0) {DUMP("SpimemOperation: SpimemOpen->PASS\r\n"); }else { DUMP("SpimemOperation: SpimemOpen->FAIL IsTunnelConfigured  \r\n"); }
  }

  uint32_t remainingBytes = min(memSize, bufSize);
  uint8_t *pByte = bufAddr;
  while (CXHID_STATUS_SUCCESS(status) && remainingBytes > 0)
  {
    uint32_t ioSize = min(Spimem.BufferSize, remainingBytes);

    Command cmd = {0};
    cmd.head.num_32b_words = 3;
    cmd.head.command_id =
        isReadOperation ? SW_SPIMEM_TUNNEL_CMD_READ : SW_SPIMEM_TUNNEL_CMD_WRITE;
    cmd.head.mgr_module_id = (ID_MM|FWID_SW);
    cmd.data[0] = 0;       // Tunnel ID
    cmd.data[1] = memAddr; // Address
    cmd.data[2] = ioSize;  // Size in bytes

    if (CXHID_STATUS_SUCCESS(status) && isReadOperation) //READ
    {
      status = AudioSmart_mid_sendcmd((uint32_t *)&cmd);
      if (status < 0)
      {
        DUMP("ERROR:AudioSmart_mid_sendcmd FAIL 1 %d \r\n", status);
      }
    }

    uint32_t nRemainingDwords = ioSize / sizeof(cmd.data[0]);
    while (CXHID_STATUS_SUCCESS(status) && nRemainingDwords > 0)
    {
      uint32_t payloadSizeInDwords = isReadOperation ? min(nRemainingDwords, MAX_COMMAND_SIZE) : min(nRemainingDwords, MAX_COMMAND_SIZE - 1); // One parameter indicates

      Command cmd = {0};

      cmd.head.num_32b_words = isReadOperation ? payloadSizeInDwords : payloadSizeInDwords + 1;
      cmd.head.command_id =
          isReadOperation ? SW_SPIMEM_TUNNEL_CMD_READ_BUFFER : SW_SPIMEM_TUNNEL_CMD_WRITE_BUFFER;
      cmd.head.mgr_module_id = (ID_MM|FWID_SW);
      cmd.data[0] = 0;     // Tunnel ID
      if (isReadOperation) //READ
      {
        //cmd.data[1] = MAX_COMMAND_SIZE * sizeof(cmd.data[0]); // Size in bytes
        cmd.data[1] = payloadSizeInDwords * sizeof(cmd.data[0]); // Size in bytes
      }
      else //WRITE
      {
        int nBytes = payloadSizeInDwords * sizeof(cmd.data[0]);
        if (nBytes > 0)
        {
          __memcpy((uint8_t *)&cmd.data[1], pByte, nBytes);
          pByte += nBytes;
          nRemainingDwords -= payloadSizeInDwords;
        }
        else
        {
          //ASSERT(0 == nBytes);
          break;
        }
      }

      status = AudioSmart_mid_sendcmd((uint32_t *)&cmd); //SEND CMD TO READ OR WRITE

      if (CXHID_STATUS_SUCCESS(status))
      {
        if (isReadOperation)
        {
          nRemainingDwords -= payloadSizeInDwords;
          if (0 == cmd.head.num_32b_words)
          {
            //ASSERT(0 == nRemainingDwords);
            break;
          }
          else
          {
            int nBytes = payloadSizeInDwords * sizeof(cmd.data[0]);
            __memcpy(pByte, (uint8_t *)cmd.data, nBytes);
            pByte += nBytes;
          }
        }
      }
      else
      {
        if (status < 0)
        {
          DUMP("ERROR:AudioSmart_mid_sendcmd FAIL 2 \r\n");
        }
      }
    }

    if (CXHID_STATUS_SUCCESS(status) && !isReadOperation) //WRITE
    {
      status = AudioSmart_mid_sendcmd((uint32_t *)&cmd);
    }
    else
    {
      //if(status < 0) { DUMP("AudioSmart_mid_sendcmd FAIL \r\n");}
    }

    remainingBytes -= ioSize;
    memAddr += ioSize;
  }

  return status;
}

static int32_t SpimemRead(
    uint32_t memAddr,
    uint32_t memSize,
    uint8_t *bufAddr,
    uint32_t bufSize)
{
  int32_t err_no = -CXERR_NOERR;
  memSize = ((memSize + 3) & ~3); // The mem size must be a multiple of 8
  err_no = SpimemOperation(CXHID_OPERATION_READ, memAddr, memSize, bufAddr, bufSize);
  if (err_no != -CXERR_NOERR)
    return -CXERR_SPI_READ_ERROR;
  return err_no;
}

static int32_t SpimemWrite(
    uint32_t memAddr,
    uint32_t memSize,
    uint8_t *bufAddr,
    uint32_t bufSize)
{
  return SpimemOperation(CXHID_OPERATION_WRITE, memAddr, memSize, bufAddr, bufSize);
}

static int32_t SpimemWriteBlock(
    uint32_t blockNo,
    uint8_t *bufAddr,
    uint32_t bufSize)

{
  int32_t err_no = -CXERR_NOERR;
  do
  {
    err_no = SpimemEraseBlock(blockNo);
    if (err_no != -CXERR_NOERR)
      break;
    err_no = SpimemWrite(SFS_BLK2POS(blockNo), EFS_BLOCKSIZE, bufAddr, bufSize);
    if (err_no != -CXERR_NOERR)
      break;
	break;
  } while (0);
  return err_no;
}

static int32_t SpimemGetId(uint32_t *spimemId)
{
  int32_t err_no = -CXERR_NOERR;
  Command cmd = {0};

  err_no = SpimemOpen(NULL, 0);
  if (err_no != (int32_t)0)
    return err_no;

  err_no = SENDCMD(&cmd, (ID_MM|FWID_SW), SW_SPIMEM_TUNNEL_GET_INFO, 0, SPIMEM_ID);
  if (err_no == (int32_t)0)
  {
    *spimemId = cmd.data[0];
    DUMP("INFO:  SpimemGetId 0x%X \r\n", *spimemId);
  }
  else
  {
    err_no = -CXERR_SPI_GET_ID_ERROR;
    *spimemId = 0;
    DUMP("ERROR:FAIL:SpimemGetId 0x%X \r\n", *spimemId);
  }
  return err_no;
}

static int32_t spi_write_verify(uint16_t sector, uint16_t length, const void *data, dword *)
{
  uint8_t verify_buffer[EFS_BLOCKSIZE]; //TODO: big buffer should request from outside  instead of stack.
  int32_t err_no = -CXERR_NOERR;
  err_no = SpimemProtect(SPIMEM_UNPROTECT);
  if (err_no != CXERR_NOERR)
    if (err_no != -CXERR_NOERR)
        return err_no;
  err_no = SpimemWriteBlock(sector, (uint8_t *)data, length);
  if (err_no != -CXERR_NOERR)
  {
    return err_no;
  }                                                                               //erase as sector and write a block/length
  err_no = SpimemRead(SFS_BLK2POS(sector), EFS_BLOCKSIZE, verify_buffer, length); //read back the block had been written, compare

  if (__memcmp((uint8_t *)data, (uint8_t *)verify_buffer, length) != 0)
  {
    err_no = -CXERR_SPI_VERIFY_ERROR;
  } //cpmpare the cotent

  return err_no;
}

static int32_t invalid_a_header(AudioSmart_spi_flash_descriptor *spimem_descriptor, uint16_t header_idx)
{
  int32_t err_no = -CXERR_NOERR;
  int32_t c;
  dword write_buffer = EFS_MAGIC_INVALIDATED;
  c = spimem_descriptor->sys_part[header_idx].blk_start;
  DUMP("INFO:  invalid the header of partition 0x%x\r\n", c);
  err_no = spi_write_verify((uint16_t)c, (uint16_t)sizeof(dword), (const void *)&write_buffer, NULL);

  return err_no;
}

#define CRC32_POLYREVERSE                 0xEDB88320



// CRC-32: x32 + x26 + x23 + x22 + x16 +  x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x1 + 1
// POLY:      0x04C11DB7
// POLY_REVERSE:  0xEDB88320
unsigned int crc_table[256] = { 0 };

static int crc_tableinit(unsigned int poly, int rev)
{
  int ret = 0;
  unsigned int crc;
  unsigned int n, k;

//  memset (crc_table, 0, sizeof(crc_table));
  for (n = 0; n < 256; n++)
  {
    crc = n;

    for(k=0; k<8; k++)
    {
      crc = crc & 0x00000001 ? poly ^ (crc >> 1) : crc >> 1;
    }
      crc_table[n] = crc;
  }
  return (ret);
}

static unsigned int crc_revert(unsigned int crc)
{
  unsigned int output = 0;
  int i = 0;

  for (i = 0; i < 32; i++) {
    output = (output << 1) + (crc & 1);
    crc >>= 1;
  }

  return output;
}


static unsigned int crc_calc32 (const unsigned char *buf, unsigned int len)
{
  unsigned int crc = 0xFFFFFFFF;
  while (len--)
    {
    crc = ( crc >> 8 ) ^ crc_table[ (crc ^ *buf ) & 0xff];
      buf++;
    }
  return crc_revert(crc);
}

#define EFS_SIG_1 11539
#define EFS_SIG_2 8867
static void calculate_crc (unsigned char *data_ptr, unsigned int size, dword *crc1_ptr, dword *crc2_ptr)
{
  unsigned int crc_val;

  crc_tableinit(CRC32_POLYREVERSE, 1); // initialization of crc table
  crc_val = crc_calc32((const unsigned char *)data_ptr, size);
  *crc1_ptr = (crc_val >> 16) * EFS_SIG_1; // part1 of expanding 32bit to 32+32 bits
  *crc2_ptr = (crc_val & 0xFFFF) * EFS_SIG_2; // part2 of expanding 32bit to 32+32bits
}

static int get_custom_partition_info (uint32_t* cust_part_start, uint32_t* cust_part_size, uint32_t partition_num)
{
	int32_t err_no = -CXERR_NOERR;
	Command cmd = { 0 };
	err_no = SENDCMD(&cmd, (ID_MM | FWID_SW), SW_SFS_CMD_CUSTOM_PARTITION_INFO, partition_num);
	if (err_no != -CXERR_NOERR)
		return err_no;

	*cust_part_start = (cmd.data[0] & 0xFFFF);
	*cust_part_size = ((cmd.data[0] & 0xFFFF0000) >> 16);
	DUMP(" Part:2 START:%d  SIZE:%d  \n", *cust_part_start, *cust_part_size);

	return err_no;
}

/*-----------------------------------------------------------------------------------------------------------------------------------*/

int32_t AudioSmart_bg_upgrade_reset_device(void)
{
  int32_t status = 0;
  int32_t err_no = CXERR_NOERR;
  Command cmd = { 0 };

  err_no = SENDCMD(&cmd, (ID_MM | FWID_SYS), SYS_CMD_SOFTRESET);

  return err_no;
}


int32_t AudioSmart_bg_get_current_sys_partition_number(void)
{
  int32_t status;
  int32_t ret_val;

  Command cmd = {0};
  status = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_CURRENT_SYS_PARTITION));
  if ((status == SENDCMD_RC_SUCCESS))
  {
    // Any of the failures below indicate problem in the CX20805 FW
    ret_val = cmd.data[0];
    DUMP("INFO: System Partition Number: [%d]\n", ret_val);
  }
  else
  {
    ret_val = status;
  }

  return ret_val;
}


int32_t AudioSmart_bg_get_firmware_version(uint32_t *ver_z, uint32_t *ver_y, uint32_t *ver_x, uint32_t *ver_w)
{
  int32_t status;

  Command cmd = {0};
  status = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_VERSION));
  if ((status == SENDCMD_RC_SUCCESS))
  {
    // Any of the failures below indicate problem in the CX20805 FW
    *ver_z = cmd.data[0];
    *ver_y = cmd.data[1];
    *ver_x = cmd.data[2];
    *ver_w = cmd.data[3];
    DUMP("INFO: Read firmware version : [%d.%d.%d.%d]\n", (uint8_t)(*ver_z),
         (uint8_t)(*ver_y), (uint8_t)(*ver_x), (uint8_t)(*ver_w));
  }
  else
  {
    return -CXERR_FW_VER_GET_ERROR;
  }

  return status;
}

int32_t AudioSmart_bg_get_bl_version(uint32_t *bl_ver, uint32_t *ver_z, uint32_t *ver_y, uint32_t *ver_x, uint32_t *ver_w)
{
  int32_t status;

  Command cmd = {0};
  status = SENDCMD(&cmd, (ID_MM|FWID_SYS), CMD_GET(SYS_CMD_BL_VERSION));
  if ((status == SENDCMD_RC_SUCCESS))
  {
    // Any of the failures below indicate problem in the CX20805 FW
    *bl_ver = cmd.data[0];
    *ver_z  = cmd.data[1];
    *ver_y  = cmd.data[2];
    *ver_x  = cmd.data[3];
    *ver_w  = cmd.data[4];
    DUMP("INFO: Read Bootloader version : %d built with FW [%d.%d.%d.%d]\n", (uint8_t)(*bl_ver),(uint8_t)(*ver_z),
         (uint8_t)(*ver_y), (uint8_t)(*ver_x), (uint8_t)(*ver_w));
  }
  else
  {
    return -CXERR_BL_VER_GET_ERROR;
  }

  return status;
}

int32_t AudioSmart_validate_fw_headers(const uint8_t *img, uint32_t image_size, uint16_t starting_spi_blk_num, uint16_t blk_num_offset,
                                       bool mn_checking, void (*delay_fn_ptr)(uint32_t delay_in_ms),
                                       AudioSmart_spi_flash_descriptor *spimem_descriptor, AudioSmart_img_descriptor *img_descriptor)
{
  int32_t err_no = -CXERR_NOERR;
  struct efs_partition_system_s *headers  = (struct efs_partition_system_s *)img ; // headers pointer (3 blocks=12KB)
  union efs_header_u *sys_part_hdr        = (union efs_header_u *)img;
  struct efs_partition_bootloader_s  *efs_bootloader_header;
  int i, j;
  uint32_t num_blks = 0;
  uint8_t temp_buffer[EFS_BLOCKSIZE];
  dword crc1, crc2;

  efs_bootloader_header = (struct efs_partition_bootloader_s  *)(&temp_buffer[0]);
  /* step 1  Parsing the SPI device */
  DUMP("/*step 1  Reading the SPI device ID */ \r\n");
  err_no = SpimemGetId(&spimem_descriptor->id);
  if (err_no != -CXERR_NOERR)
    return err_no;
  *spimem_descriptor->spi_name_str = '\0';

  /* Reading the Bootloader header to find information about the firmware in the device */
  err_no = SpimemRead(SFS_BLK2POS(starting_spi_blk_num), sizeof(struct efs_partition_bootloader_s), (uint8_t *)efs_bootloader_header, EFS_BLOCKSIZE);
  if (err_no != -CXERR_NOERR)
    return err_no;

  /* Check whether the magic number is right for Bootloader Partition, to make sure we are talking to the right device */
  if ((efs_bootloader_header->hdr.magic != EFS_MAGIC_LOAD) && (efs_bootloader_header->hdr.magic != EFS_MAGIC_LOAD_NC)) {
    err_no = -CXERR_INVALID_BOOTLOADER;
    return err_no;
  }

  if (mn_checking) {
    if ( (headers->hdr.vid != efs_bootloader_header->hdr.vid) || (headers->hdr.pid != efs_bootloader_header->hdr.pid) ) {
      err_no = -CXERR_MAGIC_NUM_MISMATCH;
      return err_no;
    }
  }



  spimem_descriptor->num_sys_images = efs_bootloader_header->table.num_system;

  /* Find the size of the system partition on the device, and save the information about the system partitions */
  for (i = 0, j=0; i < efs_bootloader_header->table.total; i++) {
    if (efs_bootloader_header->table.system[i].type == EFS_TYPE_SYSTEM) {
      spimem_descriptor->sys_part[j].blk_start = efs_bootloader_header->table.system[i].start - blk_num_offset + starting_spi_blk_num;
      spimem_descriptor->sys_part[j].attribute = efs_bootloader_header->table.system[i].attribute;
      spimem_descriptor->sys_part[j].num_blks  = efs_bootloader_header->table.system[i].size;
      if (num_blks == 0) {
        num_blks = spimem_descriptor->sys_part[j].num_blks;
      } else {
        if (num_blks != spimem_descriptor->sys_part[j].num_blks) {
          err_no = -CXERR_INVALID_BOOTLOADER;
          return err_no;
        }
      }
      j++;
    }
  }
  if ((j== 0) || (j != efs_bootloader_header->table.num_system)) {
    err_no = -CXERR_INVALID_BOOTLOADER;
    return err_no;
  }

  /* Check whether the size of the incoming System image is right - meaning it will fit into the allocated System Partition */
  if ( image_size > (num_blks * EFS_BLOCKSIZE) ) {
    err_no = -CXERR_IMAGE_SIZE_TOO_BIG;
    return err_no;
  }

  /* Checking the System Partition image */

  /* Check whether the Magic number is right inside of the System Partition header */
  if ((headers->hdr.magic != EFS_MAGIC_SYSTEM) && (headers->hdr.magic != EFS_MAGIC_SYSTEM_NC)) {
    err_no = -CXERR_INVALID_IMAGE;
    return err_no;
  }

  /* Check whether the number of blocks for System Partition Partition are same as in the bootloader partition table */
  if ((headers->hdr.bsize != num_blks)) {
    err_no = -CXERR_INVALID_IMAGE;
    return err_no;
  }

  /* Check the CRC of the System partition header */
  calculate_crc((unsigned char *)sys_part_hdr, sizeof(struct efs_record_s)-8, &crc1, &crc2);
  if ( (crc1 != sys_part_hdr->rec.crc1) || (crc2 != sys_part_hdr->rec.crc2) ) {
    err_no = -CXERR_CRC_CHECK_ERROR;
  }

  img_descriptor->fw_img_version[0] = headers->hdr.version[0];
  img_descriptor->fw_img_version[1] = headers->hdr.version[1];
  img_descriptor->fw_img_version[2] = headers->hdr.version[2];
  img_descriptor->fw_img_version[3] = headers->hdr.version[3];

  return err_no;
}

int32_t AudioSmart_start_custom0_partition_flashing(uint32_t *cus0_partition_start, uint32_t *cus0_partition_size, uint32_t custom0_fsz_in_block)
{
  int32_t err_no = -CXERR_NOERR;
  Command cmd = {0};
  uint32_t partition_num = 2; // PARTITION_CUSTOM0

  err_no = get_custom_partition_info(cus0_partition_start, cus0_partition_size, partition_num);
  if (err_no != -CXERR_NOERR)
    return err_no;

  if (*cus0_partition_size != custom0_fsz_in_block)
  {
    return CXERR_INVALID_IMAGE;
  }

  // Let System know to start BG UPGRADE process
  err_no = SENDCMD(&cmd, (ID_MM | FWID_SYS), CMD_GET(SYS_CMD_START_BG_UPGRADE));
  if (err_no != -CXERR_NOERR)
    return err_no;

  return err_no;
}

int32_t AudioSmart_flash_a_block_of_cus_partition(const uint8_t *block_buff, uint32_t block_number) 
{
	int32_t err_no = CXERR_NOERR;
	uint32_t len;

	if (block_buff == NULL)
	{
		err_no = -CXERR_MALLCO_FAIL;
		return err_no;
	}

	/* flash one block to device */
	len = num_non_ff(EFS_BLOCKSIZE / sizeof(uint32_t), (uint32_t *)block_buff);
	if (len) // Non-Padding block
	{
		err_no = spi_write_verify((uint16_t)block_number, (uint16_t)len * sizeof(uint32_t), block_buff, NULL);
	}
	else
	{
		err_no = SpimemEraseBlock(block_number);
	}

	if (err_no != CXERR_NOERR) // Note:don't care the below err_no, only care the original err_no
	{
		// protect the device
		SpimemProtect(SPIMEM_PROTECT);
		// close the device
		SpimemClose();

		DUMP("ERROR: Failed to download image block #%u err:%d\n", block_number, err_no);
		return err_no;
	}
	return err_no;
}

int32_t AudioSmart_start_fw_image_flashing(const uint8_t *img, void (*delay_fn_ptr)(uint32_t delay_in_ms),
                                           AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor)
{
  int32_t err_no = -CXERR_NOERR;
  union efs_header_u system_header;
  uint16_t i, validated;

  do
  {
    /*step 0.1  */
    //err_no = firmware_mgr_reset();
    //if (err_no != -CXERR_NOERR)
    //  break;

    /* TODO: The following should ideally be part of the firmware or bootloader itself and the tool ideally doesn't need */
    /*       to know where the firmware update needs to be happen */
    /* Go through all of the system partitions and find the active partition */
    spimem_descriptor->active_partition = 0xFF;
    spimem_descriptor->updating_partition = 0xFF;
    validated = 0;
    for (i = 0; i < spimem_descriptor->num_sys_images; i++) {
      /* Reading the System Partition header */
      err_no = SpimemRead(SFS_BLK2POS(spimem_descriptor->sys_part[i].blk_start), sizeof(efs_header_u), (uint8_t *)&system_header, sizeof(efs_header_u));
      if (err_no != -CXERR_NOERR)
        return err_no;
      if ( (system_header.system.hdr.magic == EFS_MAGIC_SYSTEM_VALIDATED) || (system_header.system.hdr.magic == EFS_MAGIC_SYSTEM_NC_VALIDATED) ) {
        spimem_descriptor->updating_partition = (spimem_descriptor->active_partition != 0xFF) ? i : spimem_descriptor->updating_partition;
        spimem_descriptor->active_partition   = (spimem_descriptor->active_partition != 0xFF) ? spimem_descriptor->active_partition : i;
        validated = 1;
      } else if (( (system_header.system.hdr.magic == EFS_MAGIC_SYSTEM) || (system_header.system.hdr.magic == EFS_MAGIC_SYSTEM_NC) ) && (validated == 0)) {
        spimem_descriptor->updating_partition = (spimem_descriptor->active_partition != 0xFF) ? i : spimem_descriptor->updating_partition;
        spimem_descriptor->active_partition   = (spimem_descriptor->active_partition != 0xFF) ? spimem_descriptor->active_partition : i;
      } else {
        spimem_descriptor->updating_partition = i;
      }
    }
    if ( (spimem_descriptor->active_partition == 0xFF) || (spimem_descriptor->updating_partition == 0xFF) ) {
      err_no = -CXERR_DEVICE_OUT_OF_CONTROL;
    }
    DUMP("INFO: Current Active System Partition:  %d\r\n", spimem_descriptor->active_partition);
    DUMP("INFO: System Partition to be Updated :  %d\r\n", spimem_descriptor->updating_partition);

    spimem_descriptor->curr_blk = spimem_descriptor->sys_part[spimem_descriptor->updating_partition].blk_start + 1;

    break;
  } while (0);
  return err_no;
}

int32_t AudioSmart_flash_a_block(const uint8_t *block,
                                 AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor)
{
  int32_t err_no = CXERR_NOERR;
  int32_t c;
  uint32_t len;

  c = spimem_descriptor->curr_blk;
  len = num_non_ff(EFS_BLOCKSIZE / sizeof(uint32_t), (uint32_t *)block);
  if (len)  //Non-Padding block
  {
    err_no = spi_write_verify((uint16_t)c, (uint16_t)len * sizeof(uint32_t), block, NULL);
  }
  else
  {
    err_no = SpimemEraseBlock(c);
  }
  spimem_descriptor->curr_blk++;

  if (err_no != CXERR_NOERR) // Note:don't care the below err_no, only care the original err_no
  {
    // protect the device
    SpimemProtect(SPIMEM_PROTECT);
    // close the device
    SpimemClose();
  }

  return err_no;
}

int32_t AudioSmart_end_fw_image_flashing(const uint8_t *block , void (*delay_fn_ptr)(uint32_t delay_in_ms),
                                         AudioSmart_img_descriptor *img_descriptor, AudioSmart_spi_flash_descriptor *spimem_descriptor)
{
  int32_t err_no = -CXERR_NOERR;
  int32_t pre_err_no = -CXERR_NOERR;
  do
  {
    /*TODO : Need support both partition update ? I2C_flash covert it right now */
    struct efs_partition_system_s *headers  = (struct efs_partition_system_s *)block ; // headers pointer (3 blocks=12KB)

    /* Change the Magic number of the first header to EFS_MAGIC_SYSTEM_FLASHED
     * (TODO: Might want to change this to EFS_MAGIC_SYSTEM_NC_FLASHED depending on whether to check CRC or not ) */
    headers->hdr.magic = EFS_MAGIC_SYSTEM_FLASHED;

    spimem_descriptor->curr_blk = spimem_descriptor->sys_part[spimem_descriptor->updating_partition].blk_start;
    err_no = AudioSmart_flash_a_block(block, img_descriptor, spimem_descriptor);
    if (err_no != -CXERR_NOERR)
      break;


    if (err_no != -CXERR_NOERR)
    {
      /*roll back , invalid the update partion header */
      /*step x.4.  invalid the update-partion's header */
      DUMP("/*step e.4 invalid the updating partion's header */  \r\n");
      err_no = SpimemProtect(SPIMEM_UNPROTECT);
      if (err_no != -CXERR_NOERR)
        break;
      err_no = invalid_a_header(spimem_descriptor, spimem_descriptor->updating_partition);
      if (err_no != -CXERR_NOERR)
        break;
      err_no = SpimemProtect(SPIMEM_PROTECT);
      if (err_no != -CXERR_NOERR)
        break;
    }
  } while (0);

  if (err_no != -CXERR_NOERR)
  {
    pre_err_no = err_no;
    DUMP("\r\n Imager transfer FAIL!\n");
  }

  do // should protect and close SPI no matter pass or fail
  {
    //step x.5 protect the device
    err_no = SpimemProtect(SPIMEM_PROTECT);
    if (err_no != -CXERR_NOERR)
      break;
    //step x.6 close the device
    err_no = SpimemClose();
    if (err_no != -CXERR_NOERR)
      break;
  } while (0);

  if ((err_no == -CXERR_NOERR) && (pre_err_no == -CXERR_NOERR))
  {
    DUMP("\nImage transfer Successfully!\n");
  }
  if (pre_err_no != -CXERR_NOERR)
  {
    return pre_err_no;
  }
  else
  {
    return err_no;
  }
}
