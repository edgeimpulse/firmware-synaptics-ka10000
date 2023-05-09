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

#ifndef _TAHITI_CMDS_H_
#define _TAHITI_CMDS_H_



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
#define APP_MID_GOVERN      'G'
#define APP_MID_FORWARD     '>'
#define APP_MID_BROADCAST   '*'
#define APP_MID_ARM         'M'
#define APP_MID_CAPE_A      'A'
#define APP_MID_CAPE_B      'B'
#define APP_MID_HIFI        'H'
#define APP_MID_SLV_ARM     'S'
#define APP_MID_SLV_CAPE_A  'X'
#define APP_MID_SLV_CAPE_B  'Y'
#define APP_MID_SLV_HIFI    'Z'

#endif //_CAPE2_CMDS_H_
