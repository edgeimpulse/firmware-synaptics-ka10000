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

#ifndef _MCU_MAJOR_H_
#define _MCU_MAJOR_H_

#include "mcu.h"

// Device major numbers
#define MAJOR_CLCK		0                   // clock/pll
#ifdef CONFIG_MCU_DRV_TIMER
#define MAJOR_TIMR		(MAJOR_CLCK + 1)    // timers
#else
#define MAJOR_TIMR      MAJOR_CLCK
#endif

#ifdef CONFIG_MCU_DRV_SPIM
#define MAJOR_SPIM		(MAJOR_TIMR + 1)    // spi master
#else
#define MAJOR_SPIM		MAJOR_TIMR
#endif

#ifdef CONFIG_MCU_DRV_SMEM
#define MAJOR_SMEM      (MAJOR_SPIM + 1)
#else
#define MAJOR_SMEM      MAJOR_SPIM
#endif

#ifdef CONFIG_MCU_DRV_I2CM
#define MAJOR_I2CM		(MAJOR_SMEM + 1)    // i2c master (only 1 I2C Master)
#else
#define MAJOR_I2CM      MAJOR_SMEM
#endif

#ifdef CONFIG_MCU_DRV_I2CS
#define MAJOR_I2CS      (MAJOR_I2CM + 1)   // i2c slave
#else
#define MAJOR_I2CS      MAJOR_I2CM
#endif

#ifdef CONFIG_MCU_DRV_I2S_PCM
#define MAJOR_I2SD      (MAJOR_I2CS + 1)    // i2s/dsp pcm driver
#else
#define MAJOR_I2SD      MAJOR_I2CS
#endif

#ifdef CONFIG_MCU_DRV_I2S_PCM_OLD
#define MAJOR_I2SM      (MAJOR_I2SD + 1)    // i2s/pcm
#else
#define MAJOR_I2SM      MAJOR_I2SD
#endif

#ifdef CONFIG_MCU_DRV_FDMA
#define MAJOR_FDMA      (MAJOR_I2SM + 1)    // flexi DMA
#else
#define MAJOR_FDMA      MAJOR_I2SM
#endif

#ifdef CONFIG_MCU_DRV_LEDS
#define MAJOR_LEDS      (MAJOR_FDMA + 1)    // tri-color leds
#else
#define MAJOR_LEDS      MAJOR_FDMA
#endif

#ifdef CONFIG_MCU_DRV_ADMA
#define MAJOR_ADMA		(MAJOR_LEDS + 1)    // audio DMA
#else
#define MAJOR_ADMA      MAJOR_LEDS
#endif

#ifdef CONFIG_MCU_DRV_UART
#define MAJOR_UART		(MAJOR_ADMA + 1)	// uart
#else
#define MAJOR_UART      MAJOR_ADMA
#endif

#ifdef CONFIG_MCU_DRV_DMIC
#define MAJOR_DMIC      (MAJOR_UART + 1)    // dmic
#else
#define MAJOR_DMIC      MAJOR_UART
#endif

#ifdef CONFIG_MCU_DRV_GPIO
#define MAJOR_GPIO      (MAJOR_DMIC + 1)    // GPIO
#else
#define MAJOR_GPIO       MAJOR_DMIC
#endif

#ifdef CONFIG_MCU_DRV_MBOX
#define MAJOR_MBOX      (MAJOR_GPIO + 1)    // M33 Mailbox
#else
#define MAJOR_MBOX      MAJOR_GPIO
#endif

#ifdef CONFIG_MCU_DRV_EARLINK_MASTER
#define MAJOR_EARLINK_MST   (MAJOR_MBOX + 1) //EARLINK Master
#else
#define MAJOR_EARLINK_MST   MAJOR_MBOX
#endif

#ifdef CONFIG_MCU_DRV_EARLINK_SLAVE
#define MAJOR_EARLINK_SLV   (MAJOR_EARLINK_MST + 1) //EARLINK Slave
#else
#define MAJOR_EARLINK_SLV   MAJOR_EARLINK_MST
#endif

#ifdef CONFIG_MCU_DRV_LOCKS
#define MAJOR_LOCKS     (MAJOR_EARLINK_SLV + 1)
#else
#define MAJOR_LOCKS     MAJOR_EARLINK_SLV
#endif

