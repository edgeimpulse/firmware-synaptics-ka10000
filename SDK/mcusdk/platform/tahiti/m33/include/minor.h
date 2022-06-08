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
//  Copyright (C) 2018 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_MINOR_H_
#define _MCU_MINOR_H_

// Device minor numbers
#define MINOR_CLOCK_0       0	// clock/pll
#define MINOR_TIMR_0	    0	// timer 0
#define MINOR_TIMR_1	    1	// timer 1
#define MINOR_TIMR_2	    2	// timer 2
#define MINOR_TIMR_3	    3	// timer 3
#define MINOR_TIMR_4	    4	// timer 4
#define MINOR_TIMR_5	    5	// timer 5
#define MINOR_TIMR_6	    6	// timer 6
#define MINOR_TIMR_7	    7	// timer 7
#define MINOR_TIMR_8	    8	// timer 8
#define MINOR_TIMR_9	    9	// timer 9
#define MINOR_TIMR_10	    10	// timer 10
#define MINOR_TIMR_11	    11	// timer 11
#define MINOR_TIMR_12	    12	// timer 12
#define MINOR_TIMR_13	    13	// timer 13
#define MINOR_TIMR_14	    14	// timer 14
#define MINOR_SPIM_0        0   // spi master, cs0
#define MINOR_SMEM_0        0
#define MINOR_I2CM_0	    0	// i2c master=0, minor=0
#define MINOR_I2CS_0        0
#define MINOR_I2SD_0        0
#define MINOR_I2SD_1        1
#define MINOR_I2SD_2        2
#define MINOR_I2SM_0        0
#define MINOR_I2SM_1        1
#define MINOR_I2SM_2        2
#define MINOR_FDMA_0	    0	// flexi DMA 0 / Channel 0
#define MINOR_LEDS_0        0
#define MINOR_LEDS_1        1
#define MINOR_LEDS_2        2
#define MINOR_ADMA_0        0    
#define MINOR_ADMA_1        1    
#define MINOR_ADMA_2        2    
#define MINOR_ADMA_3        3    
#define MINOR_ADMA_5        5    
#define MINOR_ADMA_6        6
#define MINOR_ADMA_7        7
#define MINOR_ADMA_8        8
#define MINOR_ADMA_9        9
#define MINOR_ADMA_10       10
#define MINOR_ADMA_11       11
#define MINOR_ADMA_12       12
#define MINOR_ADMA_13       13
#define MINOR_ADMA_14       14
#define MINOR_ADMA_15       15
// ADMA 3: 48-63 + 64-67 (audio peripherals)
#define MINOR_ADMA_60  60    // ADMA 3 Ch0: DEV_ADMA_I2S_PCM1_RX_L
#define MINOR_ADMA_61  61    // ADMA 3 Ch1: DEV_ADMA_I2S_PCM1_RX_R
#define MINOR_ADMA_62  62    // ADMA 3 Ch2: 
#define MINOR_ADMA_63  63    // ADMA 3 Ch3: 
#define MINOR_ADMA_64  64    // ADMA 3 Ch4: 
#define MINOR_ADMA_65  65    // ADMA 3 Ch5: 
#define MINOR_ADMA_66  66    // ADMA 3 Ch6: DEV_ADMA_I2S_PCM2_RX_L
#define MINOR_ADMA_67  67    // ADMA 3 Ch7: DEV_ADMA_I2S_PCM2_RX_R
#define MINOR_ADMA_68  68    // ADMA 3 Ch8: DEV_ADMA_I2S_PCM3_RX_L
#define MINOR_ADMA_69  69    // ADMA 3 Ch9: DEV_ADMA_I2S_PCM3_RX_R

