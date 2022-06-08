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

  File Name: AudioSmart_sendcmd_usb.c

------------------------------------------------------------------------------*/
#include "AudioSmart_definitions.h"
#include "AudioSmart_sendcmd.h"
#include "AudioSmart_usb_interface_api.h"

#define REPLY_POLL_INTERVAL_MSEC  1
#define REPLY_POLL_TIMEOUT_MSEC   5000

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

int32_t AudioSmart_mid_sendcmd(uint32_t *cmd_ptr)
{
  Command *cmd = (Command *)cmd_ptr;
  int32_t err = SENDCMD_RC_INVALID_ARG_NUM;

  int32_t elapsed_ms = 0;

  if (g_delay_fn_ptr == NULL)
  {
    return SENDCMD_RC_INVALID_ARG_NUM;
  }

  int32_t num_32b_words = cmd->head.num_32b_words;
  cmd->head.num_32b_words = (cmd->head.command_id & CMD_GET(0)) ? MAX_COMMAND_SIZE : num_32b_words;

  while ((elapsed_ms < REPLY_POLL_TIMEOUT_MSEC) && (err < 0)) //if there is an error with sending to USB retry every 1ms up to 2 seconds before giving up.
  {
    if ((err = AudioSmart_usb_interface_write((unsigned int *)cmd, sizeof(Command))) < 0)
    {
      (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
      //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);
      elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
    }
  }

  if (err < 0) // if error after 2000 tries give up.
    return (SENDCMD_RC_FAILED_SEND);

  elapsed_ms = 0;

  while (elapsed_ms < REPLY_POLL_TIMEOUT_MSEC)
  {
    // only read the first word and check the reply bit
    if ((err = AudioSmart_usb_interface_read((uint32_t *)cmd, sizeof(Command))) < 0)
      return (SENDCMD_RC_FAILED_SEND);

    if (cmd->head.reply == 1)
    {
      if (cmd->head.num_32b_words >= 0)
        return (SENDCMD_RC_SUCCESS);
      else
        return (SENDCMD_RC_CMD_ERROR);
    }

    (*g_delay_fn_ptr)(REPLY_POLL_INTERVAL_MSEC);
    //sys_mdelay(REPLY_POLL_INTERVAL_MSEC);

    elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
  }
  return (SENDCMD_RC_REPLY_TIMEOUT);
}

int32_t AudioSmart_mid_sendcmd_async(uint32_t *cmd, uint32_t wait_timeout, sendcmd_cb cb_func){
  //these interfaces don't support non-blocking API, so ignore these parameters  
  return AudioSmart_mid_sendcmd(cmd);
}
