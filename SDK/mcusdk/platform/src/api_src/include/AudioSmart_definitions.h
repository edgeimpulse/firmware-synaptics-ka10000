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

  File Name: AudioSmart_definitions.h

------------------------------------------------------------------------------*/
#ifndef AudioSmart_DEFINITIONS_H
#define AudioSmart_DEFINITIONS_H

#include "syna_types.h"



#ifndef MCU_RAW_I2C
#define MCU_RAW_I2C                     0xD4
#endif

#ifndef MCU_I2C_SLAVE_BOOT 
#define MCU_I2C_SLAVE_BOOT              0xD8
#endif

#ifndef CONTROL_MGR_EVENT_HANDLER_ENABLE 
#define CONTROL_MGR_EVENT_HANDLER_ENABLE  95
#endif
#ifndef CAPE_DSP_RESET 
#define CAPE_DSP_RESET                  0xA6
#endif

#define ECOMM 70
/*----------------------------------------------------------------*/

#define PAR_INDEX_I2S_RX_WIDTH          14
#define PAR_INDEX_I2S_RX_DELAY          15
#define PAR_INDEX_I2S_RX_JUSTIFY        16
#define PAR_INDEX_I2S_RX_LR_POLARITY    17
#define PAR_INDEX_I2S_RX_NUM_OF_BITS    18
#define PAR_INDEX_I2S_TX_WIDTH          19
#define PAR_INDEX_I2S_TX_DELAY          20
#define PAR_INDEX_I2S_TX_JUSTIFY        21
#define PAR_INDEX_I2S_TX_LR_POLARITY    22
#define PAR_INDEX_I2S_TX_NUM_OF_BITS    23

#define I2S_PCM_SAMPLE_WIDTH__8_BIT 0
#define I2S_PCM_SAMPLE_WIDTH_16_BIT 1
#define I2S_PCM_SAMPLE_WIDTH_24_BIT 2
#define I2S_PCM_SAMPLE_WIDTH_32_BIT 3

#define ID(a,b,c,d)  ((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26))
#define _ID(a,b,c,d) ((uint32_t)((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26)))
#define nID0         8
#define nID1         14
#define nID2         20
#define nID3         26
#define cID(c,n)     (((c)>>(n))&0x3f)+0x20

// Retrieve the app and module id from an mgr_module_id
#define GET_MGR_ID(mgr_module_id)    ((mgr_module_id)&~0xFF)
#define GET_APP_ID(mgr_module_id)    GET_MGR_ID(mgr_module_id)
#define GET_MODULE_ID(mgr_module_id) ((mgr_module_id)& 0xFF)

#define SENDCMD_RC_SUCCESS              0
#define SENDCMD_RC_INVALID_ARG_NUM     -1 // Invalid number of arguments
#define SENDCMD_RC_FAILED_SEND         -2 // Failed to send command
#define SENDCMD_RC_REPLY_TIMEOUT       -3 // Timed out waiting for a reply
#define SENDCMD_RC_CMD_ERROR           -4 // Succeeded to send command, which returned an error


