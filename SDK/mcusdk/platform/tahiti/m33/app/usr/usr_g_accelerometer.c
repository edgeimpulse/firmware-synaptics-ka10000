////////////////////////////////////////////////////////////////////////////////
//
//  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Conexant Systems, Inc.
//  ("Conexant")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Conexant Systems Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Conexant that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, CONEXANT
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2009-2013 Conexant Systems Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
/* --------------------------------------------------------------------------------------------- 
	Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes. 
		Valid options are:
			> ASSERT_DISABLE
			> ASSERT_LEVEL_ERR
			> ASSERT_LEVEL_WARN
			> ASSERT_LEVEL_INFO
			> ASSERT_LEVEL_ALL
*/
#define  PRINT_LEVEL_SET    ASSERT_LEVEL_WARN
/* --------------------------------------------------------------------------------------------- */
#include "mcu.h"
#include "i3cm_drv.h"
#include "i2spcm_drv.h"
#include "mcu_cmds.h"
#include "command.h"
#include "base_module_ex.h"



#ifdef CONFIG_MCU_G_ACCELOMETER

#define LIS25BA_I2C_SLAVE_ADDR 0x19
#define LIS25BA_SPEED 100000

// Reg addresses
#define LIS25BA_TEST_REG (0x0B)
#define LIS25BA_WHO_AM_I_REG (0x0F)
#define LIS25BA_TDM_CMAX_LSB_REG (0x24)
#define LIS25BA_TDM_CMAX_MSB_REG (0x25)
#define LIS25BA_CTRL_REG (0x26)
#define LIS25BA_TDM_CTRL_REG (0x2E)
#define LIS25BA_AXES_CTRL_REG (0x2F)

// Reg values
#define LIS25BA_START (0x00)
#define LIS25BA_STOP (0x20)
#define LIS25BA_AXIS_XY (0x61)
#define LIS25BA_16KHZ (0x62)
#define LIS25BA_ENABLE_SELF_TEST (0x08)
#define LIS25BA_DISABLE_SELF_TEST (0x00)


typedef enum {
  ACCEL_RESERVED      =  0,
  ACCELOMETER_REG     =  1,
  ACCELOMETER_ENABLE  =  2,
  ACCELOMETER_TEST    =  3,
} AccelCmdCode;

struct accel_s
{
  os_TaskHandle task; // this task
} accel;

/*------------------------------------------------------------------------------------*/

int accelerometer_write_i2c(int reg_addr, int reg_val)
{
	uint8_t write_buffer[2] ={0};

	write_buffer[0] = reg_addr;
	write_buffer[1] = reg_val;
	return i2cm_xfer_write(DEV_I3CM0,LIS25BA_I2C_SLAVE_ADDR,LIS25BA_SPEED,write_buffer,2);
}

int accelerometer_read_i2c(int reg_addr, uint32_t *read_data)
{
	int ret_val;
	uint8_t write_buffer[1] ={0};
	uint8_t read_buffer[1]  ={0};

	write_buffer[0] = reg_addr;
	ret_val=i2cm_xfer_write(DEV_I3CM0,LIS25BA_I2C_SLAVE_ADDR,LIS25BA_SPEED,write_buffer,1);
	if(ret_val>=0)
		ret_val=i2cm_xfer_read(DEV_I3CM0, LIS25BA_I2C_SLAVE_ADDR, LIS25BA_SPEED,read_buffer, 1);
	if(ret_val>=0)
	{
		*read_data=read_buffer[0];
		ret_val=1;
	}
	return ret_val;
}

