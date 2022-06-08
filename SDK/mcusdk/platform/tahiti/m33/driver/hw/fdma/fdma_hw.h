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

#ifndef _MCU_HW_FDMA_H_
#define _MCU_HW_FDMA_H_

#include "mcu.h"
#include "tahiti_register_ahb.h"

#define FDMA0_BASE  FLEXI_DMA0_s_BASE_ADDRESS
#define FDMA1_BASE  FLEXI_DMA1_s_BASE_ADDRESS

#define FDMA_STAT_INT_MASK	0xFF000000
#define FDMA_EN_BIT       0
#define FDMA_REQ_EN_BIT   8
#define FDMA_IRQ_EN_BIT   16
#define FDMA_CLR_INT_BIT  24

#define DMA_SLOT_CTRL_BUFFER_SIZE_SRC       0
#define DMA_SLOT_CTRL_BUFFER_SIZE_DEST      10
#define DMA_SLOT_CTRL_STEP_OFST_SRC         20
#define DMA_SLOT_CTRL_BURST_SIZE            23
#define DMA_SLOT_CTRL_STEP_OFST_DEST        24
#define DMA_SLOT_CTRL_ADDRESS_DIRECTION     27
#define DMA_SLOT_CTRL_SAMP_SIZE             28
#define DMA_SLOT_CTRL_BLOCK_INTERRUPT       31

#define FDMA_BURST_SIZE_1        0
#define FDMA_BURST_SIZE_4        1
#define FDMA_ADDR_DIR_DECR       0
#define FDMA_ADDR_DIR_INCR       1

#define FDMA_DST_STEP_OFFSET_NONE			0
#define FDMA_SRC_STEP_OFFSET_NONE			0

// The interleaving feature ONLY works on burst of 1
#define FDMA_DST_STEP_OFFSET_1				1
#define FDMA_DST_STEP_OFFSET_2				2
#define FDMA_DST_STEP_OFFSET_3				3
#define FDMA_DST_STEP_OFFSET_4				4
#define FDMA_DST_STEP_OFFSET_5				5
#define FDMA_DST_STEP_OFFSET_6     			6
#define FDMA_DST_STEP_OFFSET_7				7

#define FDMA_SRC_STEP_OFFSET_1				1
#define FDMA_SRC_STEP_OFFSET_2				2
#define FDMA_SRC_STEP_OFFSET_3				3
#define FDMA_SRC_STEP_OFFSET_4				4
#define FDMA_SRC_STEP_OFFSET_5				5
#define FDMA_SRC_STEP_OFFSET_6				6
#define FDMA_SRC_STEP_OFFSET_7				7

#define FDMA_IRQ_BLOCKING       1
#define FDMA_IRQ_NONBLOCKING    0


#define FDMA_CTRL(ssiz,dsiz,ssof,bsiz,dofs,dir,tsiz,birq)  \
		((((dword)(ssiz))&0x3ff)<<DMA_SLOT_CTRL_BUFFER_SIZE_SRC)|\
		((((dword)(dsiz))&0x3ff)<<DMA_SLOT_CTRL_BUFFER_SIZE_DEST)|\
		((((dword)(ssof))&7)<<DMA_SLOT_CTRL_STEP_OFST_SRC)|\
		((((dword)(bsiz))&1)<<DMA_SLOT_CTRL_BURST_SIZE)|\
		((((dword)(dofs))&7)<<DMA_SLOT_CTRL_STEP_OFST_DEST)|\
		((((dword)(dir) )&1)<<DMA_SLOT_CTRL_ADDRESS_DIRECTION)|\
		((((dword)(tsiz))&7)<<DMA_SLOT_CTRL_SAMP_SIZE)|\
		((((dword)(birq))&1)<<DMA_SLOT_CTRL_BLOCK_INTERRUPT)

typedef struct fdma_channel_s
{
    FLEXI_DMA0_SRC_ADDR_CH0_s  flexi_dma_src_addr;
    FLEXI_DMA0_DST_ADDR_CH0_s  flexi_dma_dst_addr;
    FLEXI_DMA0_CTRL_CH0_s  flexi_dma_ctrl;
    hwint8 filler1[0x4];
}fdma_channel_t;

typedef struct _fdma_hw_reg_s {
	FLEXI_DMA0_ENABLE_s  flexi_dma_enable;
	hwint8 filler0[0xc];
  	fdma_channel_t channel[8]; 
	FLEXI_DMA0_PARIS_SRST_s  flexi_dma_paris_srst;
	FLEXI_DMA0_FIFO_STATUS_s  flexi_dma_fifo_status;

} fdma_hw_reg;

typedef  FLEXI_DMA0_ENABLE_s FLEXI_DMA_ENABLE_s;


// Flexi-DMA callback function

#if 1

	#define INSTRUCTION_BARRIER
#endif // #if 1/0

#ifdef CONFIG_MCU_UNIT_TESTS
	#define M2M_DBG_STRESS_TEST 	1
#endif

#endif  // _MCU_HW_FDMA_H_