/*Error codes*/
#define CXERR_NOERR                         0
#define CXERR_MIS_ENDIANNESS                101
#define CXERR_WRITE_FAILED                  102
#define CXERR_INVALID_DATA                  103
#define CXERR_CHECKSUM_FAILED               104
#define CXERR_FAILED                        105
#define CXERR_INVALID_PARAMETER             106
#define CXERR_NOMEM                         107
#define CXERR_I2CFUN_NOT_SET                108
#define CXERR_UPDATE_MEMORY_FAILED          109
#define CXERR_DEVICE_NOT_RESET              110
#define CXERR_DEVICE_OUT_OF_CONTROL         111
#define CXERR_UPDATE_EEPROM_FAILED          112
#define CXERR_INVALID_BOOTLOADER            113
#define CXERR_INVALID_IMAGE                 114
#define CXERR_WAITING_RESET_TIMEOUT         115
#define CXERR_LOAD_IMG_TIMEOUT              116
#define CXERR_STATE_FATAL                   117
#define CXERR_CRC_CHECK_ERROR               118
#define CXERR_I2C_ERROR                     119
#define CXERR_BOOT_LOADER_NOT_FOND          120
#define CXERR_IMAGE_NOT_FOND                121
#define CXERR_WAITING_LOADER_TIMEOUT        123
#define CXERR_NULL_POINTER                  124
#define CXERR_I2C_BLOCK_NR_ERROR            125
#define CXERR_SEND_I2C_ERROR                126
#define CXERR_NO_MORE_DATA                  127
#define CXERR_UNSUPPORTED_FLASH_MEMORY      128
#define CXERR_SPI_PROTECT_ERROR             129
#define CXERR_SPI_MEM_READ_ERROR            130 
#define CXERR_SPI_MEM_WRITE_ERROR           131
#define CXERR_SPI_CLOSE_ERROR               132 
#define CXERR_SPI_READ_ERROR                133
#define CXERR_SPI_WRITE_ERROR               134
#define CXERR_SPI_ERASE_ERROR               135
#define CXERR_SPI_PARTION_NUM_ERROR         136
#define CXERR_SPI_OPEN_ERROR                137
#define CXERR_SPI_GET_ID_ERROR              138
#define CXERR_SPI_GET_STATUS_ERROR          139
#define CXERR_MGR_RESET_ERROR               140 
#define CXERR_SPI_VERIFY_ERROR              141 
#define CXERR_INTERFACE_USB_OPEN_ERROR      142  
#define CXERR_INTERFACE_I2C_OPEN_ERROR      143 
#define CXERR_IMAGE_CRC_CHECK_ERROR         144
#define CXERR_DEVICE_CRC_CHECK_ERROR        145
#define CXERR_MGR_CAPE_SOFT_RESET           146
#define CXERR_I2C_SEND_HASH_ERROR           147
#define CXERR_IMAGE_HEADERSIZE_MISMATCH     148
#define CXERR_FILE_SEEK_ERROR               149
#define CXERR_FILE_READ_ERROR               150

#define CXERR_I2CWRITE_DATA_MISALIGNMENT    201
#define CXERR_I2CWRITE_ADDR_MISALIGNEMNT    202
#define CXERR_I2CREAD_DATA_MISALIGNMENT     203
#define CXERR_I2CREAD_ADDR_MISALIGNEMNT     204

#define CXERR_FW_NO_RESPONSE                301
#define CXERR_FW_VER_INCORRECT              302
#define CXERR_FW_CANNOT_BOOT                303
#define CXERR_FW_VER_GET_ERROR              304
#define CXERR_FW_VER_CHECK_ERROR            305
#define CXERR_FW_GET_CURMODE                306
#define CXERR_FW_EXEC_FILE                  307
#define CXERR_FIND_PERSISTENT_BLK_ERROR     308
#define CXERR_CONTENT_VERIFY_ERROR          309
#define CXERR_BL_VER_GET_ERROR              310

#define CXERR_MAGIC_NUM_MISMATCH            401
#define CXERR_PARTITION_SIZE_MISMATCH       402
#define CXERR_PERISISTEN_ERASE_ERROR        403
#define CXERR_IMAGE_SIZE_TOO_BIG            404

#define CXERR_MALLCO_FAIL                   501
#define CXERR_SFWUG_DATA_PACK_ERR           502 


#ifndef CAF_INCLUDE_BASE_MODULE_EX_H_
#define CAF_INCLUDE_BASE_MODULE_EX_H_
/*------------------------------------------------------------------------------
  Type:       ModuleReturnCode

  Purpose:    Return codes of module's functions
------------------------------------------------------------------------------*/
#define MODULE_RC_FIRST -1024

