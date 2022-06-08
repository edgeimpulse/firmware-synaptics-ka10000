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

  File Name: usr_demo.c

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
#ifndef __DOMAIN_NS
#define __DOMAIN_NS
#endif

#include "mcu.h"
#include "driver.h"
#include "gpio_drv.h"
#include "madc_drv.h"
#include "command.h"
#include "AudioSmart_sendcmd_common.h"

#include "fdma_drv.h"
#include "uart_drv.h"
#include "usr_demo.h"
#include "fdma_drv.h"
#ifdef CONFIG_LLE_DEMO_IPC
#include "ipc_drv.h"
#endif


#if (defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS))

#if (defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))

#define UART_RX_PENDING       1

#define TOGGLE_GPIO_NUMBER    5   //GPIO 5

typedef struct usr_demo {
	TaskHandle_t   usr_TaskHandle;
	volatile uint32_t loop_processing_delay;
	volatile uint32_t enable_gp_toggle_test;
	volatile uint32_t enable_madc_test;
	volatile uint32_t uart_buffer;
#ifdef CONFIG_MCU_DRV_FDMA
	volatile uint32_t fdma_done;
	volatile uint32_t fdma_channel;
#endif
	os_TaskHookMsg uart_rx_pending_event_msg;
} usr_demo;

static usr_demo UsrDemo;

void usr_demo_set_delay(const unsigned long TimeInms)
{
	UsrDemo.loop_processing_delay = TimeInms;
    Print_DBG("Processing Loop Interval %d ms\n", UsrDemo.loop_processing_delay);
}

void enable_toggle_gp_test(const unsigned long enable)
{
	if(enable)
	{
		gpiomux_select_gpio(TOGGLE_GPIO_NUMBER,IO_MUX_ENABLE,OUT_DIR, DRV_CUR_2mA_FAST, PULL_DISABLED);
	} else
	{
		gpiomux_select_gpio(TOGGLE_GPIO_NUMBER,IO_MUX_DISABLE,0, 0, PULL_DISABLED);
	}
	UsrDemo.enable_gp_toggle_test = enable;
    Print_DBG("%s GPIO Test\n", enable==1 ? "Enable" : "Disable");
}

void enable_madc_test(const unsigned long enable)
{
	UsrDemo.enable_madc_test = enable;
    Print_DBG("%s MADC Test\n", enable==1 ? "Enable" : "Disable");
}

void run_toggle_gp_test()
{
	static int gpio_state = 0;
	if(UsrDemo.enable_gp_toggle_test)
	{
		gpio_set(TOGGLE_GPIO_NUMBER,gpio_state++%2);  //Toggle GPIO
	}
}

int run_madc_test(void)
{
	int i, ret;
	uint32_t madc_dev = DEV_MADC;
	uint32_t madc_val[3];

	if(UsrDemo.enable_madc_test)
	{
#ifdef CONFIG_MCU_DRV_MADC
		for(i=0; i<3; i++){ //cycle thru all pins
			// Open madc device 
			// DEV_BATT = DEV_MADC+0  DEV_TEMP = DEV_MADC+1  DEV_KYPD = DEV_MADC+2
			madc_val[i] = madc_get(madc_dev);
			madc_dev++;
		}
		
		Print_DBG("\t VBAT: 0x%x %d \t TMPSNS: 0x%x %d \t MBSNS: 0x%x %d \n", 
			madc_val[0], madc_val[0], madc_val[1],  madc_val[1], madc_val[2],  madc_val[2]);
		madc_dev = DEV_MADC;
#endif
	}

	return 0;
}

#ifdef CONFIG_MCU_DRV_FDMA

#define FDMA_TEST_DEVICE		0	/* 0 = FDMA-0,  1 = FDMA-1 */
#define FDMA_TEST_CHANNEL		1   /* 0~7 for each channel */

static void fdma_demo_callback(void *param)
{
	UsrDemo.fdma_done = 1;
	UsrDemo.fdma_channel = (int)param;

	return;
}

int run_fdma_test(uint32_t dev_id, uint32_t channel)
{
	uint32_t *src, *dst;
	int i = 0;
	int copy_data_len = 0;
	uint32_t dev = DEV_FDMA(dev_id, channel);

	src = (uint32_t *)os_Malloc(256 * 4);
	dst = (uint32_t *)os_Malloc(256 * 4);

	if (os_open(dev, 0) < 0)
	{
		return -ENODEV;
	}

	for(i = 0; i < 256; i++)
	{
		if(i%2 == 0)
			src[i] = 0xA1B2C300 + i;
		else
			src[i] = 0xD4E5F600 + i;
		dst[i] = 0;
	}

	UsrDemo.fdma_done = 0;
	fdma_set_irq_cb(dev, 0, fdma_demo_callback, (void *)channel);

	Print_DBG("User FDMA Demo\n");

	/* Normal FDMA Memory To Memory */
	copy_data_len = fdma_m2m_transfer(	dev, 			\
										(dmaaddr_t)dst, \
										(dmaaddr_t)src, \
										256, 			\
										FDMA_NO_FLAG);

	while ((UsrDemo.fdma_done == 0))
	{
		os_TaskSleep(1);
	}

	Print_DBG("Data Transfer=%d\n", copy_data_len);

	for (i = 0; i < 256; i++)
	{
		dst[i] = 0;
	}

	/* Packing Mode FDMA Memory To Memory */
	UsrDemo.fdma_done = 0;
	copy_data_len = fdma_m2m_transfer(	dev, 			\
										(dmaaddr_t)dst, \
										(dmaaddr_t)src, \
										256, 			\
										(FDMA_FLAG_SET | FDMA_SAMPLE_TYPE_PACK_16D_16D));

	while ((UsrDemo.fdma_done == 0))
	{
		os_TaskSleep(1);
	}

	Print_DBG("Pack Mode Data Transfer=%d\n", copy_data_len);


	os_close(dev);

	os_Free(src);
	os_Free(dst);

	return 0;
}