#ifdef CONFIG_MCU_DRV_AES
#define MAJOR_AES       (MAJOR_LOCKS + 1)
#else
#define MAJOR_AES       MAJOR_LOCKS
#endif

#ifdef CONFIG_MCU_DRV_QSPI
#define MAJOR_QSPI      (MAJOR_AES + 1)
#else
#define MAJOR_QSPI      MAJOR_AES
#endif

#ifdef CONFIG_MCU_DRV_RRAM
#define MAJOR_RRAM      (MAJOR_QSPI + 1)
#else
#define MAJOR_RRAM      MAJOR_QSPI
#endif

#ifdef CONFIG_MCU_DRV_I3CM
#define MAJOR_I3CM      (MAJOR_RRAM + 1)
#else
#define MAJOR_I3CM      MAJOR_RRAM
#endif

#ifdef CONFIG_MCU_DRV_I3CS
#define MAJOR_I3CS      (MAJOR_I3CM + 1)
#else
#define MAJOR_I3CS      MAJOR_I3CM
#endif

#ifdef CONFIG_MCU_DRV_ADC
#define MAJOR_ADC       (MAJOR_I3CS + 1)
#else
#define MAJOR_ADC       MAJOR_I3CS
#endif

#ifdef CONFIG_MCU_DRV_DAC
#define MAJOR_DAC       (MAJOR_ADC + 1)
#else
#define MAJOR_DAC       MAJOR_ADC
#endif

#ifdef CONFIG_MCU_DRV_PWR_MGT
#define MAJOR_PWR_MGT   (MAJOR_DAC + 1)
#else
#define MAJOR_PWR_MGT   MAJOR_DAC
#endif

#ifdef CONFIG_MCU_DRV_SYNC
#define MAJOR_SYNC      (MAJOR_PWR_MGT + 1)
#else
#define MAJOR_SYNC      MAJOR_PWR_MGT
#endif

#ifdef CONFIG_MCU_DRV_SFS
#define MAJOR_SFS       (MAJOR_SYNC + 1)
#else
#define MAJOR_SFS       MAJOR_SYNC
#endif

#ifdef CONFIG_MCU_DRV_AVAD
#define MAJOR_AVAD		(MAJOR_SFS + 1)
#else
#define MAJOR_AVAD      MAJOR_SFS
#endif

#ifdef CONFIG_MCU_DRV_MADC
#define MAJOR_MADC		(MAJOR_AVAD + 1)
#else
#define MAJOR_MADC      MAJOR_AVAD
#endif

#ifdef CONFIG_MCU_DRV_KEYPAD
#define MAJOR_KEYPAD	(MAJOR_MADC + 1)
#else
#define MAJOR_KEYPAD     MAJOR_MADC
#endif

#ifdef CONFIG_MCU_DRV_CAMERA
    #define MAJOR_CAMERA    (MAJOR_KEYPAD + 1)
#else
    #define MAJOR_CAMERA    MAJOR_KEYPAD
#endif

#ifdef CONFIG_MCU_DRV_SWPWM
#define MAJOR_SWPWM 	(MAJOR_CAMERA + 1)
#else
#define MAJOR_SWPWM     MAJOR_CAMERA
#endif

#ifdef CONFIG_MCU_DRV_BLUETOOTH
    #define MAJOR_BLUETOOTH (MAJOR_SWPWM + 1)
#else
    #define MAJOR_BLUETOOTH MAJOR_SWPWM
#endif

#ifdef CONFIG_MCU_PM_LITE
    #define MAJOR_PM_LITE   (MAJOR_BLUETOOTH + 1)
#else
    #define MAJOR_PM_LITE   MAJOR_BLUETOOTH
#endif

#ifdef CONFIG_MCU_DRV_WIFI
    #define MAJOR_WIFI      (MAJOR_PM_LITE + 1)
#else
    #define MAJOR_WIFI      MAJOR_PM_LITE
#endif
                                                //<== Add New To Above
#define MAJOR_LAST          (MAJOR_WIFI + 1)    // ##### LAST ####

//-----------------------  SW APIS ------------------------------
#define MAJOR_API_START     MAJOR_LAST
#define MAJOR_API_PRINTF    (MAJOR_API_START  + 0)
#define MAJOR_API_LAST      (MAJOR_API_PRINTF + 1)

#endif // _MCU_MAJOR_H_
