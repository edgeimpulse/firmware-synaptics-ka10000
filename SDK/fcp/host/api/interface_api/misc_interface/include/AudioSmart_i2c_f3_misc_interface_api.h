/*------------------------------------------------------------------------------
  Copyright (C) 2020 Synaptics Inc.
  All rights reserved.

  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Synaptics Inc.
  ("Synaptics")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Synaptics Inc.

  Except as otherwise specifically provided through an express agreement
  with Synaptics that governs the confidentiality, possession, use
  and distribution of the information contained in this file, SYNAPTICS
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: AudioSmart_i2c_f3_misc_interface_api.h
  
------------------------------------------------------------------------------*/
#ifndef _AudioSmart_I2C_F3_MISC_API_H_
#define _AudioSmart_I2C_F3_MISC_API_H_
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \fn int32_t AudioSmart_i2c_interface_f3_event_handler_ctrl (bool ForceBooted)
 * \brief This function control Freeman3 event handler
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_f3_event_handler_ctrl(bool Enable);

#ifdef __cplusplus
}
#endif
#endif