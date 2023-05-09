/*------------------------------------------------------------------------------
  Copyright (C) 2018 Synaptics Inc.
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

  File Name: AudioSmart_sendcmd.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_sendcmd.h
 * \brief This file contains the mid-level SendCmd API for communicating with
 *        Synaptics Audio DSP.
 *
 * \note
 * - When the USB<->I2C connection is provided through CX31993, please link in the
 *   libsendcmd_i2c-f3.lib and \ref USBInterfaceAPI
 *
 * \addtogroup SendCmdAPI
 * @{
 *
 */
#pragma once
#ifndef AudioSmart_SENDCMD_H_
#define AudioSmart_SENDCMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "syna_types.h"

// sendcommand callback function
typedef void(*sendcmd_cb)(void *data);

 /**
 * \fn int32_t AudioSmart_mid_level_init(void (*delay_fn_ptr)(uint32_t delay_in_ms));
 * \brief This function initializes the mid-level API
 *
 * \note  This should be called at least once before calling AudioSmart_mid_sendcmd
 * 
 * \param delay_fn_ptr function pointer to a delay function which takes number of ms as argument.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_mid_level_init(void (*delay_fn_ptr)(uint32_t delay_in_ms));

 /**
 * \fn int32_t AudioSmart_mid_sendcmd (uint32_t *cmd)
 * \brief This function sends a command to the Synaptics Audio DSP
 *
 * \note  Before this function is called the function AudioSmart_mid_level_init should be called to register the delay function
 * 
 * \param cmd uint32_t Pointer to the command buffer to be sent
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_mid_sendcmd(uint32_t *cmd);

/**
* \fn int32_t AudioSmart_mid_sendcmd_async (uint32_t *cmd)
* \brief This function sends a command to the Synaptics Audio DSP
*
* \note  Before this function is called the function AudioSmart_mid_level_init should be called to register the delay function
*
* \param cmd uint32_t Pointer to the command buffer to be sent
* \param wait_timeout uint32_t timeout in MS
* \param cb_func sendcmd_cb Pointer to the command callback function
* \return status int32_t representing the status of the function.
*               = 0 if successful
*               < 0 if error
*/
int32_t AudioSmart_mid_sendcmd_async(uint32_t *cmd, uint32_t wait_timeout, sendcmd_cb cb_func);

 /**
 * \fn int32_t AudioSmart_i2c_wait_until_bootup (void (*delay_fn_ptr)(uint32_t delay_in_ms))；
 * \brief This function checks the boot-up status of the Synaptics Audio DSP
 *
 * \param delay_fn_ptr  Function Pointer to a delay function which takes number of ms as argument.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_wait_until_bootup(void (*delay_fn_ptr)(uint32_t delay_in_ms));

#ifdef __cplusplus
}
#endif

#endif /* AudioSmart_SENDCMD_H_ */

/*@}*/