#define MINOR_ADMA_70  70    // ADMA 3 Ch10: DEV_ADMA_I2S_PCM1_TX_L
#define MINOR_ADMA_71  71    // ADMA 3 Ch11: DEV_ADMA_I2S_PCM1_TX_R
#define MINOR_ADMA_72  72    // ADMA 3 Ch12: DEV_ADMA_CAB0_DAC0
#define MINOR_ADMA_73  73    // ADMA 3 Ch13: DEV_ADMA_CAB0_DAC1
#define MINOR_ADMA_74  74    // ADMA 3 Ch14: DEV_ADMA_PWM1
#define MINOR_ADMA_75  75    // ADMA 3 Ch15: DEV_ADMA_PWM2
#define MINOR_ADMA_76  76    // ADMA 3 Ch16: DEV_ADMA_PWM3
#define MINOR_ADMA_77  77    // ADMA 3 Ch17: DEV_ADMA_PWM4
#define MINOR_ADMA_78  78    // ADMA 3 Ch18: DEV_ADMA_I2S_PCM2_TX_L
#define MINOR_ADMA_79  79    // ADMA 3 Ch19: DEV_ADMA_I2S_PCM2_TX_R
// ADMA DMIC: Reserve range 80 - 99
#define MINOR_DMICDMA_89  89    // ADMA DMIC Ch9:  DEV_ADMA_DIGMIC0_RX_L
#define MINOR_DMICDMA_91  91    // ADMA DMIC Ch11: DEV_ADMA_DIGMIC0_RX_R
#define MINOR_DMICDMA_93  93    // ADMA DMIC Ch13: DEV_ADMA_DIGMIC1_RX_L
#define MINOR_DMICDMA_95  95    // ADMA DMIC Ch15: DEV_ADMA_DIGMIC1_RX_R
#define MINOR_UART_0        0
#define MINOR_UART_1        1
#define MINOR_SAIF_0        0
#define MINOR_SAIF_1        1
#define MINOR_SAIF_2        2
#define MINOR_DMIC_0        0
#define MINOR_GPIO_0        0
#define MINOR_USBD_0        0

#define MINOR_I2SD_0        0
#define MINOR_I2SD_1        1
#define MINOR_I2SD_2        2
#define MINOR_MBOX_0        0 // MBOX0
#define MINOR_MBOX_1        1 // MBOX1
#define MINOR_MBOX_2        2 // MBOX2
#define MINOR_MBOX_3        3 // MBOX3
#define MINOR_MBOX_4        4 // MBOX3
#define MINOR_MBOX_5        5 // MBOX3

#define MINOR_MBOX_A530_0   0 // MBOX0
#define MINOR_MBOX_A530_1   1 // MBOX1
#define MINOR_MBOX_A530_2   2 // MBOX2
#define MINOR_MBOX_A530_3   3 // MBOX3

#define MINOR_MBOX_A531_0   0 // MBOX0
#define MINOR_MBOX_A531_1   1 // MBOX1
#define MINOR_MBOX_A531_2   2 // MBOX2
#define MINOR_MBOX_A531_3   3 // MBOX3

#define MINOR_MBOX_HIFI4_0  0 // MBOX0
#define MINOR_MBOX_HIFI4_1  1 // MBOX1
#define MINOR_MBOX_HIFI4_2  2 // MBOX2
#define MINOR_MBOX_HIFI4_3  3 // MBOX3

#define MINOR_OSPI_0        0
#define MINOR_RTC_0         0
#define MINOR_SDIO_0        0

#define MINOR_EARLINK_MST_0     0
#define MINOR_EARLINK_SLV_0     0
#define MINOR_LOCKS_0       0
#define MINOR_AES_0         0
#define MINOR_SYNC_0        0

#define MINOR_I3CM_0        0 // I3C Master 0  
#define MINOR_I3CS_0        0 // I3C SLAVE  0

#define MINOR_MADC_0		0
#define MINOR_MADC_1		1
#define MINOR_MADC_2		2

#define MINOR_CAMERA_0      0

#define MINOR_BLUETOOTH_0   0

#endif // _MCU_MINOR_H_
