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
/**
 * \file madc_drv.h
 * \brief Monitor ADC API
 * \defgroup MADC_API Monitor ADC
 * \ingroup HW_DRV_API
 * @{
 *
 *
 * madc_drv.h
 * 
 * \section Introduction
 * Monitoring ADC with 10-bit resolution samples each pin every 15 msec.
 * 
 * @code
 * //Available MADC devices
 * #define DEV_VBAT			DEVICEn(MADC,0)  //Battery sense
 * #define DEV_TMPSNS			DEVICEn(MADC,1)  //Temperature sense
 * #define DEV_MBSNS			DEVICEn(MADC,2)  //Button sense
 * @endcode
 *
 * \section Usage
 * Programming sequence to sample temperature sense pin\n
 * 
 * \b Step 1:
 * Call os_open(DEV_TMPSNS) to open device driver.
 * 
 * \b Step 2: 
 * Call madc_get(DEV_TMPSNS) sample the temperature sense pin
 *
 * \b Step 3:
 * Call os_close(DEV_TMPSNS) to close MADC device.
 *
 * \section Example
 * Refer to usr_demo.c under mcusdk/app/demo for more detail example code.
 *
 *
 */
 /** @} */  
#ifndef _MADC_DRV_H_
#define _MADC_DRV_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_MCU_DRV_MADC


int drv_madc_start(void);

//
//  Driver's exported functions
//
struct madc_ops_s
  {
  DECLARE_STANDARD_DEVICE_API;
  int (*_get)(uint32_t dev);
  };

struct madc_t
{
    uint32_t pulse_rcosc_lp;
    uint32_t madc_curr_value;
} madc;

//
//  Interface function to the driver
//
STATIC_INLINE struct madc_ops_s *madc_driver_info(uint dev)
  {
  return (struct madc_ops_s *)os_get_driver_info(dev);
  }

STATIC_INLINE int madc_get(uint32_t dev)
  {
  struct madc_ops_s *op;
  if ((op=madc_driver_info(dev))!=NULL)
    {
      return op->_get(dev);
    }
  return -ENODEV;
  }
  
#endif // CONFIG_MCU_DRV_MADC

#ifdef __cplusplus
}
#endif

#endif // _MADC_DRV_H_

