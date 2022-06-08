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

  File Name: AudioSmart_sendcmd_mbox.c

------------------------------------------------------------------------------*/

#include "AudioSmart_cmd_notifier.h"
#include "AudioSmart_sendcmd_common.h" //for CONFIG_DEFAULT_WAIT_TIMEOUT

#include "print.h"
#include "mbox_drv.h"

#include "FreeRTOS.h"
#include "task.h"

#if (!defined(SIMULATION)) || (defined(SIMULATION) &&  defined(SIM_MULTIPLE_CORE))

int32_t AudioSmart_mid_level_init(void(*delay_fn_ptr)(uint32_t delay_in_ms))
{
  //don't follow existing implementation since we have unified delay API
#ifdef CONFIG_CMD_NOTIFIER_ENABLE
  CmdNotifierCreate();
#else

#endif
  return 0;
}





int32_t AudioSmart_mid_sendcmd(uint32_t *p_void_cmd) {
  return AudioSmart_mid_sendcmd_async(p_void_cmd, CONFIG_DEFAULT_WAIT_TIMEOUT, NULL);
}


int32_t AudioSmart_mid_sendcmd_async(uint32_t *p_void_cmd, uint32_t wait_timeout, sendcmd_cb cb_func)
{
  Command *p_cmd = (Command *)p_void_cmd;
  uint32_t dev_name = mbox_map_id2num(&(p_cmd->head.mgr_module_id));
  int32_t ret = 0;
  size_t msg = (size_t)p_cmd;

  if (mbox_write(dev_name, (void *)&msg, sizeof(size_t) / sizeof(int)) < 0)
  {
    ret = -1;
  }

 
  if (cb_func==NULL&& wait_timeout>0)
  {
    //Blocking SENDCMD
    Command volatile *cmd_volatile = p_cmd;

    //wait for reply for M0P
    uint32_t jiffies = 0;
    do
    {
      vTaskDelay(1);
      jiffies++;
    } while ((0 == cmd_volatile->head.reply) && (jiffies < wait_timeout));

    if (jiffies == wait_timeout)
    {
      Print_ERR("%s():timeout\n", __func__);
    }
  }
#ifdef CONFIG_CMD_NOTIFIER_ENABLE
  else if(cb_func!=NULL){
    //Non-blocking SENDCMD
    CmdNotifierPushItem(p_void_cmd, wait_timeout, cb_func);
  }
#endif

  return 0;

}
#else
//always build AudioSmart_sendcmd_common.c, which depends on this function
int32_t AudioSmart_mid_sendcmd(uint32_t *p_void_cmd)
{
    return 0;
} 
int32_t AudioSmart_mid_sendcmd_async(uint32_t *p_void_cmd, uint32_t wait_timeout, sendcmd_cb cb_func)
{
  return 0;
}

#endif