int AccelerometerAppCommand (Command *cmd) 
{
  int ret_val = MODULE_RC_SUCCESS;
  
  switch ((AccelCmdCode)(CMD_ITEM(cmd->head.command_id)))
  {
  case ACCELOMETER_REG:
    {
      //Print_ERR("Acel CMD\n");
      if(CMD_SET(cmd->head.command_id)==cmd->head.command_id) //set command
      {
        if(cmd->head.num_32b_words==2)  //if address, val
        {
            //Print_ERR("Write %x=%x\n",cmd->data[0],cmd->data[1]);
            ret_val=accelerometer_write_i2c(cmd->data[0], cmd->data[1]);
        }
        else
          ret_val=MODULE_RC_CMD_DATA_SIZE_WRONG;
      }
      else
      {
    	  //Print_ERR("Read %x ",cmd->data[0]);
    	  ret_val=accelerometer_read_i2c(cmd->data[0],&(cmd->data[0]));
    	  //Print_ERR("%x\n",cmd->data[0])
      }
    }
    break;
  case ACCELOMETER_ENABLE:
    {
        if(CMD_SET(cmd->head.command_id)==cmd->head.command_id) //set command
        {
        	if(cmd->data[0]) //enable
        	{
        		accelerometer_write_i2c(LIS25BA_TDM_CTRL_REG, LIS25BA_16KHZ); //TDM, 1 bit delay, latch on falling edge, 16Khz
        		accelerometer_write_i2c(LIS25BA_AXES_CTRL_REG, LIS25BA_AXIS_XY); //enable X and Y axis and ODR AUTO
        		ret_val=accelerometer_write_i2c(LIS25BA_CTRL_REG, LIS25BA_START); //enable accelerometer
        	}
        	else  //disable
        	{
        		ret_val=accelerometer_write_i2c(LIS25BA_CTRL_REG, LIS25BA_STOP); //disable accelerometer
        	}
        }
        else  //else get command
        {
        	ret_val=accelerometer_read_i2c(LIS25BA_CTRL_REG,&(cmd->data[0]));
        	if(cmd->data[0]==LIS25BA_START)
        		cmd->data[0]=1;
        	else
        		cmd->data[0]=0;
        }
    }
	break;
  case ACCELOMETER_TEST:
    {
      	if(cmd->data[0]) //enable
      	{
      		ret_val=accelerometer_write_i2c(LIS25BA_TEST_REG, LIS25BA_ENABLE_SELF_TEST); //enable self test
      	}
      	else  //disable
      	{
      		ret_val=accelerometer_write_i2c(LIS25BA_TEST_REG, LIS25BA_DISABLE_SELF_TEST); //disable self test
      	}
    }
    break;

    //todo add other commands here.
  default:
    ret_val=-EINVAL;
    break;
  }
  return (ret_val);
}


long accelometer_signal(void *pvParam )
{
  os_TaskHookMsg* EventMsg = (os_TaskHookMsg*) pvParam;

  switch(EventMsg->msg)
  {
	case SIG_CMD:
      {
    	 //Print_ERR("ACC SIG\n");
    	 Command  *cmd = (Command *)EventMsg->parm;
    	 cmd->head.num_32b_words = AccelerometerAppCommand(cmd);
    	 cmd->head.reply = 1;
      }
      break;
    default:
      return -1;
  }
  return 0;
}

int accelometer_init()
{
  memset(&accel,0,sizeof(struct accel_s));
  i2s_mclk_enable(DEV_I2SD,12288000,0,MCLK_START_I2S0); //make sure MCLK is enabled and outputing 12.288Mhz on GPIO2 when I2S0 master clock gate is enabled/disable
  accel.task=os_TaskGetCurrentHandle();
  accelerometer_write_i2c(LIS25BA_CTRL_REG, LIS25BA_STOP); //disable accelerometer
  return 0;
}


void accelometer_run(void *param)
{
  accelometer_init();
  for(;;)
  {
    os_TaskWaitEvent(NULL);     //wait for an event to happen indefinitely;
    //todo???
  }
}



os_TaskHandle accelometer_TaskHandle;
#define ACCELOMETER_STACK_SIZE					  96U 
os_TaskHandle g_accelometer_start(void)
{
    os_TaskInit	accelTaskInit;
    memset(&accelTaskInit, 0, sizeof(os_TaskInit));

    accelTaskInit.ContextID = _ID('A','C','C','E');  
    accelTaskInit.TaskFunc = accelometer_run; 
    accelTaskInit.TaskHookFunc = accelometer_signal;
    accelTaskInit.TaskName = "acel_task";
    accelTaskInit.TaskStackSize =ACCELOMETER_STACK_SIZE;
    accelTaskInit.TaskPriority = OS_TASK_MID_PRIORITY;
#if (__DOMAIN_NS == 1U)
	  accelTaskInit.TaskTZModule = 0U;
#endif
    accelometer_TaskHandle= os_TaskCreate(&accelTaskInit);
    return accelometer_TaskHandle;
}

#endif // CONFIG_MCU_G_ACCELOMETER