#endif //CONFIG_MCU_DRV_FDMA

//#define ENABLE_SEND_CMD_DEMO
extern int32_t DemoSendCmd(Command *p_cmd);

void check_fw_version()
{
#ifndef CONFIG_CHIP_NAME_KATANA
	Command cmd;	
	int ret_val = SENDCMD(&cmd, _ID('C', 'T', 'R', 'L'), CMD_GET(CONTROL_MGR_VERSION));  //query system for fw version
	if (ret_val==MODULE_RC_SUCCESS)
		Print_DBG("FW Version: %d.%d.%d.%d\n", cmd.data[0],cmd.data[1],cmd.data[2],cmd.data[3]);
	else
		Print_DBG("FW Version: ERROR (%d)\n",ret_val);
#endif
}

#ifdef CONFIG_MCU_DRV_UART_FDMA
//UART callback
static void uart_fdma_irq_callback (void *param)
{
  UsrDemo.uart_rx_pending_event_msg.msg = UART_RX_PENDING;  //read complete, send pending event
  os_TaskHook(UsrDemo.usr_TaskHandle, &UsrDemo.uart_rx_pending_event_msg);
  return;
}

//Initialize the UART (NON BLOCKING - DMA MODE) for loopback 
int init_uart_loopback()
{
	if(os_open(DEV_FDMA_UART(FDMA_0,UART_TX_FDMA_CH),0) < 0)
	{
		return -ENODEV;
	}
	if(os_open(DEV_FDMA_UART(FDMA_0,UART_RX_FDMA_CH),0) < 0)  
	{
		return -ENODEV;
	}
	uart_set_callback(NULL, uart_fdma_irq_callback);

	uart_read_dwords((dmaaddr_t)&UsrDemo.uart_buffer, (dwsize_t)1, UART_FIFO_UNPACK_FALSE);
	
	return 0;
}

//Loopback UART one DWORD at a time
void run_uart_loopback()
{
	uart_write_dwords((dmaaddr_t)&UsrDemo.uart_buffer, (dwsize_t)1);  //write last dword
	uart_read_dwords((dmaaddr_t)&UsrDemo.uart_buffer, (dwsize_t)1, UART_FIFO_UNPACK_FALSE);   //read next dword
}
#endif


void usr_demo_run(void *pvParam)
{
	Command cmd;
	int count = 0;
	Print_DBG("Running User Demo \n");

#ifdef CONFIG_MCU_DRV_UART_FDMA	
	if (init_uart_loopback() < 0)
		Print_DBG("UART init error\n");
#endif

	check_fw_version();
	usr_demo_set_delay(10000); //set processing loop delay to 10 secs

#ifdef CONFIG_MCU_DRV_FDMA
	run_fdma_test(FDMA_TEST_DEVICE, FDMA_TEST_CHANNEL);
#endif

	while(1){
		Print_DBG("usr_demo loop %d.\n", count);
		os_TaskSleep(UsrDemo.loop_processing_delay);
		run_toggle_gp_test();
		run_madc_test();
#if 0//def CONFIG_LLE_DEMO_IPC
		if(count==1)
		{
			uint32_t test_msg = 0;
			ipc_send_msg(IPC_TARGET_LLEA, &test_msg, IPC_BLOCK);
		}
#endif
#ifdef ENABLE_SEND_CMD_DEMO
		DemoSendCmd(&cmd);
#endif
		count++;
	}

	Print_DBG("%s %d: End user demo.\n", __func__, __LINE__);

}

long usr_demo_hook(void *pvParam )
{
  os_TaskHookMsg* EventMsg = (os_TaskHookMsg*) pvParam;

  switch(EventMsg->msg)
  {
	  case UART_RX_PENDING:
#ifdef CONFIG_MCU_DRV_UART_FDMA	
		run_uart_loopback();
#endif
	  break;
  }
  
  return 0;
}

int usr_demo_start(void)
{
    os_TaskInit	usrTaskInit;
    memset(&usrTaskInit, 0, sizeof(os_TaskInit));
    memset(&UsrDemo, 0, sizeof(usr_demo));

    usrTaskInit.ContextID     = _ID('U','D','M','O');  
    usrTaskInit.TaskFunc      = usr_demo_run; 
    usrTaskInit.TaskHookFunc  = usr_demo_hook;
    usrTaskInit.TaskName      = "usr_demo";
    usrTaskInit.TaskStackSize = 512U;
    usrTaskInit.TaskPriority  = OS_TASK_MID_PRIORITY;
    usrTaskInit.TaskTZModule  = 0;

	UsrDemo.usr_TaskHandle = os_TaskCreate(&usrTaskInit);
	if (UsrDemo.usr_TaskHandle == 0)
	{
		Print_DBG("CREATE_USER_MODE_TASK - usr_demo failed\n ");
	}

#ifndef CONFIG_CHIP_NAME_KATANA
	AudioSmart_mid_level_init(NULL); //want to use CmdNotifier, so init it here
#endif
    return 0;
}

#endif //(defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))
#endif //(defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS))
