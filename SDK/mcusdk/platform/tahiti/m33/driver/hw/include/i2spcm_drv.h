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

#ifndef _DRV_I2SPCM_H_
#define _DRV_I2SPCM_H_

#if defined(CONFIG_MCU_DRV_I2S_PCM)

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_I2S_MAX_DEV_NUM 	2

#define _STATIC_INLINE                    static inline

#define I2S_PCM_BIT768 768
#define I2S_PCM_BIT512 512
#define I2S_PCM_BIT384 384
#define I2S_PCM_BIT256 256
#define I2S_PCM_BIT128 128
#define I2S_PCM_BIT64 64
#define I2S_PCM_BIT48 48
#define I2S_PCM_BIT32 32
#define I2S_PCM_BIT16 16


#define I2S_SET_MUTE  1
#define I2S_GET_MUTE  0

#define I2S_MUTE_ENABLE  1
#define I2S_MUTE_DISABLE 0

#define I2S_DIRECTION_TX 0
#define I2S_DIRECTION_RX 1

#define I2S_PCM_MAX_TX_CHANNEL_NUM 6

#define MCLK_START_ASYNC		0
#define MCLK_START_I2S0 		1
#define MCLK_START_I2S1 		2

typedef struct i2s_config_s
  {
  uint tx_rx;
#define I2S_PCM_TX	0
#define I2S_PCM_RX	1

  uint sample_rate;          // 0 for slave mode, else master mode
#define I2S_PCM_SAMPLE_RATE_SLAVE   0

  uint justified;
#define I2S_JUSTIFIED_LEFT    0 // data appears before filler
#define I2S_JUSTIFIED_RIGHT   1 // data appears after  filler

  uint dstart_delay;
#define I2S_DELAY_0_BIT       0 // 0 bit delay
#define I2S_DELAY_1_BIT       1 // 1 bit delay

  uint lrclk_pol;
#define I2S_LRCLK_POL_LEFT_LOW    1 // left channel when L/R clock is low
#define I2S_LRCLK_POL_LEFT_HIGH   0 // left channel when L/R clock is high

  uint lrck_low_first;
#define I2S_LRCK_FIRST_HIGH        0  // High level pulse comes out first
#define I2S_LRCK_FIRST_LOW         1  // Low level pulse comes out first
  uint rx_en_by_sync_module;

  } i2s_config_t;


typedef struct pcm_config_s
  {
  uint tx_rx;
  uint sample_rate; // 0 for slave mode
  uint sync_pulse_length; // 0 for short-frame mode
#define PCM_SHORT_FRAME 0
#define PCM_LONG_FRAME  1
  uint data_edge;	//data latched on 0=Positive edge of BCLK 1=negative edge of BCLK (default)
#define PCM_POS_EDGE    0
#define PCM_NEG_EDGE	  1
  uint rx_en_by_sync_module ;
  } pcm_config_t;

typedef struct i2s_pcm_en_s
{
	uint tx_rx;
#define I2S_PCM_TX	0
#define I2S_PCM_RX	1
  uint sample_width;
#define I2S_PCM_SAMPLE_WIDTH__8_BIT 0
#define I2S_PCM_SAMPLE_WIDTH_16_BIT 1
#define I2S_PCM_SAMPLE_WIDTH_24_BIT 2
#define I2S_PCM_SAMPLE_WIDTH_32_BIT 3
  uint framelength;	// frame lenght in bits
  uint channels;	// 0=disable, 1=Enable
  uint flag; 
#define I2SPCM_EN_NO_FLAG           0      //means don't do reset, and i2s channels will be enabled     
#define I2SPCM_EN_FLAG_WITH_RST_RX  1      //when this bit is set, means reset rx     
#define I2SPCM_EN_FLAG_WITH_RST_TX  2      //when this bit is set, means reset tx
#define I2SPCM_EN_FLAG_WITHOUT_EN   4      //when this bit is set, means don't enable i2s rx/tx channels, but it will support the i2s fifo full mechanism, when fifo full, adma will bloacked
}i2s_pcm_en_t;



int drv_i2spcm_start(void);

struct i2spcm_ops_s
  {
  DECLARE_STANDARD_DEVICE_API

  int (*config_i2s) (uint dev, i2s_config_t *config);
  int (*config_pcm) (uint dev, pcm_config_t *config);

  int (*enable_i2s_pcm)(uint dev, i2s_pcm_en_t *config);

  int (*set_wire_mode) (uint dev, uint wire_mode);
// Tahiti definitions
#define I2S_PCM_4_WIRE_RX_SHARE_LRCK_MODE 1
#define I2S_PCM_4_WIRE_TX_SHARE_LRCK_MODE 2
#define I2S_PCM_5_WIRE_MODE 			  3
#ifdef CONFIG_MCU_DRV_I2S_PCM_SUPPORT_GET_COMMAND
  int (*get_wire_mode) (uint dev);
#endif
  int (*mute) (uint dev, uint direction, uint mute_en, uint flag);
  int (*mclk_enable)(uint mclk_freq, uint polarity, uint mclk_start);
  };

//#if defined(_SOS_H_INCLUDED_)
//
//  Interface function to the driver
//
_STATIC_INLINE struct i2spcm_ops_s *i2s_pcm_driver_info(uint dev)
  {
  return (struct i2spcm_ops_s *)os_get_driver_info(dev);
  }

_STATIC_INLINE int i2s_pcm_config_i2s (uint dev, i2s_config_t *config)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->config_i2s(dev, config);
  return -ENODEV;
  }

_STATIC_INLINE int i2s_pcm_config_pcm (uint dev, pcm_config_t *config)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->config_pcm(dev, config);
  return -ENODEV;
  }

_STATIC_INLINE int i2s_pcm_enable (uint dev, i2s_pcm_en_t *config)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->enable_i2s_pcm(dev, config);
  return -ENODEV;
  }

_STATIC_INLINE int i2s_pcm_set_wire_mode(uint dev, uint wire_mode)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->set_wire_mode(dev, wire_mode);
  return -ENODEV;
  }

#ifdef CONFIG_MCU_DRV_I2S_PCM_SUPPORT_GET_COMMAND
_STATIC_INLINE int i2s_pcm_get_wire_mode(uint dev)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->get_wire_mode(dev);
  return -ENODEV;
  }
#endif

_STATIC_INLINE int i2s_pcm_set_mute(uint dev, uint direction, uint mute_en)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->mute(dev, direction, mute_en, I2S_SET_MUTE); 
  return -ENODEV; 
  }

#ifdef CONFIG_MCU_DRV_I2S_PCM_SUPPORT_GET_COMMAND
_STATIC_INLINE int i2s_pcm_get_mute(uint dev, uint direction)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->mute(dev, direction, 0, I2S_GET_MUTE);
  return -ENODEV;
  }
#endif

_STATIC_INLINE int i2s_mclk_enable(uint dev, uint mclk_freq, uint polarity,uint mclk_start)
  {
  struct i2spcm_ops_s *op;
  if ((op=i2s_pcm_driver_info(dev)) != NULL)
    return op->mclk_enable(mclk_freq, polarity, mclk_start);
  return -ENODEV;
  }

#ifdef __cplusplus
}
#endif

#endif // #ifdef CONFIG_MCU_DRV_I2S

#endif //_DRV_I2SPCM_H_
