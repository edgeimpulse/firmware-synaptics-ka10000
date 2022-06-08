////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Synaptics that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, SYNAPTICS
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2019 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
/**
 * \file gpio_drv.h
 * \name GPIO Driver API
 * \defgroup Programming_API_GPIO General Purpose Input/Output (GPIO)
 * \ingroup HW_DRV_API
 * @{
 *
 *
 * gpio_drv.h
 * 
 * \section Introduction
 * GPIO APIs configures a device pin for use as an INPUT or OUTPUT GPIO. 
 * INPUT GPIOs can be configured to generate an interrupt callback on transition. \n
 * 
 * \section Usage
 * The following sequence can be used to configure a GPIO as an output. \n
 *
 * \b Step 1:
 * Call gpiomux_select_gpio(GPIO_PIN_NUM, IO_MUX_ENABLE, OUT_DIR, DRV_CUR_2mA) to configure one GPIO number as Output.
 * 
 * \b Step 2:
 * Calling gpio_set(GPIO_PIN_NUM, 1) to toggle GPIO low=0 or high=1.
 *
 * \section Example
 * Refer to usr_demo.c under mcusdk/app/demo for more detail example code.
 *
 */
 
/** @} */
/**
* \file gpio_drv.h
* \brief Those APIs can be used control the gpio
*/
#ifndef __GPIO_DRV_H__
#define __GPIO_DRV_H__

#include "mcu.h"

#ifdef CONFIG_MCU_DRV_GPIO

#ifdef CONFIG_CHIP_VERSION_A1
	#define MAX_GPIOs    		32
#else
	#define MAX_GPIOs    		27
#endif

#define IO_MUX_ENABLE       1
#define IO_MUX_DISABLE      0

#define IN_DIR		0	
#define OUT_DIR		1
#define NO_DIR		2

#define PULL_DISABLED	0
#define PULL_DOWN		1
#define PULL_UP			2

//irq type
enum GPIO_INTR_MODE
{
	GPIO_INTR_MODE_FALL=0,
	GPIO_INTR_MODE_RISE,
	GPIO_INTR_MODE_BOTH,
};

enum JTAG_CORES 
{
	JTAG_ARM,
	JTAG_HIFI3,
	JTAG_ASIPs
};

enum PWM_LEDs 
{
	PWM_LED_R,
	PWM_LED_G,
	PWM_LED_B,
	PWM_LED_RGB
};

enum UART_DIR 
{
	RX_DIR,
	TX_DIR
};

enum UART_FLOWCTRL 
{
	EXCLUDE_FLOW_CTRL,
	INCLUDE_FLOW_CTRL
};

enum I2S_MCLK_DRV 
{
	DRV_CUR_2mA_FAST,
	DRV_CUR_2mA_SLOW,
	DRV_CUR_8mA_FAST,
	DRV_CUR_8mA_SLOW,
	DRV_CUR_4mA_FAST,
	DRV_CUR_4mA_SLOW,
	DRV_CUR_12mA_FAST,
	DRV_CUR_12mA_SLOW
};

enum DMIC_NUM 
{
	eDMIC_0,
	eDMIC_1,
	eDMIC_2
};

enum AMIC_NUM 
{
	eAMIC_0,
	eAMIC_1
};


typedef int (*gpio_func_t) ( uint32_t param);
int gpio_irq_handler(uint32_t param);

extern void gpio_multiple_release(uint32_t gp_list);

/**
* @brief 	Request GPIO for Irq and register callback to gpio_irq_handler 
* @param 	gp			GPIO number
* @param 	edge		GPIO Edge detection (Fall or Rise)
* @param 	cb_func		GPIO pin callback function
* @param	cb_param	GPIO pin callback parameter
* @return 	0 on success or negative error code
*/
extern int32_t gpio_request_irq(uint32_t gp, uint32_t irq_rise_edge,  gpio_func_t cb_func, uint32_t cb_param);

/**
* @brief 	Set GPIO Output Value
* @param 	gp		GPIO number
*			level	Value (1 or 0)
* @return 	0 on success or negative error code
*/
extern int32_t gpio_set(uint32_t gp, uint32_t level);

/**
* @brief 	Get GPIO Input Value
* @param 	gp		GPIO number
* @return 	GPIO value or negative error code
*/
extern int32_t gpio_get(uint32_t gp);

/**
* @brief 	Set GPIO pin mux as GPIO
* @param 	gp			GPIO number
* @param 	mux_ena		1 = Set pins for JTAG  ; 0 = Release pins
* @param 	io_dir		OUT_DIR = 1		IN_DIR = 0	NO_DIR = 2
* @param 	drv_sr      3 = 4mA_slow   ;  2 = 4mA_fast   ;  1 = 2mA_slow  ;  0 = 2mA_fast 
*                       7 = 12mA_slow  ;  6 = 12mA_fast  ;  5 = 8mA_slow  ;  4 = 8mA_fast 
* @param 	pull_res    0 = PULL_DISABLED  ;  1 = PULL_DOWN  ;  2 = PULL_UP
* @return 	0 on success or negative error code
*/
extern int32_t gpiomux_select_gpio(uint32_t gp, uint32_t mux_ena, uint32_t io_dir, uint32_t drv_sr, uint32_t pull_res);

