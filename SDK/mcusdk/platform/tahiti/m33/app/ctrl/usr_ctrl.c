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

  File Name: usr_ctrl.c

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

#include "mcu.h"
#include "command.h"
#include "usr_demo.h"

typedef enum  {
	USER_DEMO_SET_DELAY  = 0,
	USER_DEMO_ENABLE_GPIO_TEST = 1,
	USER_DEMO_ENABLE_MADC_TEST = 2,	
} USER_DEMO_COMMANDS;

#if (defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))
	extern int usr_demo_set_delay(const unsigned long TimeInms);
	extern int usr_demo_start(void);
	extern int enable_toggle_gp_test(const unsigned long enable);
	extern int enable_madc_test(const unsigned long enable);
#endif
  
//CAF Command Handler
//golem::sendcmd MCU 0x3 0 {100}
int process_ext_cmd(Command *cmd)
{	
    int ret_val = 0;
#if defined(CONFIG_SDK_MCU)	
    switch (CMD_ITEM(cmd->head.command_id))
    {
		case USER_DEMO_SET_DELAY:
			if(cmd->head.num_32b_words>0)
			{
#if (defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))
				usr_demo_set_delay((unsigned long)cmd->data[0]);
#endif
			}
			else
			{
				ret_val = -1;
			}
				
			cmd->head.num_32b_words = 0;
			cmd->head.reply = 1;
		break;
		case USER_DEMO_ENABLE_GPIO_TEST:
			if(cmd->head.num_32b_words>0)
			{
#if (defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))
					enable_toggle_gp_test((unsigned long)cmd->data[0]);
#endif
			}
			else
			{
				ret_val = -1;
			}

			cmd->head.num_32b_words = 0;
			cmd->head.reply = 1;
		break;
		case USER_DEMO_ENABLE_MADC_TEST:
			if(cmd->head.num_32b_words>0)
			{
#if (defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))
				enable_madc_test((unsigned long)cmd->data[0]);
#endif
			}
			else
			{
				ret_val = -1;
			}

			cmd->head.num_32b_words = 0;
			cmd->head.reply = 1;
		break;		
	}
#endif
	return ret_val;
}
