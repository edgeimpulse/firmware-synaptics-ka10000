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

#ifndef _INTER_IO_BUFFER_H_
#define _INTER_IO_BUFFER_H_

#include "autoconf.h"

#if defined(CONFIG_INTER_IO_DESC_XFER_BUFFERED)

typedef enum IoHandShakeEnumTag {
  HandShakeNone           =  0,
  HandShakeRequest        =  1,
  HandShakeReply          =  2,
  HandShakeError          =  3,
  HandShakeLast
} IoHandShakeEnum;

#if !defined(CONFIG_INTER_PROCESS_BUF_DESC_XFER_STATE_SIZE)
#define CONFIG_INTER_PROCESS_BUF_DESC_XFER_STATE_SIZE        (64U)
#endif

#define _INTER_IO_XFER_BUFFER_CAPACITY         CONFIG_INTER_PROCESS_BUF_DESC_XFER_STATE_SIZE

typedef struct InterIOXferBufferTag {
uint32_t read_idx;
uint32_t write_idx;
void *buf[_INTER_IO_XFER_BUFFER_CAPACITY];
} InterIOXferBuffer;

static inline void inter_io_buf_reset(volatile InterIOXferBuffer *cbuf)
{
    cbuf->write_idx = 0;
    cbuf->read_idx = 0;
}

static inline uint32_t inter_io_buf_capacity(volatile InterIOXferBuffer *cbuf)
{
    return (_INTER_IO_XFER_BUFFER_CAPACITY - 1);
}

static inline uint32_t inter_io_buf_empty_chk(volatile InterIOXferBuffer *cbuf)
{
    return cbuf->write_idx == cbuf->read_idx;
}

static inline uint32_t inter_io_buf_full_chk(volatile InterIOXferBuffer *cbuf)
{
    volatile uint32_t w_idx = cbuf->write_idx + 1;
    if (w_idx >= _INTER_IO_XFER_BUFFER_CAPACITY)
    {
        return cbuf->read_idx == 0;
    }
    else
    {
        return w_idx == cbuf->read_idx;
    }
}

static inline uint32_t inter_io_buf_read_avail_chk(volatile InterIOXferBuffer *cbuf)
{
    uint32_t avail;
    volatile uint32_t w_idx = cbuf->write_idx;
    volatile uint32_t r_idx = cbuf->read_idx;

    if (w_idx >= r_idx)
    {
        avail = 0;
    }
    else
    {
        avail = _INTER_IO_XFER_BUFFER_CAPACITY;
    }

    avail += w_idx - r_idx;
    return avail;
}

static inline uint32_t inter_io_buf_write_avail_chk(volatile InterIOXferBuffer *cbuf)
{
    uint32_t avail;
    volatile uint32_t w_idx = cbuf->write_idx;
    volatile uint32_t r_idx = cbuf->read_idx;

    if (r_idx > w_idx)
    {
        avail = 0;
    }
    else
    {
        avail = _INTER_IO_XFER_BUFFER_CAPACITY;
    }

    avail += r_idx - w_idx - 1;
    return avail;
}

static inline void inter_io_buf_push_no_chk(volatile InterIOXferBuffer *cbuf, void* item)
{
    volatile uint32_t w_idx = cbuf->write_idx;
    cbuf->buf[w_idx++] = item;
    if (w_idx >= _INTER_IO_XFER_BUFFER_CAPACITY)
    {
        cbuf->write_idx = 0;
    }
    else
    {
        cbuf->write_idx = w_idx;
    }
}

static inline void* inter_io_buf_pop_no_chk(volatile InterIOXferBuffer *cbuf)
{
    void * item;
    volatile uint32_t r_idx = cbuf->read_idx;
    item = cbuf->buf[r_idx++];
    if (r_idx >= _INTER_IO_XFER_BUFFER_CAPACITY)
    {
        cbuf->read_idx = 0;
    }
    else
    {
        cbuf->read_idx = r_idx;
    }
    return item;
}

#endif // CONFIG_INTER_IO_DESC_XFER_BUFFERED
#endif // _INTER_IO_BUFFER_H_