typedef enum ModuleReturnCodeTag {
  MODULE_RC_SUCCESS                = 0,

  MODULE_RC_GENERIC_FAILURE        = MODULE_RC_FIRST- 1, // -1025
  MODULE_RC_ALREADY_EXISTS         = MODULE_RC_FIRST- 2, // -1026
  MODULE_RC_NULL_APP_POINTER       = MODULE_RC_FIRST- 3, // -1027
  MODULE_RC_NULL_MODULE_POINTER    = MODULE_RC_FIRST- 4, // -1028
  MODULE_RC_NULL_STREAM_POINTER    = MODULE_RC_FIRST- 5, // -1029
  MODULE_RC_NULL_POINTER           = MODULE_RC_FIRST- 6, // -1030

  MODULE_RC_BAD_APP_ID             = MODULE_RC_FIRST- 7, // -1031
  MODULE_RC_MODULE_ID_OUT_OF_RANGE = MODULE_RC_FIRST- 8, // -1032
  MODULE_RC_MODULE_ID_NOT_CREATED  = MODULE_RC_FIRST- 9, // -1033
  MODULE_RC_MODULE_TYPE_HAS_NO_API = MODULE_RC_FIRST-10, // -1034
  MODULE_RC_STREAM_ID_OUT_OF_RANGE = MODULE_RC_FIRST-11, // -1035
  MODULE_RC_STREAM_ID_NOT_CREATED  = MODULE_RC_FIRST-12, // -1036
  MODULE_RC_BAD_SIGNAL_TYPE        = MODULE_RC_FIRST-13, // -1037
  MODULE_RC_BAD_IO_TYPE            = MODULE_RC_FIRST-14, // -1038
  MODULE_RC_MODULE_LIB_NOT_LOADED  = MODULE_RC_FIRST-15, // -1039

  MODULE_RC_ALLOC_APP_FAILURE      = MODULE_RC_FIRST-17, // -1041
  MODULE_RC_ALLOC_HEAP_FAILURE     = MODULE_RC_FIRST-18, // -1042
  MODULE_RC_ALLOC_MODULE_FAILURE   = MODULE_RC_FIRST-19, // -1043
  MODULE_RC_ALLOC_STREAM_FAILURE   = MODULE_RC_FIRST-20, // -1044
  MODULE_RC_ALLOC_X32_FAILURE      = MODULE_RC_FIRST-21, // -1045 Obsolete
  MODULE_RC_ALLOC_X24_FAILURE      = MODULE_RC_FIRST-22, // -1046 Obsolete
  MODULE_RC_ALLOC_Y32_FAILURE      = MODULE_RC_FIRST-23, // -1047 Obsolete
  MODULE_RC_ALLOC_Y24_FAILURE      = MODULE_RC_FIRST-24, // -1048 Obsolete
  MODULE_RC_ALLOC_FAILURE          = MODULE_RC_FIRST-25, // -1049
  MODULE_RC_FILE_OPEN_ERROR        = MODULE_RC_FIRST-26, // -1050
  MODULE_RC_BAD_MEM_TYPE           = MODULE_RC_FIRST-27, // -1051
  MODULE_RC_BAD_MAGIC_NUMBER       = MODULE_RC_FIRST-28, // -1052

  // Codes specific to module's Command()
  MODULE_RC_CMD_MODE_UNSUPPORTED   = MODULE_RC_FIRST-32, // -1056 Unsupported mode (SET or GET)
  MODULE_RC_CMD_CODE_UNSUPPORTED   = MODULE_RC_FIRST-33, // -1057 Unknown value of command_code
  MODULE_RC_CMD_DATA_SIZE_WRONG    = MODULE_RC_FIRST-34, // -1058 Wrong value of int32_size_of_data
  MODULE_RC_CMD_DATA_INCORRECT     = MODULE_RC_FIRST-35, // -1059 Incorrect data in (*command_data)

  MODULE_RC_VALUE_OUT_OF_RANGE     = MODULE_RC_FIRST-36, // -1060
  MODULE_RC_INDEX_OUT_OF_BOUNDS    = MODULE_RC_FIRST-37, // -1061
  MODULE_RC_ARRAY_IS_FULL          = MODULE_RC_FIRST-38, // -1062

  MODULE_RC_TUNNEL_INVALID_ID      = MODULE_RC_FIRST-45, // -1069
  MODULE_RC_TUNNEL_NOT_READY       = MODULE_RC_FIRST-46, // -1070
  MODULE_RC_TUNNEL_INVALID_DEVICE  = MODULE_RC_FIRST-47, // -1071
  MODULE_RC_TUNNEL_DATA_UNALIGNED  = MODULE_RC_FIRST-48, // -1072
  MODULE_RC_FILE_NOT_PRESENT       = MODULE_RC_FIRST-49, // -1073

  MODULE_RC_ALLOC_HEAP_SEG0_FAILED = MODULE_RC_FIRST-50, // -1074
  MODULE_RC_ALLOC_HEAP_SEG1_FAILED = MODULE_RC_FIRST-51, // -1075
  MODULE_RC_ALLOC_HEAP_SEG2_FAILED = MODULE_RC_FIRST-52, // -1076
  MODULE_RC_ALLOC_HEAP_SEG3_FAILED = MODULE_RC_FIRST-53, // -1077
  MODULE_RC_ALLOC_HEAP_SEG4_FAILED = MODULE_RC_FIRST-54, // -1078
  MODULE_RC_ALLOC_HEAP_SEG5_FAILED = MODULE_RC_FIRST-55, // -1079
  MODULE_RC_ALLOC_HEAP_SEG6_FAILED = MODULE_RC_FIRST-56, // -1080
  MODULE_RC_ALLOC_HEAP_SEG7_FAILED = MODULE_RC_FIRST-57, // -1081
  MODULE_RC_ALLOC_HEAP_SEG8_FAILED = MODULE_RC_FIRST-58, // -1082
  MODULE_RC_ALLOC_HEAP_SEG9_FAILED = MODULE_RC_FIRST-59  // -1083
} ModuleReturnCode ;

