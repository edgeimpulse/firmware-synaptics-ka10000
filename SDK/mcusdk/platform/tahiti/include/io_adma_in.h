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
#ifndef __IO_ADMA_IN_H__
#define __IO_ADMA_IN_H__

#define IO_ADMA_MAX_NUM_CHANNELS            (20)
#define MAX_NUM_CHANNELS_PER_IO             8     //Max should be I2STX1 on Tahiti which supported 6 ADMA channels.
                                                  //Temply set it as 8 to WA the strange issue which caused ffv configuration's CAPEB crashed.
#define ADMA_MAJOR_ADJUST(a)                (((MAJOR_ADMA)<<DEV_MAJOR_SHIFT)| ((a)&0xFF))
// Interrupt callback function
typedef void (*io_adma_irq_cb)(void *data);

struct io_adma_callback
{
  io_adma_irq_cb callback;
  void *data;
};

/*
 * IRQ/Tick profiling is only for trigger IO.
 */
typedef struct AdmaIoProfileTag {
/*
 * uint32_t module_id;
 * uint32_t module_type;
 * uint32_t flags;
 * For host to show readable text, upper information should be needed to get 
 * by MODULE_CMD_GET_INIT_DATA for saving memory.
 */
  uint32_t lst_cpu_cycles;            //Each IRQ's timestamp.
                                      //Use adma_io->base.num_object to replace when host GET.
  uint32_t min_irq_interval;          //Min irq interval.
  uint32_t max_irq_interval;          //Max irq interval.
                                      //Min/Max irq interval can be used as profiling hardware or whether missing IRQ occurred.
  uint32_t avg_irq_interval;          //Average irq interval of last 0x8000 times.

  //Use int16_t instead of uint16_t because num_samples is int_t.
  int16_t  min_num_samples;           //Min num_samples of IO
  int16_t  max_num_samples;           //Max num_samples of IO 
  uint32_t total_num_samples;         //total num_samples of IO
  int32_t  rw_distance;               //This is only for output IO since out IO has in_stream.
  int32_t  min_rw_distance;           //Min read/write distance between edge module and IO module.
  int32_t  max_rw_distance;           //Max read/write distance between edge module and IO module.
  uint32_t apply_count;               //The apply count of updating its write_index.
  uint32_t audp_tick_count;           //The count of irq and audp_tick can be used as profiling the jitter between hardware and RTOS
  uint32_t max_jitter;                //Max jitter between HW and RTOS. 
                                      //Can be used as estimate the R/W delay between hardware and edge module.
  uint32_t samples_left;              //Samples left for trigger IO.
} AdmaIoProfile_t;

typedef struct AdmaIoTag
{
  BaseIo base;
  uint32_t num_channels;
  uint32_t ch_index[MAX_NUM_CHANNELS_PER_IO];
  uint32_t adma_dev;
  int32_t  prev_address;
  uint32_t input_status;
  int32_t  group_pair_idx; 
  io_adma_shared_info_t share_info;
  io_adma_ch_reconfig_t ch_reconfig;
  /*  
  * below member uint32_t adma_io_devs[MAX_NUM_CHANNELS_PER_IO] is used for 
  * transfering ADMA dev information from CAPE to M33 side,
  * please note that only low 8 bits are effetive, other higher bits are for MAJOR_ADMA, 
  * since MAJOR_ADMA is different betwen M33 and CAPE, it should be converted when transfer from 
  * one side to the other.
  * */
  uint32_t adma_io_devs[MAX_NUM_CHANNELS_PER_IO];
  uint32_t adma_io_sample_size;
  uint32_t frame_size;
  uint32_t adma_io_remaining_samples;

#if defined(CONFIG_CAPE2_IO_ADMA_PROFILING)
  AdmaIoProfile_t       adma_io_profile;
  uint32_t              min_adma_irq_frame_cycles;
  uint32_t              max_adma_irq_frame_cycles;
  uint32_t              avg_adma_irq_frame_cycles;
#endif
} AdmaIo;

#if defined(CONFIG_CAPE2_IO_ADMA_PROFILING)
void AdmaIoProfileUpdateAudpTick(uint32_t tick);
#endif

#endif  //__IO_ADMA_IN_H__
