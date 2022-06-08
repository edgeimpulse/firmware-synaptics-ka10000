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

#ifndef _DRV_ADMA_H_
#define _DRV_ADMA_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_MCU_DRV_ADMA

// Interrupt callback function
typedef void (*adma_irq_cb)(void * data);

struct adma_callback
{
	adma_irq_cb    callback;
	void          *data;
};

typedef enum
{
	CHANNEL_ENABLE,							// Enable channel
	CHANNEL_DISABLE,						// Disable channel
	SET_CHANNEL_CONTROL,						// Store channel control information
	GET_CHANNEL_CONTROL,
	CHANNEL_ENABLE_INTERRUPT,				// Enable interrupt of physical channel
	CHANNEL_DISABLE_INTERRUPT,				// disable interrupt of physical channel
	CHANNEL_CURRENT_ADDRESS,				// Channel current address
	CHANNEL_FIFO_STATUS,					// Channel FIFO status
	CHANNEL_GROUP_INTERRUPT,				// Group several channels into one interrupt
} eADMA_CMD;


#define   ADMA_CUR_ADDRESS    0
#define   ADMA_FIFO_STATUS    1

#define   OUTPUT_CH_INT_STATUS_OFFSET   10
#define   OUTPUT_CH_INT_ENABLE_OFFSET   16

#define   ADMA0_MAX_INPUT_GROUP (8)
#define   ADMA3_MAX_INPUT_GROUP (10)
#define   OFFSET_FOR_ODD_GROUP  (16) 
#define   GROUP_MASK            (0xFFFF) 



#define   FLAG_GET   1
#define   FLAG_SET   2
#define   FLAG_ADMA_CH_EN_BY_SYNC_MODULE 4

#define		CHANNEL_0	0
#define		CHANNEL_1	1
#define		CHANNEL_2	2
#define		CHANNEL_3	3
#define		CHANNEL_4	4
#define		CHANNEL_5	5
#define		CHANNEL_6	6
#define		CHANNEL_7	7
#define		CHANNEL_8	8
#define		CHANNEL_9	9
#define		CHANNEL_10	10
#define		CHANNEL_11	11
#define		CHANNEL_12	12
#define		CHANNEL_13	13
#define		CHANNEL_14	14
#define		CHANNEL_15	15
#define		CHANNEL_16	16
#define		CHANNEL_17	17
#define		CHANNEL_18	18
#define		CHANNEL_19	19
#define		CHANNEL_20	20

typedef union {
	struct {
		hwint32 start_address : 32;	/* 0:21 sw=RW hw=RO 0x0  */
//		ulong reserved22 : 10;	/* 22:31 sw=RO hw=RO 0x0  */
	} bf;
	hwint32 dw;
} ADMA_START_ADDRESS;

typedef union {
	struct {
		hwint32 buffer_size_low_part : 15;	/* 0:14 sw=RW hw=RO 0x0, B0 increase the buffer size to 18bit, another 3 bit is in other register*/  
		hwint32 samp_size : 3;	/* 15:17 sw=RW hw=RO 0x0  */
		hwint32 s_cnt : 7;	/* 18:24 sw=RW hw=RO 0x0  */
		hwint32 step_ofst : 3;	/* 25:27 sw=RW hw=RO 0x0  */
		hwint32 address_direction : 1;	/* 28:28 sw=RW hw=RO 0x0  */
		hwint32 burst_size : 1;	/* 29:29 sw=RW hw=RO 0x0  */
		hwint32 block_interrupt : 1;	/* 30:30 sw=RW hw=RO 0x0  */
		hwint32 reserved31 : 1;	/* 31:31 sw=RO hw=RO 0x0  */	
	} bf;
	hwint32 dw;
} adma_ctrl;


typedef struct {
	hwint32 buffer_size;
	adma_ctrl ctrl;
} adma_channel_ctrl;




// use tahiti_register_ahb.h to access ADMA 
#define ADMA0_BASE		   DMA0_CHANNEL_ENABLE_ADDRESS
#define ADMA3_BASE		   DMA3_CHANNEL_ENABLE_ADDRESS
#define ADMA_CH_BUFFER_SIZE_MASK         DMA0_CONTROL_CH0_BUFFER_SIZE_MASK
#define ADMA_CH_EXTEND_BUFFER_SIZE_MASK  DMA0_CONTROL_EXT_CH0_WRITEMASK
#define ADMA_CH_BUFFER_SIZE_OFFSET       DMA0_CONTROL_CH0_SAMP_SIZE_OFFSET

#define ADMA_MAX_NUM_MODULES	2		// ADMA0, ADMA3
#define ADMA_MAX_NUM_CHANNELS	20

#define ADMA_CH_ENABLE  1
#define ADMA_CH_DISABLE 0

