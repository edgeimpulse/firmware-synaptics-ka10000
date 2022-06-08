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
//  Copyright (C) 2018 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_CMDS_H_
#define _MCU_CMDS_H_

typedef enum {
  CMD_MSG_INDEX  = 3,
  I2CS_MSG_INDEX = 4,
  I2CS_MSG_FWUG  = 5, //extend the command buffer size to 2*sizeof(command)
  NUM_OF_CMD_MSG_BUFS=6
} COMMAND_BUFFER_INDEX;

#include "mcu_ex.h"
#include "hw_cmds_ex.h"

// Signal code for sending a command using sos_signal()
#define SIG_CMD  1

#define INT_SIZE_OF(objtype,int_type)   (((sizeof(objtype)-1)/sizeof(int_type))+1)
#define INT32_SIZE_OF(obj_type)         INT_SIZE_OF(obj_type, int32_t)

#define CMD_GetAppIDx(x,o)  (((x>>o)+0x20)&0x0000003f)
#define CMD_GetAppNm(x)     ((x<0x20) ? (x|0x40) : x)
#define CMD_AppID1(x)       CMD_GetAppNm(CMD_GetAppIDx(x, 8))
#define CMD_AppID2(x)       CMD_GetAppNm(CMD_GetAppIDx(x, 14))
#define CMD_AppID3(x)       CMD_GetAppNm(CMD_GetAppIDx(x, 20))
#define CMD_AppID4(x)       CMD_GetAppNm(CMD_GetAppIDx(x, 26))
#define CMD_MdlID(x)        (x&0x000000ff)
#define APP_MID_CTRL        'M'
#define APP_MID_FORWARD     '>'
#define APP_MID_BROADCAST   '*'

#ifdef CONFIG_MCU_CTRL_ELK_TUNNELING
#define TAHITI_MODE     earlink_tunneling_get_mode()
#define APP_MID_ARM_LOC(x)          ((x==ELK_MST_MODE) ? 'M' : 'S')
#define APP_MID_CAPE_A_LOC(x)       ((x==ELK_MST_MODE) ? 'A' : 'X')
#define APP_MID_CAPE_B_LOC(x)       ((x==ELK_MST_MODE) ? 'B' : 'Y')
#define APP_MID_HIFI_LOC(x)         ((x==ELK_MST_MODE) ? 'H' : 'Z')
#define APP_MID_SLV_ARM         'S'
#define APP_MID_SLV_CAPE_A      'X'
#define APP_MID_SLV_CAPE_B      'Y'
#define APP_MID_SLV_HIFI        'Z'
#define APP_MID_ARM             APP_MID_ARM_LOC(TAHITI_MODE)
#define APP_MID_CAPE_A          APP_MID_CAPE_A_LOC(TAHITI_MODE)
#define APP_MID_CAPE_B          APP_MID_CAPE_B_LOC(TAHITI_MODE)
#define APP_MID_HIFI            APP_MID_HIFI_LOC(TAHITI_MODE)
#define APP_MID_SELF            APP_MID_ARM_LOC(TAHITI_MODE)
#define CMD_Convert_TargetID(x,o) ((x&(~0xFC000)) | (o-0x20)<<14)  
#else
#define APP_MID_ARM             'M'
#define APP_MID_CAPE_A          'A'
#define APP_MID_CAPE_B          'B'
#define APP_MID_HIFI            'H'
#define APP_MID_SELF            APP_MID_ARM
#endif

#define CMD_FWD_NEXT_MBOX       DEV_MBOX_CAPE_A

#define IO_CMD_NAPLES_GAINS     0xC7

#endif //_MCU_CMDS_H_
