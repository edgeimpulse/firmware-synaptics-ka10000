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

#ifndef _MCU_DEVICES_H_
#define _MCU_DEVICES_H_

// Macros to combine device major/minor nrs into one nr

#define DEV_MAJOR_SHIFT         8
#define DEV_MAJOR_MASK          0xFF

#define DEV_MINOR_MASK          0xFF

#define DEV_ADMA_ID_MASK        (0x3)
#define DEV_ADMA_CH_MASK        (0x1F)
#define DEV_ADMA_ID_OFFSET      (6)
#define DEV_ADMA_CH_OFFSET      (0)

#define MAJOR(dev)              (((dev)>>DEV_MAJOR_SHIFT)&DEV_MAJOR_MASK)
#define MINOR(dev)              ((dev)&DEV_MINOR_MASK)

#define DEVICE0(d)              ((MAJOR_##d)<<DEV_MAJOR_SHIFT)
#define DEVICEn(d,n)            (((MAJOR_##d)<<DEV_MAJOR_SHIFT)|(MINOR_##d##_##n))

#define DEV_ADMA_CH(adma,_ch)   (((MAJOR_ADMA)<<DEV_MAJOR_SHIFT)|(_ch & DEV_ADMA_CH_MASK) | ((adma & DEV_ADMA_ID_MASK) << DEV_ADMA_ID_OFFSET))

#define DEV_FDMA_SPIM(dma, _ch)	 DEV_FDMA(dma,_ch)
#define DEV_FDMA_I2CM(dma, _ch)  DEV_FDMA(dma,_ch)
#define DEV_FDMA_AES(dma, _ch)   DEV_FDMA(dma,_ch)
#define DEV_FDMA_I3CM(dma, _ch)  DEV_FDMA(dma,_ch)
#define DEV_FDMA_I3CS(dma, _ch)  DEV_FDMA(dma,_ch)
#define DEV_FDMA_UART(dma, _ch)  DEV_FDMA(dma,_ch)
// Device numbers
#define DEV_CLOCK               DEVICE0(CLCK)
#define DEV_TIMER0              DEVICEn(TIMR, 0)        /* GPTC#0 */
#define DEV_TIMER1              DEVICEn(TIMR, 1)        /* GPTC#1 */
#define DEV_TIMER2              DEVICEn(TIMR, 2)        /* GPTC#2 Asip*/
#define DEV_TIMER3              DEVICEn(TIMR, 3)        /* GPTC#3 Asip*/
#define DEV_TIMER4              DEVICEn(TIMR, 4)        /* GPTC#4 Asip*/
#define DEV_TIMER5              DEVICEn(TIMR, 5)        /* GPTC#5 Asip*/
#define DEV_TIMER6              DEVICEn(TIMR, 6)        /* GPTC#6 Asip*/
#define DEV_TIMER7              DEVICEn(TIMR, 7)        /* GPTC#7 Asip*/
#define DEV_SPTC                DEVICEn(TIMR, 8)        /* SPTC   */
#define DEV_ASTC0               DEVICEn(TIMR, 9)       /* ARM Single Timer#0 */
#define DEV_ASTC1               DEVICEn(TIMR, 10)       /* ARM Single Timer#1 */
#define DEV_ADTCA               DEVICEn(TIMR, 11)       /* ARM Dual Timer First Part or "A" */
#define DEV_ADTCB               DEVICEn(TIMR, 12)       /* ARM Dual Timer Second Part or "B" */
#define DEV_AWD1                DEVICEn(TIMR, 13)       /* ARM Watchdog #1" */
#define DEV_AWD2                DEVICEn(TIMR, 14)       /* ARM Watchdog #2" */

#define DEV_SPIM0               DEVICEn(SPIM, 0)
#define DEV_SMEM0               DEVICEn(SMEM, 0)
#define DEV_I2CM                DEVICEn(I2CM,0)
#define DEV_I2CS                DEVICE0(I2CS)
#define DEV_I2SD                DEVICE0(I2SD)
#define DEV_I2S0                DEVICEn(I2SD, 0)
#define DEV_I2S1                DEVICEn(I2SD, 1)

