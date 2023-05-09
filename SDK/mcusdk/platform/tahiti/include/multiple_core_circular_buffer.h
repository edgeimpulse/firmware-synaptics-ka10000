/*------------------------------------------------------------------------------
  Copyright (C) 2010-2019 Conexant Systems Inc.
  All rights reserved.

  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Conexant Systems, Inc.
  ("Conexant")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Conexant Systems Inc.

  Except as otherwise specifically provided through an express agreement
  with Conexant that governs the confidentiality, possession, use
  and distribution of the information contained in this file, CONEXANT
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: mutiple_core_circular_buffer.h

  Description: Circular Buffer that can runs for multcore, 
               the producer-core controls the write pointer;
               the consumer-core controls the read pointer.               
------------------------------------------------------------------------------*/
#ifndef MULTIPLE_CORE_CIRCULAR_BUFFER_H_
#define MULTIPLE_CORE_CIRCULAR_BUFFER_H_

#include <stdint.h>


typedef void *HANDLE;

#define __stdcall        //signal_stream.h:455: error: expected initializer before 'Q31ToFloat'


#include "signal_stream.h"
#include "io_struct_def.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define CIRCULAR_BUFFER_READ_MODE 1
#define CIRCULAR_BUFFER_WRITE_MODE 2

#define SCRACH_MEMORY_SIZE 300

#define GET_HIFI3_STEREO_BUFFER(stream,index)                                        \
  ( ((int64_t*)(void *)(stream)->buffer_start) + ((short)(index)*((stream)->buffer_size)) )
#define GET_HIFI3_MONO_BUFFER(stream,index)                                          \
  ( (int32_t*)GET_HIFI3_STEREO_BUFFER(stream,                                        \
                                SIGNAL_NUM_STEREO_BUFFERS((stream)->signal_type))\
   + ((short)(index)*((stream)->buffer_size)) )

#define CIRCULAR_BUFFER_RESULT_OK         (0)
#define CIRCULAR_BUFFER_RESULT_RESET      (1)
#define CIRCULAR_BUFFER_RESULT_OVERRUN    (2)
#define CIRCULAR_BUFFER_RESULT_UNDERRUN   (3)

//if both consumer and producer use this struct, can use this to do buffer overflow checking
//#define KEEP_REAL_SIZE 0 

typedef struct Hifi3BufConfigTag {
  int16_t   unused;
  float     wr_delay;
} Hifi3bufConfig;


typedef struct {
  //audio info
  uint32_t          change_param;
  SignalType        signal_type;
  uint32_t          bytes_per_sample;
  uint32_t          write_index;        //read write index
  uint32_t          in_offset;
  uint32_t          prev_index;
  uint32_t          stall_count;
  uint32_t          num_available;
  uint32_t          num_avail_prev;
  uint32_t          num_consumed;
  Hifi3bufConfig    config;
  uint32_t          samples_per_1ms;

  //buffer info
#ifdef KEEP_REAL_SIZE
  volatile uint32_t m_count;
#endif

  volatile int32_t buffer_size;       //how many mili-second this circular buffer can hold

  int32_t          *buffer_start;   //must be in AHB space
  
#ifdef SIMULATION
  HANDLE atomic_add_mutex;
#endif
  
} CircularBuffer;


#if 0
/**
 * multiple_core_circular_buffer.h
 * 
 * <pre>
CircularBuffer * CircularBufferCreate(IOInterfaceConfig *init_data);
 * </pre>
 * 
 * @param init_data [in] Initial parameter for the CircularBuffer.
 * @return CircularBuffer*-  Pointer to created CircularBuffer. If failed to create, return NULL.
 * 
 * \defgroup CircularBufferCreate CircularBufferCreate
 * \ingroup Programming_API_CircularBuf
 */
CircularBuffer * CircularBufferCreate(IOInterfaceConfig *init_data);
#endif
/**
 * multiple_core_circular_buffer.h
 * 
 * <pre>
void CircularBufferReset(CircularBuffer *p_cir_buf);
 * </pre>
 * 
 * @param p_cir_buf [in] The circular buffer instance
 * 
 * \defgroup CircularBufferReset CircularBufferReset
 * \ingroup Programming_API_CircularBuf
 */
void CircularBufferReset(CircularBuffer *p_cir_buf);

/**
 * multiple_core_circular_buffer.h
 * 
 * <pre>
int CircularBufferPrepare(CircularBuffer *p_cir_buf, int32_t mode, void *p_buf, uint32_t sample_cnt, void *p_scratch);
 * </pre>
 * 
 * @param p_cir_buf [in] The circular buffer instance
 * @param mode [in] write or read mod
 * @param p_buf [out] returned buffer that contains input data in read mode or buffer to write in write mode
 * @param sample_cnt [in] sample counter to read or write
 * @param p_scratch [in] scratch memory if partial frame happens, so p_buf is always a linear buffer 
 * @return int always 0 in current stage
 * 
 * \defgroup CircularBufferPrepare CircularBufferPrepare
 * \ingroup Programming_API_CircularBuf
 */
int CircularBufferPrepare(CircularBuffer *p_cir_buf, int32_t mode, void **p_buf, uint32_t sample_cnt, void *p_scratch);

/**
 * multiple_core_circular_buffer.h
 * 
 * <pre>
int CircularBufferFinish(CircularBuffer *p_cir_buf, int32_t mode, uint32_t sample_cnt, void *p_scratch);
 * </pre>
 * 
 * @param p_cir_buf [in] The circular buffer instance
 * @param mode [in] write or read mod
 * @param sample_cnt [in] sample counter to read or write
 * @param p_scratch [in] scratch memory if partial frame happens, this buffer will be copied into the circular buffer
 * @return int always 0 in current stage
 * 
 * \defgroup CircularBufferFinish CircularBufferFinish
 * \ingroup Programming_API_CircularBuf
 */
int CircularBufferFinish(CircularBuffer *p_cir_buf, int32_t mode, uint32_t sample_cnt, void *p_scratch);

#ifdef __cplusplus
}
#endif

#endif  // MULTIPLE_CORE_CIRCULAR_BUFFER_H_
