#include "syna_definitions.h"
////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS, INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Systems Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Synaptics that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, SYNAPTICS
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2020 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////



#ifndef _MCU_EX_H_
#define _MCU_EX_H_

_start_expose_ext_ 
typedef enum {
  FWID_SYS = 0,   //Target to SysCmdCode which in mcu_ex.h
_end_expose_ext_  
  FWID_HW  = 1,   //Target to HWCmdCode  which in current file
_start_expose_ext_ 
  FWID_SW  = 2,   //Target to SwCmdCode  which in mcu_ex.h
  FWID_EXT = 3,   //Target to SDK Extension commands  which in mcu_ex.h
} FwModuleId;

/*
 * Sys related Commands Codes which not suitable to group to HW/SW.
 */
typedef enum {
_end_expose_ext_ 
  SYS_CMD_NONE                          = 0,
_start_expose_ext_
  SYS_CMD_VERSION                       = 1,
_end_expose_ext_
  SYS_CMD_ACCESS_AHB                    = 2,
_start_expose_ext_
  SYS_CMD_SOFTRESET                     = 3,
_end_expose_ext_

  SYS_CMD_WAIT_TASK_RUNNING             = 4,
  SYS_CMD_OS_TASK_SLEEP                 = 5,
  SYS_CMD_OS_TASK_NOTIFY                = 6,
  SYS_CMD_OS_TASK_NOTIFY_EVENT          = 7,
_start_expose_ext_
  SYS_CMD_CONF_ERR_INFO                 = 8,
  SYS_CMD_EXEC_FILE                     = 9,
_end_expose_ext_
  SYS_CMD_EXEC_IF_FILE_EXISTS           = 10,
  SYS_CMD_CHAIN_COMMANDS                = 11,
  SYS_CMD_SEND_MULTIPLE_CMDS            = 12,

  SYS_CMD_PARAMETER_EVAL                = 13,
_start_expose_ext_ 
  SYS_CMD_PARAMETER_VALUE               = 14,

  SYS_CMD_EVENT_PARAM                   = 15,
_end_expose_ext_   
  SYS_CMD_EVENT_HANDLER                 = 16,
  SYS_CMD_EVENT_HANDLER_ENABLE          = 17,
  SYS_CMD_OS_TIME                       = 18, 
_start_expose_ext_
  SYS_CMD_MODE_ID                       = 19,
  SYS_CMD_SCRIPT_DATA                   = 20,
_end_expose_ext_
  SYS_CMD_CPU_STATE                     = 21,
  SYS_CMD_PRINT_SWITCH_S_2_NS           = 22,
  SYS_CMD_SYS_CORE_CLK                  = 23, 
  SYS_CMD_LAST_FILE_ERR_INFO            = 24, 
  MCU_MEM_STATUS                        = 25,
_start_expose_ext_
  SYS_CMD_BL_VERSION                    = 26,
_end_expose_ext_
  SYS_CMD_VID_PID                       = 27,
_start_expose_ext_
  SYS_CMD_CURRENT_SYS_PARTITION         = 28,
_end_expose_ext_
  SYS_CMD_PM_CMD                        = 29,
  SYS_CMD_PM_STATE_TABLE                = 30,
  SYS_CMD_PM_STATE_CHG_NOTIFY           = 31,
  SYS_CMD_OS_MEM_USAGE                  = 32,
  SYS_CMD_HANDLER_NOTIFY_PM             = 33,
  SYS_CMD_PM_SYNC_LUT                   = 34,
  SYS_CMD_ENTER_AVAD_STATE              = 35,
  SYS_CMD_MUTE_UART_DEBUG               = 36,
_start_expose_ext_ 
  SYS_CMD_IPC_CMD                       = 37,
  SYS_CMD_IPC_MSG                       = 38,
  SYS_CMD_EVENT_MASK                    = 39,
_end_expose_ext_
  SYS_CMD_PM_AUTO_MODE_EXCEPTION        = 40,
  SYS_CMD_JTAG_SELECTION                = 41,
  SYS_CMD_SYS_RETURN_TO_DEFAULT         = 42,
  SYS_CMD_SVN_VERSION                   = 43,
  SYS_CMD_MEM_BULK_ACS_CTL              = 44,
  SYS_CMD_MEM_BULK_ACS_DAT              = 45,
  SYS_CMD_GTK_GPIO_TEST                 = 46,

_start_expose_ext_ 
  SYS_CMD_SIDETONE_ENABLE               = 47,
  SYS_CMD_SIDETONE_VOLUME               = 48,
_end_expose_ext_
  SYS_CMD_SLT_NOTIFY                    = 49,
  
  SYS_CMD_GPIO_RESET_CONFIG             = 50,
  SYS_CMD_GPIO_RESET                    = 51,

  SYS_CMD_ENCODER_CONFIG                = 52,
  SYS_CMD_CONFIG_GPIO_IRQ               = 53,

_start_expose_ext_ 
  SYS_CMD_START_BG_UPGRADE              = 54,
_end_expose_ext_

//------------------- Katana Group ------------------------
  SYS_CMD_CAMERA_DEV_RESET              = 128,
  SYS_CMD_CAMERA_DEV_CTRL               = 129,
  SYS_CMD_CAMERA_DEV_READ               = 130,
  SYS_CMD_CAMERA_DEV_WRITE              = 131,

  SYS_CMD_PM_CLK                        = 132,
  SYS_CMD_PM_LPM                        = 133,
  
  SYS_CMD_BT_SEND                       = 134,
  SYS_CMD_BT_FACTORY                    = 135

_start_expose_ext_
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
_end_expose_ext_
  SW_SFS_CMD_FILE_CREATE                = 7,
  SW_SFS_CMD_FILE_OPEN                  = 8,
  SW_SFS_CMD_FILE_CLOSE                 = 9,
  SW_SFS_CMD_FILE_DELETE                = 10,
  SW_SFS_CMD_FILE_ACCESS                = 11,
  SW_SFS_CMD_FILE_SEEK                  = 12,
_start_expose_ext_
  SW_SFS_CMD_TUNED_MODES                = 13,
_end_expose_ext_
  SW_CADENCE_CMD_STOP                   = 14,
  SW_CADENCE_CMD_START                  = 15,
  SW_CADENCE_CMD_CONFIG                 = 16,

  SW_SPIAUDIO_CMD                       = 17,
_start_expose_ext_
  SW_SFS_PERSIST_CMD_FILE_CREATE        = 18,
  SW_SFS_PERSIST_CMD_FILE_OPEN          = 19,
  SW_SFS_PERSIST_CMD_FILE_CLOSE         = 20,
  SW_SFS_PERSIST_CMD_FILE_DELETE        = 21,
  SW_SFS_PERSIST_CMD_FILE_ACCESS        = 22,
  SW_SFS_PERSIST_CMD_FILE_SEEK          = 23,
_end_expose_ext_
  SW_UARTAUDIO_CMD                      = 25,
  SW_IPC_CMD                            = 26,

  SW_KEYPAD_CMD_CONF_BTN_SETTINGS       = 27,
  SW_KEYPAD_CMD_CONF_BTN_STATES         = 28,

  SW_SWPWM_CMD_CONF_GP                  = 29,
  SW_SWPWM_CMD_SET_COLOR                = 30,
  SW_KEYPAD_CMD_ENABLE_BTN              = 31,
  
  SW_UARTAUDIO_CFG                      = 32,
  SW_UARTAUDIO_ENABLE                   = 33,
_start_expose_ext_
  SW_SFS_CMD_CUSTOM_PARTITION_INFO      = 34,

  SW_SFWUG_CMD_ERASE                    = 35, 
  SW_SFWUG_CMD_WRITE                    = 36,
  SW_SFWUG_CMD_READ                     = 37,
  SW_SFWUG_CMD_CONFIG                   = 38,
  SW_SFWUG_CMD_STATUS                   = 39,
  SW_SFWUG_CMD_DATA                     = 40,
  SW_SFWUG_CMD_I2CS                     = 41,
  

} SwCmdCode;
_end_expose_ext_
/*
 * Most of the CmdCode should be use CMD_GET() or CMD_SET() in the code so not need dedicated micros for
 * *_CMD_GET_* or *_CMD_SET_* instead of needing using a more readable name.
 */
#define SW_SFS_CMD_FILE_READ            CMD_GET(SW_SFS_CMD_FILE_ACCESS)
#define SW_SFS_CMD_FILE_WRITE           CMD_SET(SW_SFS_CMD_FILE_ACCESS)
#define SW_SFS_PERSIST_CMD_FILE_READ    CMD_GET(SW_SFS_PERSIST_CMD_FILE_ACCESS)
#define SW_SFS_PERSIST_CMD_FILE_WRITE   CMD_SET(SW_SFS_PERSIST_CMD_FILE_ACCESS)

#endif  //_MCU_EX_H_