/**
* @brief 	JTAG MUX Pin Select
* @param 	mux_ena		1 = Enable JTAG for desired Core  ; 0 = Disable JTAG for desired Core
* @param 	core		JTAG_ARM, JTAG_ASIPs, JTAG_HIFI3
* @return 	0 on success or negative error code
*/
extern int32_t gpiomux_select_jtag(uint32_t mux_ena, uint32_t core);

/**
* @brief 	I2C Master MUX Select
* @param 	mux_ena		1 = Set pins for I2C_M ; 0 = Release pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_I2CM 
extern int32_t gpiomux_select_i2cm(uint32_t mux_ena);
#endif

/**
* @brief 	I2C Slave MUX Select
* @param 	mux_ena		1 = Set pins for I2C_S ; 0 = Release pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_I2CS
extern int32_t gpiomux_select_i2cs(uint32_t mux_ena);
#endif

/**
* @brief 	I3C Master MUX Select
* @param 	mux_ena		1 = Set pins for I3C_M ; 0 = Release pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_I3CM
extern int32_t gpiomux_select_i3cm(uint32_t mux_ena);
#endif

/**
* @brief 	I3C Slave MUX Select
* @param 	mux_ena		1 = Set pins for I3C_S ; 0 = Release pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_I3CS
extern int32_t gpiomux_select_i3cs(uint32_t mux_ena);
#endif

/**
* @brief 	I2S MUX Pins Select
* @param 	i2s_num		1 = Select I2S1  ; 0 = Select I2S0
* @param 	mux_ena		1 = Set pins for I2S   ; 0 = Release pins
* @param 	drv_sr      3 = 4mA_slow   ;  2 = 4mA_fast   ;  1 = 2mA_slow  ;  0 = 2mA_fast 
*                       7 = 12mA_slow  ;  6 = 12mA_fast  ;  5 = 8mA_slow  ;  4 = 8mA_fast 
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_I2S_PCM
extern int32_t gpiomux_select_i2s(uint32_t i2s_num, uint32_t mux_ena, uint32_t drv_sr );
#endif

/**
* @brief 	I2S MCLK Output Enable
* @param 	mux_ena		1 = Set pin for I2S_MCLK   ; 0 = Release pin
* @param 	drv_sr      3 = 4mA_slow   ;  2 = 4mA_fast   ;  1 = 2mA_slow  ;  0 = 2mA_fast 
*                       7 = 12mA_slow  ;  6 = 12mA_fast  ;  5 = 8mA_slow  ;  4 = 8mA_fast 
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_I2S_PCM
extern int32_t gpiomux_select_i2s_mclk(uint32_t mux_ena, uint32_t drv_sr );
#endif

/**
* @brief 	UART MUX Pins Select
* @param 	mux_ena		1 = Set pins for UART   ; 0 = Release pins
* @param 	dir			1 = Configure for TX	; 0 = Configure for RX
* @param 	flow_ctrl	1 = Set/Release CTS pins for TX or RTS pad for RX   	
*						0 = Do not Set/Release CTS/RTS pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_UART
extern int32_t gpiomux_select_uart(uint32_t mux_ena, uint32_t dir ,  uint32_t flow_ctrl);
#endif

#ifdef CONFIG_MCU_DRV_ADC
/**
* @brief 	DMIC MUX Pins Select
* @param 	dmic_num	2 = Select DMIC2 ; 1 = Select DMIC1  ; 0 = Select DMIC0
* @param 	mux_ena		1 = Set pins for DMIC   ; 0 = Release pins
* @return 	0 on success or negative error code
*/
extern int32_t gpiomux_select_dmic(uint32_t dmic_num, uint32_t mux_ena);

/**
* @brief 	Analog MICs Pins Select
* @param 	amic_num	1 = MIC1   ;  0 =  MIC0
* @param 	mux_ena		1 = Set pins for Analog MIC   ; 0 = Release pins
* @return 	0 on success or negative error code
*/
extern int32_t gpiomux_select_amic(uint32_t amic_num,  uint32_t mux_ena);
#endif

/**
* @brief 	PWM LEDs MUX Pins Select
* @param 	led_num		PWM_LED_R  ; PWM_LED_G  ; PWM_LED_B  ;  PWM_LED_RGB
* @param 	mux_ena		1 = Set pins for PWM LED   ; 0 = Release pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_LEDS
extern int32_t gpiomux_select_pwm( uint32_t led_num, uint32_t mux_ena);
#endif

/**
* @brief 	SPIM MUX Pins Select
* @param 	mux_ena		1 = Set pins for SPIM  ;  0 = Release pins
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_SPIM
extern int32_t gpiomux_select_spim( uint32_t mux_ena);
#endif

/**
* @brief 	MADC (VBAT/TEMP_SNS/MB_SNS) MUX Pins Select
* @param 	mux_ena		1 = Set pins for MADC  ;  0 = Release pins
* @param 	dev			DEV_TMPSNS = Configure Temp Sense pad
*						DEV_MBSNS = Configure Multi-Button pad
* @return 	0 on success or negative error code
*/
#ifdef CONFIG_MCU_DRV_MADC
extern int32_t gpiomux_select_madc(uint32_t mux_ena,uint32_t dev);
#endif

#endif // CONFIG_MCU_DRV_GPIO
#endif // _GPIO_DRV_H_