#define DEV_FDMA(dma,_ch)       (((MAJOR_FDMA)<<DEV_MAJOR_SHIFT)|(_ch & 0x7) | ((dma & 0x1) << 3))
#define DEV_LED0                DEVICEn(LEDS,0)
#define DEV_LED1                DEVICEn(LEDS,1)
#define DEV_LED2                DEVICEn(LEDS,2)
#define DEV_QSPI                DEVICE0(QSPI)
#define DEV_RRAM                DEVICE0(RRAM)

//#define DEV_ADMA                DEVICEn(ADMA, 0)
#define DEV_EARLINK_MST			DEVICEn(EARLINK_MST, 0)
#define DEV_EARLINK_SLV			DEVICEn(EARLINK_SLV, 0)
#define DEV_LOCKS               DEVICEn(LOCKS, 0)
#define DEV_AES                 DEVICEn(AES, 0)
#define DEV_ADC                 DEVICE0(ADC)
#define DEV_DAC                 DEVICE0(DAC)
#define DEV_ANALOG              DEVICE0(ANALOG)
#define DEV_PWR_MGT             DEVICE0(PWR_MGT)
#define DEV_SYNC                DEVICE0(SYNC)
#define DEV_SFS                 DEVICE0(SFS)
#define DEV_AVAD                DEVICE0(AVAD)
#define DEV_MADC				DEVICEn(MADC,0)
#define DEV_VBAT				DEVICEn(MADC,0)
#define DEV_TMPSNS				DEVICEn(MADC,1)
#define DEV_MBSNS				DEVICEn(MADC,2)
#define DEV_KEYPAD              DEVICE0(KEYPAD)
#define DEV_SWPWM               DEVICE0(SWPWM)

#define DEV_I3CM                DEVICE0(I3CM)
#define DEV_I3CS                DEVICE0(I3CS)
#define DEV_I3CM0		        DEVICEn(I3CM,0)
#define DEV_I3CS0		        DEVICEn(I3CS,0)


#define DEV_ADMA                DEVICE0(ADMA)
// ADMA Resources

// Devices mapped to ADMA0
#define DEV_ADMA0_HQ_LP_DECIMATOR_0         DEV_ADMA_CH(0,1) 
#define DEV_ADMA0_HQ_LP_DECIMATOR_1         DEV_ADMA_CH(0,3) 
#define DEV_ADMA0_HQ_LP_DECIMATOR_2         DEV_ADMA_CH(0,5) 
#define DEV_ADMA0_LINEIN_DECIMATOR_0        DEV_ADMA_CH(0,7) 
#define DEV_ADMA0_LINEIN_DECIMATOR_1        DEV_ADMA_CH(0,9) 
#define DEV_ADMA0_DEV_HQ_LP_INTERP_0        DEV_ADMA_CH(0,0) 
#define DEV_ADMA0_DEV_HQ_LP_INTERP_1        DEV_ADMA_CH(0,2) 

// Devices mapped to ADMA3
#define DEV_ADMA3_I2S_PCM0_TX_L             DEV_ADMA_CH(1,0) //DEVICEn(ADMA,60)   //0
#define DEV_ADMA3_I2S_PCM0_RX_L             DEV_ADMA_CH(1,1) //DEVICEn(ADMA,61)   //1
#define DEV_ADMA3_I2S_PCM0_TX_R             DEV_ADMA_CH(1,2) //DEVICEn(ADMA,62)   //2
#define DEV_ADMA3_I2S_PCM0_RX_R             DEV_ADMA_CH(1,3) //DEVICEn(ADMA,63)   //3
#define DEV_ADMA3_I2S_PCM1_TX_L             DEV_ADMA_CH(1,4) //DEVICEn(ADMA,64)   //4
#define DEV_ADMA3_I2S_PCM1_RX_L             DEV_ADMA_CH(1,5) //DEVICEn(ADMA,65)   //5
#define DEV_ADMA3_I2S_PCM1_TX_R             DEV_ADMA_CH(1,6) //DEVICEn(ADMA,66)   //6
#define DEV_ADMA3_I2S_PCM1_RX_R             DEV_ADMA_CH(1,7) //DEVICEn(ADMA,67)   //7

