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

#ifndef _SWPWM_DRV_H_
#define _SWPWM_DRV_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_MCU_DRV_SWPWM

#define _RGB(R, G, B)                           ((R&0xFF)|((G&0xFF)<<8)|((B&0xFF)<<16))
#define _RGB_GP(GP_PWM0, GP_PWM1, GP_PWM2)      _RGB(GP_PWM0,  GP_PWM1,  GP_PWM2)
#define DONT_USE_GP     MAX_GPIOs

/**
*
* @fn       int swpwm_conf( uint32_t rgb_gp );
* @brief 	Configure GPIO for Soft PWM 
* @param 	rgb_gp			GPIO number in format 0xBBGGRR
* @return 	0 on success or negative error code
*/
int swpwm_conf( uint32_t rgb_gp );

/**
*
* @fn       int swpwm_set_color( uint32_t rgb_color )
* @brief 	Request GPIO for Irq and register callback to gpio_irq_handler 
* @param 	rgb_color			RGB color to display by PWM LEDs in format 0xBBGGRR
* @return 	0 on success or negative error code
*/
int swpwm_set_color( uint32_t rgb_color );

//
//  Driver's exported functions
//
struct swpwm_ops_s
  {
  DECLARE_STANDARD_DEVICE_API;
  };

//
//  Interface function to the driver
//
STATIC_INLINE struct swpwm_ops_s *swpwm_driver_info(uint dev)
  {
  return (struct swpwm_ops_s *)os_get_driver_info(dev);
  }

int drv_swpwm_start(void);

#endif // CONFIG_MCU_DRV_SWPWM

#ifdef __cplusplus
}
#endif

#endif // _MADC_DRV_H_

