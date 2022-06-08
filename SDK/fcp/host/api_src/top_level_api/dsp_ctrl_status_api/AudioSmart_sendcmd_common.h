/*------------------------------------------------------------------------------
  Copyright (C) 2018 Synaptics Inc.
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

  File Name: AudioSmart_sendcmd_common.h

------------------------------------------------------------------------------*/
#ifndef AudioSmart_SENDCMD_COMMON_H
#define AudioSmart_SENDCMD_COMMON_H

#include "AudioSmart_sendcmd.h" 
#include "AudioSmart_definitions.h"


#ifdef __cplusplus
extern "C"
{
#endif


#define CONFIG_DEFAULT_WAIT_TIMEOUT 120000 //2S



#define SENDCMD(cmd, mgr_module_id, command_id, ...) \
        AudioSmart_sendcmd(cmd, (uint32_t)(mgr_module_id), (uint32_t)(command_id), NARGS(__VA_ARGS__), ##__VA_ARGS__)

#define SENDCMD_SYNC(cmd) \
         AudioSmart_sendcmd_sync(cmd)    

#define SENDCMD_ASYNC(cmd, cb_func) \
         AudioSmart_sendcmd_async(cmd, (sendcmd_cb)(cb_func))   


int32_t AudioSmart_sendcmd(Command *cmd, uint32_t mgr_module_id, uint32_t command_id, uint32_t num_32b_words, ...);
int32_t AudioSmart_sendcmd_sync(Command *cmd);
//NOTE: For this API, cmd pointer should be alive until cb_func is called
int32_t AudioSmart_sendcmd_async(Command *cmd,  sendcmd_cb cb_func);


#ifdef __cplusplus
}
#endif

#endif