#define DEV_ADMA3_EARLINK_OUTPUT_CH_0       DEV_ADMA_CH(1,9) //DEVICEn(ADMA,68)   //9
#define DEV_ADMA3_EARLINK_INPUT_CH_0        DEV_ADMA_CH(1,8) //DEVICEn(ADMA,69)   //8
#define DEV_ADMA3_EARLINK_OUTPUT_CH_1       DEV_ADMA_CH(1,11) //DEVICEn(ADMA,70)   //11
#define DEV_ADMA3_EARLINK_INPUT_CH_1        DEV_ADMA_CH(1,10) //DEVICEn(ADMA,71)   //10
#define DEV_ADMA3_EARLINK_OUTPUT_CH_2       DEV_ADMA_CH(1,13) //DEVICEn(ADMA,72)   //13
#define DEV_ADMA3_EARLINK_INPUT_CH_2        DEV_ADMA_CH(1,12) //DEVICEn(ADMA,73)   //12
#define DEV_ADMA3_I2S_PCM1_TX_2       	    DEV_ADMA_CH(1,12) //DEVICEn(ADMA,73)   //12
#define DEV_ADMA3_EARLINK_OUTPUT_CH_3       DEV_ADMA_CH(1,15) //DEVICEn(ADMA,74)   //15
#define DEV_ADMA3_EARLINK_INPUT_CH_3        DEV_ADMA_CH(1,14) //DEVICEn(ADMA,75)   //14
#define DEV_ADMA3_I2S_PCM1_TX_3			    DEV_ADMA_CH(1,14) //DEVICEn(ADMA,75)   //14
#define DEV_ADMA3_I2S_PCM1_TX_4			    DEV_ADMA_CH(1,16)    				  //16
#define DEV_ADMA3_EARLINK_OUTPUT_CH_4       DEV_ADMA_CH(1,17) //DEVICEn(ADMA,77)   //17
#define DEV_ADMA3_I2S_PCM1_TX_5			    DEV_ADMA_CH(1,18)    				  //18
#define DEV_ADMA_DEV_GROUP_INTERRUPTS       DEV_ADMA_CH(1,0x1F)


#define DEV_UART0               DEVICEn(UART, 0)
#define DEV_UART1               DEVICEn(UART, 1)
#define DEV_SAIF0               DEVICEn(SAIF, 0)
#define DEV_SAIF1               DEVICEn(SAIF, 1)
#define DEV_SAIF2               DEVICEn(SAIF, 2)
#define DEV_DMIC0               DEVICE0(DMIC)
#define DEV_GPIO                DEVICE0(GPIO)

#define DEV_MBOX	            DEVICE0(MBOX)

#define DEV_MBOX_0          	DEVICEn(MBOX, 0)
#define DEV_MBOX_1          	DEVICEn(MBOX, 1)
#define DEV_MBOX_2          	DEVICEn(MBOX, 2)
#define DEV_MBOX_3          	DEVICEn(MBOX, 3)
#define DEV_MBOX_4          	DEVICEn(MBOX, 4)
#define DEV_MBOX_5          	DEVICEn(MBOX, 5)

#define DEV_MBOX_ARM            DEV_MBOX_0
#define DEV_MBOX_CAPE_A         DEV_MBOX_1
#define DEV_MBOX_CAPE_B         DEV_MBOX_2
#define DEV_MBOX_LLE_A          DEV_MBOX_3
#define DEV_MBOX_LLE_B          DEV_MBOX_4
#define DEV_MBOX_HIFI3          DEV_MBOX_5

#define DEV_CAMERA              DEVICE0(CAMERA)

#define DEV_BLUETOOTH           DEVICE0(BLUETOOTH)

#define DEV_WIFI                DEVICE0(WIFI)

#define DEV_OSPI                DEVICE0(OSPI)
#define DEV_RTC                 DEVICE0(RTC)
#define DEV_SDIO                DEVICE0(SDIO)

#define DEV_PM_LITE             DEVICE0(PM_LITE)

//  Open mode
#define O_RDONLY                0	// read only access wanted
#define O_WRONLY                1	// write only access wanted
#define O_RDWR                  2	// read and write access wanted
#define O_WREXCL                3	// exclusive write access
#define O_RWMASK                3

#endif // _MCU_DEVICES_H_
