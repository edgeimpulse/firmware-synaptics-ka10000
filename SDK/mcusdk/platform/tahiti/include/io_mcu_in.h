////////////////////////////////////////////////////////////////////////////////
//
//  Synaptics, INC. CONFIDENTIAL AND PROPRIETARY
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
//  Copyright (C) 2020 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __IO_MCU_IN_H__
#define __IO_MCU_IN_H__

#include "io_mcu_ex.h"

#define CMD_SIZE_CAPE2ARM     4
typedef struct McuIoTag
{
  BaseIo    base;
  uint32_t  pre_write_idx;
  int       cmd_buf_idx;
  Command   cmd_cape2arm[CMD_SIZE_CAPE2ARM];
  uint32_t  frame_size;   //From init_data
  uint32_t  sample_width; //From init_data
  uint32_t  read_index;
} McuIo;

#endif  //__IO_MCU_IN_H__