#endif //CAF_INCLUDE_BASE_MODULE_EX_H_

// return 1 if the cpu is little_endian
static inline int endianess_is_little()
{
    unsigned int endianness = 1;
    return ((char*)(&endianness))[0]==1;
}

/*----------------------------------------------------------------------------*/
#define Q8_23_T(val)            (((val)>=256.f)?(int32_t)((1LL<<31)-1):(((val)<-256.f)?(int32_t)(-1LL<<31):(int32_t)((val)*(float)(1LL<<23))))
#define Q31_T(val)              (((val)>=1.f  )?(int32_t)((1LL<<31)-1):(((val)<-1.f  )?(int32_t)(-1LL<<31):(int32_t)((val)*(float)(1LL<<31))))
#define FLOAT_FROM_Q8_23_T(val) ((float)((int32_t)(val))*(1.0f/(float)(1LL<<23)))
#define FLOAT_FROM_Q31_T(val)   ((float)((int32_t)(val))*(1.0f/(float)(1LL<<31)))

/*----------------------------------------------------------------------------*/

//append from command.h

#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct CommandHeaderTag {
  int32_t   num_32b_words : 16;
  uint32_t  command_id : 15;
  uint32_t  reply : 1;
  uint32_t  mgr_module_id;
} CommandHeader;

#define COMMAND_OF_SIZE(n)   \
struct {                     \
  CommandHeader head;        \
  uint32_t      data[n];     \
}

// The maximum number of 32-bit data elements that a command can contain
#define MAX_COMMAND_SIZE 13

#define CMD_SET(item)                  ((item) & ~0x0100)
#define CMD_GET(item)                  ((item) |  0x0100)
#define CMD_PARAM(item)                ((item) |  0x0200)
#define CMD_CLR_PARAM(item)            ((item) & ~0x0200)
#define CMD_FWD(item)                  ((item) |  0x0400)
#define CMD_FWD_MASK(item)             ((item) &  0x0400)
#define CMD_CLR_FWD(item)              ((item) & ~0x0400)
#define CMD_FWD_TO_2ND_DEV(item)       ((item) |  0x0800)
#define CMD_FWD_TO_2ND_DEV_MASK(item)  ((item) &  0x0800)
#define CMD_CLR_FWD_TO_2ND_DEV(item)   ((item) & ~0x0800)
#define CMD_MASK                       (~(CMD_SET(0)|CMD_GET(0)|CMD_PARAM(0)|CMD_FWD(0)|CMD_FWD_TO_2ND_DEV(0)))
#define CMD_ITEM(cmd)                  ((cmd) & CMD_MASK)

#define CMD_REPLY 1
#if !defined _ID
#define _ID(a,b,c,d)  ((uint32_t)((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26)))
#endif

// Retrieve the app and module id from an mgr_module_id
#define GET_MGR_ID(mgr_module_id)    ((mgr_module_id)&~0xFF)
#define GET_APP_ID(mgr_module_id)    GET_MGR_ID(mgr_module_id)
#define GET_MODULE_ID(mgr_module_id) ((mgr_module_id)& 0xFF)

// Reserved App IDs
#define APP_ID_BROADCAST     0xFFFFFF00 // to broadcast commands to all apps

// Reserved module IDs
#define MODULE_ID_APP        0    // to send commands to the app
#define MODULE_ID_BROADCAST  0xFF // to broadcast commands to all modules

// The Command type may be used to point to commands of arbitrary
// sizes, for example:
// COMMAND_OF_SIZE(5) cmd
// Command *ptr = (Command *)&cmd;
typedef COMMAND_OF_SIZE(MAX_COMMAND_SIZE) Command;

//------------------------------------------------------------------------------

