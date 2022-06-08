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

  File Name: audp.h

  Description: Define struct and APIs exposed to other objects 
               
------------------------------------------------------------------------------*/
#ifndef AudioSmart_CMD_NOTIFIER_H_
#define AudioSmart_CMD_NOTIFIER_H_

#include "autoconf.h"

#ifdef CONFIG_CMD_NOTIFIER_ENABLE

#define CONFIG_CMD_NOTIFIER_QUEUE_SIZE 10
#define CONFIG_CMD_NOTIFIER_POLLING_INTERVAL 2
#define CONFIG_CMD_NOTIFIER_PRIORITY 5
#define CONFIG_CMD_NOTIFIER_TASK_STK_SIZE 0x1000

#include <stdio.h>
#include "AudioSmart_sendcmd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define CMD_NOTIFIER_IS_RESET_MODE   (1<<1)  //in reset_mode, just clear queue


  typedef struct {

    uint32_t    *p_cmd;
    int32_t   timeout_ms;
    sendcmd_cb p_cmd_callback;

  } CmdNotifierQueueItem;


  typedef struct CmdNotifierStateTag {

    volatile int         run;
 
    QueueHandle_t        queue_handle;
    TaskHandle_t         task_handle;

    CmdNotifierQueueItem item_array[CONFIG_CMD_NOTIFIER_QUEUE_SIZE];  //items in io_queue
    uint32_t             p_cur_idx;

  } CmdNotifierState;

  extern CmdNotifierState  g_cmd_notifier_task;
  int CmdNotifierCreate();
  void CmdNotifierPushItem(uint32_t *p_void_cmd, uint32_t wait_timeout, sendcmd_cb cb_func);
  int CmdNotifierSetResetMode();
  int CmdNotifierWaitUntilReady();

#ifdef __cplusplus
}
#endif

#endif  //CONFIG_CMD_NOTIFIER_ENABLE
#endif  // AudioSmart_CMD_NOTIFIER_H_
