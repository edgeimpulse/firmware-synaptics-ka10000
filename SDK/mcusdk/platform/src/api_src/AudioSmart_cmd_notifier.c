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
#define PRINT_LEVEL_SET ASSERT_LEVEL_ERR
#include "AudioSmart_cmd_notifier.h"
#include "AudioSmart_definitions.h"

#include "print.h"
#include "mbox_drv.h"

#if (!defined(SIMULATION)) || (defined(SIMULATION) &&  defined(SIM_MULTIPLE_CORE))

#ifdef CONFIG_CMD_NOTIFIER_ENABLE
CmdNotifierState g_cmd_notifier_task;


void CmdNotifierPushItem(uint32_t *p_void_cmd, uint32_t wait_timeout, sendcmd_cb cb_func) {
  uint32_t cur_idx = g_cmd_notifier_task.p_cur_idx;
  CmdNotifierQueueItem *p_item = &g_cmd_notifier_task.item_array[cur_idx];
  p_item->p_cmd = p_void_cmd;
  p_item->timeout_ms = wait_timeout;
  p_item->p_cmd_callback = cb_func;
  g_cmd_notifier_task.p_cur_idx = (cur_idx + 1) % CONFIG_CMD_NOTIFIER_QUEUE_SIZE;

  if(g_cmd_notifier_task.queue_handle != NULL){
      Print_DBG("g_cmd_notifier_task.queue_handle != NULL, cur_idx = %d\n", cur_idx);
      xQueueSendToBack(g_cmd_notifier_task.queue_handle, (unsigned long*)&p_item, portMAX_DELAY);
  } else {
     Print_ERR("g_cmd_notifier_task.queue_handle = NULL, cur_idx = %d\n", cur_idx);
  }


  if (g_cmd_notifier_task.run&CMD_NOTIFIER_IS_RESET_MODE) {
    g_cmd_notifier_task.run &= (~CMD_NOTIFIER_IS_RESET_MODE);
  }
}


int CmdNotifierSetResetMode() {
  g_cmd_notifier_task.run |= CMD_NOTIFIER_IS_RESET_MODE;
  return 0;
}

