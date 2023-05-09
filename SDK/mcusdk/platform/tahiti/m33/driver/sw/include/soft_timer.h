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

#ifndef _MCU_DRV_SOFT_TIMER_H_
#define _MCU_DRV_SOFT_TIMER_H_

#include "mcu.h"

#if (defined(CONFIG_TARGET_TAHITI) || defined(CONFIG_TARGET_KATANA)) && defined(CONFIG_MCU_DRV_SOFT_TIMER)

#define TICKS_PER_SEC	24576000UL
#define TICKS_PER_MSEC	24576UL
#define TICKS_PER_USEC	25UL

// To convert desired limit to CPU Ticks
#define MSEC_2_TICKS(_milisec)	((_milisec)*TICKS_PER_MSEC)
#define USEC_2_TICKS(_milisec)	((_milisec)*TICKS_PER_USEC)

// To get equivalent time from CPU Ticks
#define TICKS_2_MSEC(_milisec)	((_milisec)/TICKS_PER_MSEC)
#define TICKS_2_USEC(_milisec)	((_milisec)/TICKS_PER_USEC)

#define	TRUE	1
#define FALSE	0
#define MAX_SOFT_TIMERS	7

volatile BOOL Softimer_Initiated; // Driver inited flag
typedef int (*stimr_fn_t) ( uint32_t param);

// Context structures for software timers
typedef static struct softimer_s{
	int 		state=0;		// timer status
	uint32_t 	limit;			// timer limit in CPU Ticks
	uint32_t 	elapsed;		// elapsed since start
	int 		source;			// System Timer Source
	stimr_fn_t 	stimr_fn;  		// Soft Timer Callback Function
	uint32_t* 	atimr_addr;		// linked list address
}

enum stimr_flags{
	// Timer Units
	MILLISEC=2,
	MICROSEC,
	
	// Timer States
	NOT_SET,
	SET,
	RUNNING,
	ISSUED,

	// Sources
	GPTC0,
	GPTC1,
	SPTC,
	ARM_TMR0,
	ARM_TMR1,
	ARM_DTMRA,
	ARM_DTMRB,
}


#endif  // CONFIG_MCU_DRV_TIMR
#endif  // _MCU_DRV_SOFT_TIMER_H_ 