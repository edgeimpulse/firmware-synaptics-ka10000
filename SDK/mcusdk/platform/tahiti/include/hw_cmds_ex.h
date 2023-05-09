////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS, INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Systems Inc.
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
//  Copyright (C) 2020 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HW_CMDS_EX_H_
#define _HW_CMDS_EX_H_

/*
 * HW Drv related Commands Codes.
 */
typedef enum {
  HW_CMD_NONE                           = 0,

  HW_ADC_CMD_PRIVATE                    = 1,
  HW_ADC_CMD_CFG                        = 2,
  HW_ADC_CMD_HPF_CFG                    = 3,
  HW_ADC_CMD_ENABLE                     = 4,
  HW_ADC_CMD_HPF_ENABLE                 = 5,
  HW_ADC_CMD_VOLUME                     = 6,
  HW_ADC_CMD_MUTE                       = 7,
  HW_ADC_CMD_ANC_CFG                    = 8,
  HW_ADC_CMD_ANC_HPF_CFG                = 9,
  HW_ADC_CMD_ANC_ENABLE                 = 10,
  HW_ADC_CMD_ANC_HPF_ENABLE             = 11,

  HW_I2CS_CMD_SLAVE_ADDRESS             = 12,
  HW_UART_CMD_BAUD_RATE                 = 13,

  HW_DAC_CMD_PRIVATE                    = 14,
  HW_DAC_CMD_CFG                        = 15,
  HW_DAC_CMD_CH_ENABLE                  = 16,
  HW_DAC_CMD_VOLUME                     = 17,
  HW_DAC_CMD_MUTE                       = 18,
  // HW_DAC_CMD_SIDETONE_EANBLE            = 19,
  // HW_DAC_CMD_SIDETONE_VOLUME            = 20,
  HW_DAC_CMD_SIDETONE_MUTE              = 21,
  HW_DAC_CMD_SIDETONE_CONFIG            = 22,

  HW_I2S_CMD_I2S_PCM_WIRE_MODE          = 23,
  HW_I2S_CMD_CONFIG_I2S                 = 24,
  HW_I2S_CMD_CONFIG_PCM                 = 25,

  HW_ADMA_CMD_IO_CONFIG                 = 26,
  HW_ADMA_CMD_IO_ENABLE                 = 27,
  HW_ADMA_CMD_CAF_CONTROL               = 28,
  HW_ADMA_CMD_IO_DIRECT_CONNECTION      = 29,
  HW_ADMA_CMD_IO_DISCONNECTION          = 30,

  HW_ELK_TUNNEL_CMD_PORT_CONFIG         = 31,
  HW_ELK_TUNNEL_CMD_PORT_ENABLE         = 32,
  HW_ELK_TUNNEL_CMD_ADMA_ENABLE         = 33,
  HW_ELK_TUNNEL_CMD_STATUS              = 34,

  HW_I2C_TUNNEL_CMD_CONFIG              = 35,
  HW_I2C_TUNNEL_CMD_WRITE               = 36,
  HW_I2C_TUNNEL_CMD_READ                = 37,
  //38,
  //39,
  HW_SYNC_CMD_EVENTS_PACK_AND_CONFIG    = 40,
  HW_SYNC_CMD_EVENT_GROUPS_CHECK_EN     = 41,

  HW_CLOCK_CMD                          = 42,

  HW_LLR_ANC_LATENCY_PATH_CONFIG        = 43,
  HW_LLR_ANC_LATENCY_PATH_ENABLE        = 44,
  
  HW_PWM_CMD_CONFIG                     = 47,
  HW_PWM_CMD_VALUE                      = 48,

  HW_AVAD_CMD_COEFF_CONFIG              = 49,
  HW_AVAD_CMD_THRESHOLD_CONFIG          = 50,
  HW_AVAD_CMD_HPF_CONFIG                = 51,
  HW_AVAD_CMD_GAIN_CONFIG               = 52,
  HW_AVAD_CMD_ENABLE                    = 53,
  
  HW_DAC_CMD_ANC_CONFIG                 = 54,
  HW_DAC_CMD_ANC_ENABLE                 = 55,
  HW_DAC_CMD_ANC_ONLY_ENABLE            = 56,
  HW_I2S_CMD_CH_ENABLE                  = 57,
  HW_DAC_CMD_HPF_CONFIG                 = 58,
  HW_DAC_CMD_HPF_ENABLE                 = 59,

  HW_TIMR_CMD_PROFILE_BCLK              = 60,
  HW_ADC_AVAD_ENABLE                    = 61,
  HW_ADC_AVAD_CFG                       = 62,

  HW_I2S_CMD_MCLK_ENABLE                = 63,
  HW_DAC_CMD_SPK_TEST                   = 64,
  
  HW_I2CS_CMD_LIMIT_ADDRESS             = 65,
  HW_I2CS_CMD_BASE_ADDRESS              = 66,

} HWCmdCode;

#endif //_HW_CMDS_EX_H_
