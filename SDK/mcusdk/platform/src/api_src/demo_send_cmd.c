/*------------------------------------------------------------------------------
  Unpublished Work Copyright � 2018-2019 Synaptics Incorporated.
  All rightsreserved.

  This file contains information that is proprietary to Synaptics Incorporated
  (�Synaptics�). The holder of this file shall treat all information contained
  herein as confidential, shall use the information only for its intended purpose,
  and shall not duplicate, disclose, or disseminate any of this information in
  any manner unless Synaptics has otherwise provided express, written permission.
  Use of the materials may require a license of intellectual property from a
  third party or from Synaptics. This file conveys no express or implied licenses
  to any intellectual property rights belonging to Synaptics.
--------------------------------------------------------------------------------

  File Name: hifi3_cmd.c

  Description: command processor which gets and parses commands​

------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------------------
    Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes.
        Valid options are:
            > ASSERT_DISABLE
            > ASSERT_LEVEL_ERR
            > ASSERT_LEVEL_WARN
            > ASSERT_LEVEL_INFO
            > ASSERT_LEVEL_ALL
*/
#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
/* --------------------------------------------------------------------------------------------- */
#include "autoconf.h"

#include "print.h"
#include "AudioSmart_sendcmd_common.h"


#if (!defined(SIMULATION)) || (defined(SIMULATION) &&  defined(SIM_MULTIPLE_CORE))

#define ENABLE_SEND_CMD_DEMO


#ifdef ENABLE_SEND_CMD_DEMO
void PrintCmdResult(void *p_void_cmd) {

  Command *p_cmd = (Command *)p_void_cmd;

#if defined(__arm__)
  Print_DBG("showing from ARM\n");
#else
  Print_DBG("showing from DSP\n");
#endif

  if (p_cmd->head.reply) {
    Print_DBG("Cmd is done, return value is:");
    int *p_cmd_data = (int *)&(p_cmd->data[0]);
    if (p_cmd->head.num_32b_words >= 0) {
      for (int i = 0; i < p_cmd->head.num_32b_words; i++) {
        Print_DBG("%d\t", p_cmd_data[i]);
      }
  }
    else {
      Print_DBG("Cmd return error = %d\n", p_cmd->head.num_32b_words);
    }
    Print_DBG("\n");
  }
  else {
    Print_DBG("Cmd timeout\n");
  }
  return;
}

#if defined(__arm__)
void AsyncCmdCallback(void *p_void_cmd) {
  PrintCmdResult(p_void_cmd);
}
#else
void TriggerEventHandledCallback(void *p_cmd) {

  Command *cmd = (Command *)p_cmd;
  Print_DBG("TriggerEventHandledCallback %x, %x\n", cmd->head.mgr_module_id, cmd->head.command_id);
  if (cmd->head.reply) {
    Print_DBG("TriggerEvent Cmd done\n");
  }
  else {
    Print_ERR("TriggerEvent Cmd timeout\n");
  }
  return;
}
#endif

int32_t DemoSendCmd(Command *p_cmd) {
 
  uint32_t       ret_val = 0;
  uint32_t x;

  Print_ERR("DemoSendCmd is called\n");

  //Option1: Existing way
#if 1
  ret_val = SENDCMD(p_cmd, _ID('C', 'T', 'R', 'L'), CMD_GET(CONTROL_MGR_VERSION));
  PrintCmdResult(p_cmd);
#endif

  //Option 2: Use command structure and synchronous way
#if 1
  p_cmd->head.num_32b_words = MAX_COMMAND_SIZE;
  p_cmd->head.command_id = CMD_GET(CONTROL_MGR_VERSION);
  p_cmd->head.reply = 0;
  p_cmd->head.mgr_module_id = _ID('C', 'T', 'R', 'L');
  ret_val = SENDCMD_SYNC(p_cmd);
  PrintCmdResult(p_cmd);
#endif

 #if defined(__arm__)
  //Option 3: Use command structure but asynchronous way
  p_cmd->head.num_32b_words = MAX_COMMAND_SIZE;
  p_cmd->head.command_id = CMD_GET(CONTROL_MGR_VERSION);
  p_cmd->head.reply = 0;
  p_cmd->head.mgr_module_id = _ID('C', 'T', 'R', 'L');
  ret_val = SENDCMD_ASYNC(p_cmd, AsyncCmdCallback);
#else
  //also show how to send a trigger event from DSP to MCU for DSP developers
  p_cmd->head.num_32b_words = 3;
  p_cmd->head.command_id = SYS_CMD_EVENT_PARAM;
  p_cmd->head.reply = 0;
  p_cmd->head.mgr_module_id = _ID('M', 'C', 'U', ' ') | FWID_SYS;
  p_cmd->data[0] = EVENT_TRIG_DETECTED;
  p_cmd->data[1] = EVENT_PAR_TRIG_DETECTED_INDEX;
  p_cmd->data[2] = 1;   //Need to set a proper value based on trigger ID table
  ret_val = SENDCMD_ASYNC(p_cmd, TriggerEventHandledCallback);
#endif

  return ret_val;
}

#endif   //end of ENABLE_SEND_CMD_DEMO


#endif