#define _NARGN(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, N, ...) N
#define _GRANN  13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#if defined(_MSC_VER)
#define LEFT_PAREN (
#define RIGHT_PAREN )
#define NARGS(...) (_NARGN LEFT_PAREN __VA_ARGS__##_GRANN, _GRANN RIGHT_PAREN)
#define FILL_COMMAND(mgr_module_id,cmd_id,...) \
    ((uint32_t)cmd_id)<<16 | NARGS(__VA_ARGS__), (uint32_t)(mgr_module_id), __VA_ARGS__
#elif defined(__GNUC__)

#if 1
#define NAGGS_TRY 1 
#if NAGGS_TRY
#define NARGS(...) \
              (sizeof(#__VA_ARGS__) == sizeof("")?0:_NARG_(__VA_ARGS__, _GRANN))

#define _NARG_(...) _NARGN(__VA_ARGS__)
#else
#define NARGS(...) (sizeof(( int[]){0,##__VA_ARGS__})/sizeof( int)-1)
#endif 
#endif

#define FILL_COMMAND(mgr_module_id,cmd_id,...) \
    ((uint32_t)cmd_id)<<16 | NARGS(__VA_ARGS__), (uint32_t)(mgr_module_id), ##__VA_ARGS__
#else
#define NARGS(...) \
              (sizeof(#__VA_ARGS__) == sizeof("")?0:_NARG_(__VA_ARGS__, _GRANN))

#define _NARG_(...) _NARGN(__VA_ARGS__)
#define FILL_COMMAND(mgr_module_id,cmd_id,...) \
    ((uint32_t)cmd_id)<<16 | NARGS(__VA_ARGS__), (uint32_t)(mgr_module_id), __VA_ARGS__
#endif

#endif // COMMAND_H_
//------------------------------------------------------------------------------

#define CMD_FORWARD(item)              ((item) | 0x0400)

//------------------------------------------------------------------------------
//append from "mcu_ex_sub.h"
typedef enum {
  FWID_SYS = 0,   //Target to SysCmdCode which in mcu_ex.h
  FWID_SW  = 2,   //Target to SwCmdCode  which in mcu_ex.h
  FWID_EXT = 3,   //Target to SDK Extension commands  which in mcu_ex.h
} FwModuleId;

/*
 * Sys related Commands Codes which not suitable to group to HW/SW.
 */
typedef enum {
  SYS_CMD_VERSION                       = 1,
  SYS_CMD_ACCESS_AHB                    = 2,
  SYS_CMD_SOFTRESET                     = 3,
  SYS_CMD_CONF_ERR_INFO                 = 8,
  SYS_CMD_EXEC_FILE                     = 9,
  SYS_CMD_PARAMETER_VALUE               = 14,

  SYS_CMD_EVENT_PARAM                   = 15,

  SYS_CMD_OS_TIME                       = 18, 
  SYS_CMD_MODE_ID                       = 19,
  SYS_CMD_SCRIPT_DATA                   = 20,
  SYS_CMD_SYS_CORE_CLK                  = 23, 
  SYS_CMD_BL_VERSION                    = 26,
  SYS_CMD_CURRENT_SYS_PARTITION         = 28,

  SYS_CMD_OS_MEM_USAGE                  = 32,

  SYS_CMD_IPC_CMD                       = 37,
  SYS_CMD_IPC_MSG                       = 38,
  SYS_CMD_EVENT_MASK                    = 39,
  SYS_CMD_SVN_VERSION                   = 43,

  SYS_CMD_SIDETONE_ENABLE               = 47,
  SYS_CMD_SIDETONE_VOLUME               = 48,

  SYS_CMD_GPIO_RESET_CONFIG             = 50,
  SYS_CMD_GPIO_RESET                    = 51,

  SYS_CMD_START_BG_UPGRADE              = 54,
} SysCmdCode;


/*
 * SW Drv related commands code.
 */
typedef enum {
  SW_CMD_NONE                           = 0,

  SW_SPIMEM_TUNNEL_CMD                  = 1,
  SW_SPIMEM_TUNNEL_CMD_CONFIG           = 2,
  SW_SPIMEM_TUNNEL_CMD_DATA             = 3,
  SW_SPIMEM_TUNNEL_CMD_APPLY            = 4,
  SW_SPIMEM_TUNNEL_CMD_ERASE            = 5,
  SW_SPIMEM_TUNNEL_CMD_PROTECT          = 6,

  SW_SFS_CMD_FILE_CREATE                = 7,
  SW_SFS_CMD_FILE_OPEN                  = 8,
  SW_SFS_CMD_FILE_CLOSE                 = 9,
  SW_SFS_CMD_FILE_DELETE                = 10,
  SW_SFS_CMD_FILE_ACCESS                = 11,
  SW_SFS_CMD_FILE_SEEK                  = 12,
  SW_SFS_CMD_TUNED_MODES                = 13,
  SW_SFS_PERSIST_CMD_FILE_CREATE        = 18,
  SW_SFS_PERSIST_CMD_FILE_OPEN          = 19,
  SW_SFS_PERSIST_CMD_FILE_CLOSE         = 20,
  SW_SFS_PERSIST_CMD_FILE_DELETE        = 21,
  SW_SFS_PERSIST_CMD_FILE_ACCESS        = 22,
  SW_SFS_PERSIST_CMD_FILE_SEEK          = 23,
  SW_SFS_CMD_CUSTOM_PARTITION_INFO      = 34,
  SW_SFWUG_CMD_ERASE                    = 35, 
  SW_SFWUG_CMD_WRITE                    = 36,
  SW_SFWUG_CMD_READ                     = 37,
  SW_SFWUG_CMD_CONFIG                   = 38,
  SW_SFWUG_CMD_STATUS                   = 39,
  SW_SFWUG_CMD_DATA                     = 40,
  SW_SFWUG_CMD_I2CS                     = 41,

} SwCmdCode;


/*
 * Most of the CmdCode should be use CMD_GET() or CMD_SET() in the code so not need dedicated micros for
 * *_CMD_GET_* or *_CMD_SET_* instead of needing using a more readable name.
 */
#define SW_SFS_CMD_FILE_READ            CMD_GET(SW_SFS_CMD_FILE_ACCESS)
#define SW_SFS_CMD_FILE_WRITE           CMD_SET(SW_SFS_CMD_FILE_ACCESS)
#define SW_SFS_PERSIST_CMD_FILE_READ    CMD_GET(SW_SFS_PERSIST_CMD_FILE_ACCESS)
#define SW_SFS_PERSIST_CMD_FILE_WRITE   CMD_SET(SW_SFS_PERSIST_CMD_FILE_ACCESS)

//------------------------------------------------------------------------------
//append from "control_ex_sub.h"
#ifndef AUDIODSP_CAF_INCLUDE_CONTROL_EX_H_
#define AUDIODSP_CAF_INCLUDE_CONTROL_EX_H_


#ifdef __cplusplus
extern "C"
{
#endif

#define ID_ARM        _ID('M','C','U',' ')
#define ID_MM         _ID('<','M','M','>')
#define ID_CTRL       _ID('C','T','R','L')
#define ID_CTRL_A     _ID('<','A','M','>')
#define ID_CTRL_B     _ID('<','B','M','>')
#define ID_HIFI       _ID('<','H','M','>')
#define ID_INDX_FILE  _ID('I','N','D','X')
#define ID_EOF        _ID('E','O','F','0')
#define ID_CAIO       _ID('<','A','W','>')  //core A IO
#define ID_CBIO       _ID('<','B','W','>')  //core B IO
#define ID_CODC       _ID('C','O','D','C')
typedef enum {
  CONTROL_MGR_RESET              =  1,
  CONTROL_MGR_VERSION            =  3,
  CONTROL_MGR_EXEC_FILE          =  4,
//CONTROL_MGR_EVENT_PARAM - Command data is an odd number of uint32_t.
// SET: The first parameter is a bit-field mask that indicates the events to trigger,
// followed by pairs of parameter ID and parameter value for parameterized scripts.
  CONTROL_MGR_EVENT_PARAM        = 13,
  CONTROL_MGR_MODE_ID            = 14,
  CONTROL_MGR_PARAMETER_VALUE    = 37,
  CONTROL_MGR_SCRIPT_DATA        = 84,
  // CONTROL_MGR_TUNED_MODES
  // GET: This command returns with a list of available tuned modes (modes from ext_scripts folder)
  //      This command has only one argument which is the index to start reading the modes from.
  //      e.g.: To start reading the modes, send this command with an input argument of 0
  //            If after reading 13 modes, the host can ask to read more from index 13, by
  //            sending this command again with the input argument of 13
  CONTROL_MGR_TUNED_MODES        = 85,
} ControlMgrCmdCode;

typedef enum {
  BASE_CONFIG_SCRIPT_ID     =  0,
  BASE_PLBK_PROFILE_ID      =  1,
  BASE_ANC_PROFILE_ID       =  2,
  BASE_PSAP_PROFILE_ID      =  3,
  BASE_NS_PROFILE_ID        =  4,
  MAX_NUM_SCRIPT_DATA       
} ControlMgrScriptData;
typedef enum {
  EVENT_PAR_ANC_STATE          = 27,
  EVENT_PAR_AF_STATE           = 28,
  EVENT_PAR_PRI_I2S_PCM_IDX    = 29, //Primary   I2S_PCM index will be used as codec clk source.
  EVENT_PAR_SEC_I2S_PCM_IDX    = 30, //Secondary I2S_PCM index.
  EVENT_PAR_ENC_STATE          = 31,
  PAR_INDEX_EVENT_ARG0         = 32
} ParProcessIndex;

typedef enum {
  EVENT_PAR_RATE_MAIN_INPUT             = PAR_INDEX_EVENT_ARG0, 
  EVENT_PAR_RATE_HOST_RECORD            = PAR_INDEX_EVENT_ARG0 + 6, //Used by Freeman3 on tahiti, careful when modify its value.
  EVENT_PAR_RATE_INDX_HOST_RECORD       = PAR_INDEX_EVENT_ARG0 + 8,
  EVENT_PAR_RATE_INDX_USB_HOST_RECORD   = PAR_INDEX_EVENT_ARG0 + 9,
  EVENT_PAR_RATE_INDX_USB_HOST_PLAYBACK = PAR_INDEX_EVENT_ARG0 + 10,
  EVENT_PAR_USB_RECORD_STATE            = PAR_INDEX_EVENT_ARG0 + 11,
  EVENT_PAR_VOL_USB_RECORD_CH0          = PAR_INDEX_EVENT_ARG0 + 12,
  EVENT_PAR_VOL_USB_RECORD_CH1          = PAR_INDEX_EVENT_ARG0 + 13,
  EVENT_PAR_LP_TRIG_STATE               = PAR_INDEX_EVENT_ARG0 + 16,
  EVENT_PAR_USB_PLAYBACK_STATE          = PAR_INDEX_EVENT_ARG0 + 18,
  EVENT_PAR_VOL_USB_PLAYBACK_CH0        = PAR_INDEX_EVENT_ARG0 + 19,
  EVENT_PAR_VOL_USB_PLAYBACK_CH1        = PAR_INDEX_EVENT_ARG0 + 20,
  EVENT_PAR_ANC_FF_CALIB_GAIN           = PAR_INDEX_EVENT_ARG0 + 21,
  EVENT_PAR_ANC_FB_CALIB_GAIN           = PAR_INDEX_EVENT_ARG0 + 22,
  EVENT_PAR_ANC_PLBK_CANC_CALIB_GAIN    = PAR_INDEX_EVENT_ARG0 + 23,
  EVENT_PAR_AMB_INCL_CALIB_GAIN         = PAR_INDEX_EVENT_ARG0 + 24,
  EVENT_PAR_PLBK_CALIB_GAIN             = PAR_INDEX_EVENT_ARG0 + 25,
  EVENT_PAR_PSAP_CALIB_GAIN             = PAR_INDEX_EVENT_ARG0 + 26,
  EVENT_PAR_EAR_ROLE                    = PAR_INDEX_EVENT_ARG0 + 27,
  EVENT_PAR_VOICE_PROMPT_STATE          = PAR_INDEX_EVENT_ARG0 + 30,
  EVENT_PAR_VOICE_PROMPT_FILE           = PAR_INDEX_EVENT_ARG0 + 31,
  EVENT_PAR_CUSTOM_0                    = PAR_INDEX_EVENT_ARG0 + 34,
  EVENT_PAR_CUSTOM_1                    = PAR_INDEX_EVENT_ARG0 + 35,
  EVENT_PAR_CUSTOM_2                    = PAR_INDEX_EVENT_ARG0 + 36,
  EVENT_PAR_CUSTOM_3                    = PAR_INDEX_EVENT_ARG0 + 37,
  EVENT_PAR_KEY_FUNC_STATE              = PAR_INDEX_EVENT_ARG0 + 38,
  EVENT_PAR_EXT_ECHO_REFERENCE          = PAR_INDEX_EVENT_ARG0 + 39,
  EVENT_PAR_TRIG_DETECTED_INDEX         = PAR_INDEX_EVENT_ARG0 + 40,
  EVENT_PAR_PRE_SET_USER_EQ_INDEX       = PAR_INDEX_EVENT_ARG0 + 55,

  EVENT_PAR_ANC_RIGHT_FF_CALIB_GAIN           = PAR_INDEX_EVENT_ARG0 + 75,
  EVENT_PAR_ANC_RIGHT_FB_CALIB_GAIN           = PAR_INDEX_EVENT_ARG0 + 76,
  EVENT_PAR_ANC_RIGHT_PLBK_CANC_CALIB_GAIN    = PAR_INDEX_EVENT_ARG0 + 77,
  EVENT_PAR_AMB_RIGHT_INCL_CALIB_GAIN         = PAR_INDEX_EVENT_ARG0 + 78,
  EVENT_PAR_PLBK_RIGHT_CALIB_GAIN             = PAR_INDEX_EVENT_ARG0 + 79,
  EVENT_PAR_PSAP_RIGHT_CALIB_GAIN             = PAR_INDEX_EVENT_ARG0 + 80,

} EventParIndex;
typedef enum {
  EVENT_SAMPLE_RATE_CHANGE              = 1 << 0,    //used by software caf 
  EVENT_USB_RECORD_STARTSTOP            = 1 << 1,
  EVENT_CADENCE_END                     = 1 << 2,
  EVENT_VOLUME_CHANGE_MUTE_USB_RECORD   = 1 << 3,
  EVENT_VOLUME_CHANGE_USB_RECORD        = 1 << 4,
  EVENT_USB_PLAYBACK_STARTSTOP          = 1 << 5,
  EVENT_FAST_EXEC_END                   = 1 << 6,
  EVENT_VOLUME_CHANGE_USB_PLAYBACK      = 1 << 7,
  EVENT_TRIG_DETECTED                   = 1 << 8,
  EVENT_CMND_DETECTED                   = 1 << 9,
  EVENT_LP_TRIG_DETECTED                = 1 << 10,
  EVENT_TRIG_VAD_HANGOVER               = 1 << 11,
  EVENT_AVAD_CONTROL                    = 1 << 12,
  EVENT_CUSTOM_0                        = 1 << 13,
  EVENT_CUSTOM_1                        = 1 << 14,
  EVENT_CUSTOM_2                        = 1 << 15,
  EVENT_CUSTOM_3                        = 1 << 16,
  EVENT_UART_TX_RECD_STARTSTOP          = 1 << 17,
  EVENT_SPIM_TX_RECD_STARTSTOP          = 1 << 18,
  EVENT_ANC_CONTROL                     = 1 << 19,
  EVENT_ENC_CONTROL                     = 1 << 20,
  EVENT_VOICE_PROMPT_CONTROL            = 1 << 21,
  EVENT_AF_CONTROL                      = 1 << 22,
  EVENT_LP_TRIG_CONTROL                 = 1 << 23,
  EVENT_KEY_DETECTED                    = 1 << 24,
  EVENT_UART_RX_PLBK_STARTSTOP          = 1 << 25,

  EVENT_HPM_CONTROL                     = 1 << 27,
} EventId;

#ifdef __cplusplus
}
#endif

#endif  // AUDIODSP_CAF_INCLUDE_CONTROL_EX_H_

//------------------------------------------------------------------------------
//append from "eipc_drv.h"
enum EIPC_COMMAND_ID
{
  EIPC_FLUSH = 0,
  EIPC_SET_THRESHOLD = 1,
  EIPC_MESSAGE_SIZE = 2,
  EIPC_REASON = 3,
  EIPC_TEST = 4,
};

//------------------------------------------------------------------------------
//append from "keypad_drv.h"
typedef enum
{
    GPIO_0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    BTN0_ID,
    BTN1_ID,
    BTN2_ID,
    BTN3_ID,

    KEY_ID_MAX = 14,
    VOICE_ASSIST_ID = 14,   //No need to poll VOICE_ASSIST since it is a virtual KEY for system.
} eKEY_ID;

typedef enum
{
    KEY_NO_EVENT,
    KEY_SHORT_PRESS_EVENT,
    KEY_LONG_PRESS_EVENT,
    KEY_DOUBLE_PRESS_EVENT,
    KEY_PRESSED_EVENT,
    KEY_RELEASE_EVENT,
    KEY_TRIPLE_PRESS_EVENT,
    KEY_GPIO_IRQ,
} eKEY_EVENT;

#endif /*AudioSmart_DEFINITIONS_H*/
