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

  File Name: AudioSmart_sendcmd_i2c.c

------------------------------------------------------------------------------*/
#include "AudioSmart_definitions.h"
#include "AudioSmart_sendcmd.h"
#include "AudioSmart_i2c_interface_api.h"
#include <stdio.h>

#if defined CONFIG_TARGET_OS_ANDROID
  #define LOG_TAG "AudioSmart_sendcmd_i2c"
  #define LOG_NDEBUG 0  //0 turns debugging on!
#endif

#define BADBEFF                   0X8BADD00D
#define READY_POOLING_INTERVAL    10 /*10 ms*/
#define READY_POOLING_LOOP        (6000/READY_POOLING_INTERVAL) /*6 s*/
#define REPLY_POLL_INTERVAL_MSEC  1
#define REPLY_POLL_TIMEOUT_MSEC   5000
//#define CONFIG_MCU_I3C0_SLAVE_ADDRESS    (1)
typedef void (*typedef_delay_fn_ptr)(uint32_t);  
typedef_delay_fn_ptr g_delay_fn_ptr = NULL; /*global delay function pointer*/

int32_t AudioSmart_mid_level_init(void (*delay_fn_ptr)(uint32_t delay_in_ms))
{
  if (delay_fn_ptr)
  {
    g_delay_fn_ptr = delay_fn_ptr;
    return SENDCMD_RC_SUCCESS;
  }
  else
  {
    return SENDCMD_RC_INVALID_ARG_NUM;
  }
}

int32_t AudioSmart_mid_sendcmd(uint32_t *cmd)
{
  Command *cmd_ptr = (Command *)cmd;
  uint32_t *i2c_data = (uint32_t *)cmd;
  int32_t size;

  if (g_delay_fn_ptr == NULL)
  {
    return SENDCMD_RC_INVALID_ARG_NUM;
  }

  cmd_ptr->head.num_32b_words = (cmd_ptr->head.command_id & CMD_GET(0)) ? MAX_COMMAND_SIZE : cmd_ptr->head.num_32b_words;
  size = cmd_ptr->head.num_32b_words + 2;

#ifdef CONFIG_MCU_I3C0_SLAVE_ADDRESS
    // write word 0 to address 0x0
    AudioSmart_i2c_interface_write (0x0, &i2c_data[0], 1); //to wake up I3C slave IP 
    (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC); //wait more interrupt to reduce interrupt frequency
#endif
  // write words 1 to N-1 , to addresses 4 to 4+4*N-1
  AudioSmart_i2c_interface_write(0x4, &i2c_data[1], size - 1);

  // write word 0 to address 0
  AudioSmart_i2c_interface_write(0x0, &i2c_data[0], 1);

  int32_t elapsed_ms = 0;

  while (elapsed_ms < REPLY_POLL_TIMEOUT_MSEC)
  {

#ifdef CONFIG_MCU_I3C0_SLAVE_ADDRESS
      // write word 0 to address 0xc
      AudioSmart_i2c_interface_write (0xc, &i2c_data[0], 1);
      (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC); //wait more interrupt to reduce interrupt frequency
#endif    
    // only read the first word and check the reply bit
    AudioSmart_i2c_interface_read(0x0, &i2c_data[0], 1);

    if (cmd_ptr->head.reply == 1)
      break;
    (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
    //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);
    elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
  }

  if (cmd_ptr->head.reply == 1)
  {
    if (cmd_ptr->head.num_32b_words > 0)
    {
#ifdef CONFIG_MCU_I3C0_SLAVE_ADDRESS
      AudioSmart_i2c_interface_write (0xf, &i2c_data[0], 1);
      (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC); 
#endif
      AudioSmart_i2c_interface_read(0x8, &i2c_data[2], cmd_ptr->head.num_32b_words);
    }
    if (cmd_ptr->head.num_32b_words >= 0)
      return SENDCMD_RC_SUCCESS;
    else
      return SENDCMD_RC_CMD_ERROR;
  }
  return SENDCMD_RC_REPLY_TIMEOUT;
}

int32_t AudioSmart_mid_sendcmd_async(uint32_t *cmd, uint32_t wait_timeout, sendcmd_cb cb_func){
  //these interfaces don't support non-blocking API, so ignore these parameters  
  return AudioSmart_mid_sendcmd(cmd);
}

int32_t AudioSmart_i2c_wait_until_bootup(void (*delay_fn_ptr)(uint32_t delay_in_ms))
{
  int32_t check_loop = READY_POOLING_LOOP;
  int32_t i;
  uint32_t val = 0;
  int32_t err_no = CXERR_NOERR;

  for (i = 0; i < check_loop; i++)
  {
    (*delay_fn_ptr)(READY_POOLING_INTERVAL);
    err_no = AudioSmart_i2c_interface_read(4, &val, 1);
    if (BADBEFF == val)
    {
      err_no = CXERR_NOERR;
      break;
    }
  }
  if (BADBEFF != val)
  {
    logError("\nERROR: Failed to reboot!\n");
    err_no = -CXERR_FW_CANNOT_BOOT;
  }
  return err_no;
}