#define ADMA_INT_ENABLE 1
#define ADMA_INT_DISABLE 0


#define ADMA_MAX_BUFFER_SIZE          (1 << 15)

#define ADMA_SAMPLE_SIZE_8D_24Z       0
#define ADMA_SAMPLE_SIZE_8D_8Z_8D_8Z  1
#define ADMA_SAMPLE_SIZE_16D_16Z      2
#define ADMA_SAMPLE_SIZE_16D_16D      3
#define ADMA_SAMPLE_SIZE_24D_8Z       4
#define ADMA_SAMPLE_SIZE_32D          6

#define ADMA_ADDR_DIR_DECR       0
#define ADMA_ADDR_DIR_INCR       1

#define ADMA_BURST_SIZE_1        0
#define ADMA_BURST_SIZE_4        1

#define ADMA_BLOCK_INTERRUPT_N	 0
#define ADMA_BLOCK_INTERRUPT_Y	 1


#define ADMA_MODULE_0		0
#define ADMA_MODULE_3		1

#define	ADMA_FIFO_OK        0
#define	ADMA_FIFO_OVERRUN   1
#define	ADMA_FIFO_UNDERRUN  2
													//       FROM  I2S					 	 ADMA 
/*
	Note:
	DMACH_CTRL - Bit 31
	Earlink port2 and I2S1 channel 2 share the same audio dma channel, 
	Earlink port3 and I2S1 channel 3 also share the same the audio channel.
	1: I2S channel 2 and 3 are connected to audio dma
	0: Earlink port2 and 3 are connected to audio dma.

*/

int drv_adma_start(void);

struct adma_ops_s
{
	DECLARE_STANDARD_DEVICE_API
	int  (*set_irq_cb)(uint32_t ndevs, const uint32_t *devs, struct adma_callback *cb, int group_en );
	int  (*channel_ctrl)(uint32_t dev, adma_channel_ctrl * chn_ctrl, ADMA_START_ADDRESS *start_address, uint32_t flag);
	int  (*enable)(uint32_t ndevs, const uint32_t *devs, int enable);
	int  (*get_current_addr)(uint32_t dev, int32_t *current_address);
#ifdef CONFIG_MCU_DRV_ADMA_SUPPORT_GET_COMMAND
	int  (*get)(uint32_t dev, uint32_t cmd);
#endif
};


static inline struct adma_ops_s *adma_driver_info (uint32_t dev)
{
	return (struct adma_ops_s *)os_get_driver_info (dev);
}


STATIC_INLINE int adma_set_irq_cb (uint32_t ndevs, const uint32_t *devs, struct adma_callback *cb, int group_en )
{
	struct adma_ops_s *op = adma_driver_info(devs[0]);      //todo

	if (op==NULL)
	{
	  return -ENODEV;
	}

	return op->set_irq_cb(ndevs, devs, cb, group_en );
}


STATIC_INLINE int adma_set_channel_ctrl (uint32_t dev, adma_channel_ctrl * chn_ctrl, ADMA_START_ADDRESS *start_address)
{
	struct adma_ops_s *op;

	if ((op=adma_driver_info(dev))!=NULL)
	{
		return op->channel_ctrl(dev, chn_ctrl, start_address, FLAG_SET);   //1 set
	}

	return -ENODEV;
}

#ifdef CONFIG_MCU_DRV_ADMA_SUPPORT_GET_COMMAND
STATIC_INLINE int adma_get_channel_ctrl (uint32_t dev, adma_channel_ctrl * chn_ctrl, ADMA_START_ADDRESS *start_address)
{
	struct adma_ops_s *op;

	if ((op=adma_driver_info(dev))!=NULL)
	{
		return op->channel_ctrl(dev, chn_ctrl, start_address, FLAG_GET);   //0 get
	}

	return -ENODEV;
}

STATIC_INLINE int adma_get (uint32_t dev, uint32_t cmd)
{
	struct adma_ops_s *op;

	if ((op=adma_driver_info(dev))!=NULL)
	{
		return op->get(dev, cmd);
	}

	return -ENODEV;
}
#endif


STATIC_INLINE int adma_enable (uint32_t ndevs, const uint32_t *devs, int enable)
{
	struct adma_ops_s *op;

	if ((op=adma_driver_info(devs[0]))!=NULL)
	{
		return op->enable(ndevs, devs, enable);
	}

	return -ENODEV;
}

STATIC_INLINE int adma_get_current_addr (uint32_t dev, int32_t *current_address)
{
	struct adma_ops_s *op;

	if ((op=adma_driver_info(dev))!=NULL)
	{
		return op->get_current_addr(dev, current_address);
	}

	return -ENODEV;
}

#endif // #ifdef CONFIG_MCU_DRV_ADMA

#ifdef __cplusplus
}
#endif

#endif  // _DRV_ADMA_H_