int CmdNotifierWaitUntilReady() {
  CmdNotifierQueueItem *io_item = NULL;
  CmdNotifierQueueItem *p_io_item = (CmdNotifierQueueItem *)&io_item;

  //do nothing just clear io_queue,  
  Print_DBG("CmdNotifier wait start\n");
  while (xQueuePeek(g_cmd_notifier_task.queue_handle, (size_t *)&p_io_item, 0) != errQUEUE_EMPTY)
  //while(g_cmd_notifier_task.run & CMD_NOTIFIER_IS_RESET_MODE)
  {
    Print_DBG("CmdNotifier wait loop\n");
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  Print_DBG("CmdNotifier wait done\n");
  return 0;
}

int CmdNotifierResetAll() {
  int                     ret = 0;
  CmdNotifierQueueItem *io_item = NULL;
  CmdNotifierQueueItem *p_io_item = (CmdNotifierQueueItem *)&io_item;

  //do nothing just clear io_queue,  
  while (xQueueReceive(g_cmd_notifier_task.queue_handle, (size_t *)&p_io_item, 0) == pdPASS)
  {

  }
  return ret;
}

void CmdNotifierRunning() {
  CmdNotifierQueueItem *io_item = NULL;
  CmdNotifierQueueItem *p_io_item = (CmdNotifierQueueItem *)&io_item;
  Command *p_cmd;
  uint32_t i, item_cnt;

  Print_DBG("CmdNotifierRunning\n");

  while (g_cmd_notifier_task.run) {


    while (xQueueReceive(g_cmd_notifier_task.queue_handle, (size_t *)&p_io_item, \
      portMAX_DELAY) == pdPASS) {
      //has command in the queue, so check each one's status
      item_cnt = CONFIG_CMD_NOTIFIER_QUEUE_SIZE - uxQueueSpacesAvailable(g_cmd_notifier_task.queue_handle) + 1;
      i = 0;
      for (i = 0; i < item_cnt; i++) {
        p_cmd = (Command *)p_io_item->p_cmd;
        if (p_cmd->head.reply || p_io_item->timeout_ms <= 0) {
          p_io_item->p_cmd_callback(p_cmd);
        }
        else {
          p_io_item->timeout_ms -= CONFIG_CMD_NOTIFIER_POLLING_INTERVAL;
          xQueueSendToBack(g_cmd_notifier_task.queue_handle, (unsigned long*)&p_io_item, portMAX_DELAY);
        }

        if (i < (item_cnt-1)) {
          //can not receive again when checking last item
          xQueueReceive(g_cmd_notifier_task.queue_handle, (size_t *)&p_io_item, 0);
        }

      } 
      //already loop through the queue, so delay a interval before next polling round
      vTaskDelay(pdMS_TO_TICKS(CONFIG_CMD_NOTIFIER_POLLING_INTERVAL));

      //check if reset mode, 
      if (g_cmd_notifier_task.run & CMD_NOTIFIER_IS_RESET_MODE) {
        Print_DBG("CmdNotifier reset start\n");
        CmdNotifierResetAll();
        g_cmd_notifier_task.run &= (~CMD_NOTIFIER_IS_RESET_MODE);
        Print_DBG("CmdNotifier reset done\n");
      }
    }

  }// end of cmd_notifier is running
}

int CmdNotifierInit()
{
  g_cmd_notifier_task.queue_handle = \
    xQueueCreate(CONFIG_CMD_NOTIFIER_QUEUE_SIZE, sizeof(CmdNotifierQueueItem *));
  if(g_cmd_notifier_task.queue_handle == NULL){
    return -1;
  } else {
    return 0;
  }

}

void CmdNotifierStart(void * arg)
{

  int ret_val = 0;
  g_cmd_notifier_task.run = 1;
  g_cmd_notifier_task.p_cur_idx = 0;

  if (CmdNotifierInit() != 0)
  {
    Print_ERR("CmdNotifierInit fails\n");
  }

  CmdNotifierRunning();
  return;
}

#if defined(__arm__)
long cmd_notifier_hook(void *pvParam )
{
  os_TaskHookMsg* EventMsg = (os_TaskHookMsg*) pvParam;

  switch(EventMsg->msg)
  {
  }
  
  return 0;
}

//For m33' case, use OSAL to create because raw FreeRTOS does not work
int CmdNotifierCreate(void)
{
  os_TaskInit	cmdNotifierInit;
  memset(&cmdNotifierInit, 0, sizeof(os_TaskInit));
  memset(&g_cmd_notifier_task, 0, sizeof(CmdNotifierState));

  cmdNotifierInit.ContextID = _ID('C','M','D','N');  
  cmdNotifierInit.TaskFunc = CmdNotifierStart; 
  cmdNotifierInit.TaskHookFunc = cmd_notifier_hook;
  cmdNotifierInit.TaskName = "cmd_notifier";
  cmdNotifierInit.TaskStackSize = 128U;
  cmdNotifierInit.TaskPriority = OS_TASK_MID_PRIORITY;
  cmdNotifierInit.TaskTZModule = 0;

  g_cmd_notifier_task.task_handle = os_TaskCreate(&cmdNotifierInit);
  if (g_cmd_notifier_task.task_handle == 0)
  {
    Print_ERR("CmdNotifierCreate failed\n ");
  } else {
    Print_DBG("CmdNotifierCreate successfully\n ");
  }

  return 0;
}

#else

int CmdNotifierCreate()
{
  int err = 0;
  UBaseType_t uxPriority = CONFIG_CMD_NOTIFIER_PRIORITY;

  err = xTaskCreate(CmdNotifierStart, "cmdn", CONFIG_CMD_NOTIFIER_TASK_STK_SIZE, \
    NULL, \
    uxPriority, &g_cmd_notifier_task.task_handle);
  if (err != pdPASS)
  {
    Print_DBG("CmdNotifierStart Create fail\n ");
  }

  return 0;
}




#endif

#endif   //end of #ifdef CONFIG_CMD_NOTIFIER_ENABLE

#endif
