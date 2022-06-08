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
//  Copyright (C) 2018 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTER_PROCESS_H_
#define _INTER_PROCESS_H_

#include "autoconf.h"

// Fixme: Consider bit fields
typedef struct InterProcessBufferDescpTag {
uint16_t         id;                    // IO id
uint8_t          rply;                   // 0 busy, 1 free, 2 HiFi processed
uint8_t          sample_bytes_width;     // bytes per sample
uint16_t         size;                   // Frame Size
uint16_t         descp_id;               // for debug
int32_t          *ptr;                   // AXI address
uint32_t         sig_type;               // stream type according to enum SignalType
#ifdef CONFIG_MCU_IO_PROFILE
uint32_t        cyc_sent;               // for debug: the cycle count when sending to HIFI4.
uint32_t        cyc_diff;               // for debug: the cycle diff compared with cyc_sent when receiving io_desc from HIFI4.
#endif
} InterProcessBufferDescp;

#if defined(CONFIG_INTER_IO_DESC_XFER_BUFFERED)

#define CONFIG_INTER_PROCESS_BUF_DESC_XFER_STATE_SIZE        (64U)

#include "inter_io_buffer.h"

typedef struct InterProcessBufferDescXferStateTag {
  uint32_t send_prev_clk_cycle;
  uint32_t send_delay_count;
  uint32_t send_handshake;
  uint32_t cb_prev_clk_cycle;
  uint32_t cb_delay_count;
  uint32_t cb_handshake;
  InterIOXferBuffer send_state;
  InterIOXferBuffer cb_state;
} InterProcessBufferDescXferState;

#endif // #if defined(CONFIG_INTER_IO_DESC_XFER_BUFFERED)

#define IO_ID_I2S_RX_0  1
#define IO_ID_I2S_TX_0  4


#define IO_ID_HOST_CAPT  3
#define IO_ID_HOST_PLBK  2


typedef enum PowerStateEnumTag {
  PROCESSOR_POWER_OFF            =  0,
  PROCESSOR_POWER_READY          =  0x11111111,

  PROCESSOR_POWER_LAST
} PowerStateEnum;


#endif // _INTER_PROCESS_H_





