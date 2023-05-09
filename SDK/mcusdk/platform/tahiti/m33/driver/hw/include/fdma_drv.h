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

#ifndef _MCU_DRV_FDMA_H_
#define _MCU_DRV_FDMA_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_MCU_DRV_FDMA

#include "../fdma/fdma_hw.h"

#define FDMA_NO_FLAG                        0x0
#define FDMA_FLAG_SET                       0x80000000
#define FDMA_FLAG_MODE_MASK                 0x7
#define FDMA_SAMPLE_TYPE_8D_24Z             0    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xA1000000, 0xD4000000 ..] , 1:1
#define FDMA_SAMPLE_TYPE_16D_16Z            1    //[0xA1B2C300, 0xC4E5F601 ..]  -> [0xA1B20000, 0xD4E50000 ..] , 1:1
#define FDMA_SAMPLE_TYPE_24D_8Z             2    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xA1B2C300, 0xD4E5F600 ..] , 1:1 
#define FDMA_SAMPLE_TYPE_32D                3    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xA1B2C300, 0xD4E5F601 ..] , 1:1 
#define FDMA_SAMPLE_TYPE_PACK_8D_8Z_8D_8Z   4    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xD400A100, 0xD400A100 ..] , 2:1
#define FDMA_SAMPLE_TYPE_PACK_16D_16D       6    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xD4E5A1B2, 0xD4E5A1B2 ..] , 2:1

#define FDMA_SAMPLE_TYPE_UNPACK_8D_24Z      5    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xC3000000, 0xA1000000, 0xF6000000..],  1:2
#define FDMA_SAMPLE_TYPE_UNPACK_16D_16Z     7    //[0xA1B2C300, 0xD4E5F601 ..]  -> [0xC3000000, 0xA1B20000, 0xF6010000..],  1:2

typedef void (*fdma_irq_cb)(void *data);

//
//  Driver's exported functions
//
struct fdma_ops_s
{
    DECLARE_STANDARD_DEVICE_API;
    int  (*set_irq_cb)(uint dev, int pair, fdma_irq_cb callback, void *data);
    fdma_channel_t *(*fdma_get_slot_ctrl)(uint dev);
    int  (*enable)(uint dev,int enable);
    int  (*reset)(uint dev);
    int  (*status)(uint dev);
    int  (*oneshot)(uint dev,uint shot);
    int  (*m2m_transfer)(uint dev,dmaaddr_t dst,dmaaddr_t src,dwsize_t len, int flag);
    int  (*m2p2m_transfer)(uint dev,uint wdt,dmaaddr_t src,dmaaddr_t io,dmaaddr_t dst,dwsize_t len);
    int  (*enable_req)(uint dev,int enable);
    int  (*start_transfer_and_wait)(uint dev,fdma_hw_reg volatile *dma_hw,uint chn,uint flg);
    int  (*chan_cnfg)(uint dev, dmaaddr_t dst,dmaaddr_t src,dwsize_t len);
    int  (*par_transfer)(uint dev,uint chn_flg,uint flg);
};


STATIC_INLINE struct fdma_ops_s *fdma_driver_info (uint dev)
{
    return (struct fdma_ops_s *)os_get_driver_info(dev);
}
  
STATIC_INLINE int fdma_set_irq_cb(uint dev, int pair, fdma_irq_cb callback, void *data) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->set_irq_cb(dev, pair, callback, data);
    return -ENODEV;
}

STATIC_INLINE fdma_channel_t * fdma_get_slot_ctrl(uint dev) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->fdma_get_slot_ctrl(dev);
    return (fdma_channel_t *)NULL;
}

STATIC_INLINE int fdma_enable(uint dev, int enable) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->enable(dev, enable);
    return -ENODEV;
}

STATIC_INLINE int fdma_enable_req(uint dev, int enable) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->enable_req(dev, enable);
    return -ENODEV;
}

STATIC_INLINE int fdma_reset(uint dev) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->reset(dev);
    return -ENODEV;
}

STATIC_INLINE int fdma_status(uint dev) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->status(dev);
    return -ENODEV;
}

STATIC_INLINE int fdma_oneshot(uint dev,uint shot) 
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->oneshot(dev, shot);
    return -ENODEV;
}

STATIC_INLINE int fdma_m2m_transfer(uint dev, dmaaddr_t dst, dmaaddr_t src, dwsize_t len, int flag)
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->m2m_transfer(dev, dst, src, len, flag);
    return -ENODEV;
}

STATIC_INLINE int fdma_m2p2m_transfer(uint dev,dwsize_t wdt,dmaaddr_t src,dmaaddr_t io,dmaaddr_t dst,dwsize_t len)
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->m2p2m_transfer(dev, wdt, src, io, dst, len);
    return -ENODEV;
}

STATIC_INLINE int fdma_start_transfer_and_wait(uint dev, fdma_hw_reg volatile *dma_hw, uint chn,uint flg)
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->start_transfer_and_wait(dev, dma_hw, chn, flg);
    return -ENODEV;
}

STATIC_INLINE int fdma_chan_cnfg(uint dev, dmaaddr_t dst, dmaaddr_t src, dwsize_t len)
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->chan_cnfg(dev, dst, src, len);
    return -ENODEV;
}

STATIC_INLINE int fdma_par_transfer(uint dev, uint chn_flg,uint flg)
{
    struct fdma_ops_s *op;
    if ((op = fdma_driver_info(dev)) != NULL)
        return op->par_transfer(dev, chn_flg, flg);
    return -ENODEV;
}

int fdma_request_channel(uint dev);
int fdma_release_channel(uint dev);

int drv_fdma_start(void);
 
#endif // #ifdef CONFIG_MCU_DRV_FDMA

#ifdef __cplusplus
}
#endif

#endif // _MCU_DRV_FDMA_H_
