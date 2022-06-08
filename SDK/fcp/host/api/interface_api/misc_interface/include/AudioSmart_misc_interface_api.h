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

  File Name: AudioSmart_misc_interface_api.h
  
------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_misc_interface_api.h
 * \brief This file contains the interface API related to communicating with
 *        the Synaptics Audio DSP using Misc Interface.
 *
 * \addtogroup MISCInterfaceAPI
 * @{
 *
 */
#ifndef AudioSmart_MISC_INTERFACE_API_H_
#define AudioSmart_MISC_INTERFACE_API_H_

#ifdef __cplusplus
extern "C"{
#endif 

#define MISC_MAX_PATH     260
/**
 * \fn int32_t AudioSmart_misc_mount (void)
 * \brief This function mount the storage device
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_misc_mount(void);

/**
 * \fn int32_t AudioSmart_misc_umount (void)
 * \brief This function umount the storage device
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_misc_umount(void);

/**
 * \fn int32_t AudioSmart_misc_open_img(uint32_t * addr, char * name)
 * \brief This function open image file.
 * \param uint32_t *addr:start offset to read the image file, will be set 0 by default.
 * \param char *name: image file path.
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_misc_open_img(uint32_t * addr, char * name);

/**
 * \fn int32_t AudioSmart_misc_read_block(uint32_t base, uint32_t * buffer, uint32_t size)
 * \brief This function read one block data from image file to buffer.
 * \param uint32_t base: offset to be read from image file.
 * \param uint32_t *buffer: buffer will save the data.
 * \param uint32_t  size: buffer size.
 * \return status int32_t representing the status of the function.
 *               > 0 if successful
 *               <= 0 if error
 */
int32_t AudioSmart_misc_read_block(uint32_t base, uint32_t * buffer, uint32_t size);

/**
 * \fn void AudioSmart_misc_wait_ms(uint32_t ms)
 * \brief This function will wait for some  milliseconds.
 * \param uint32_t ms: wait milliseconds time.
 * \return void 
 */
void AudioSmart_misc_wait_ms(uint32_t ms);


#ifdef __cplusplus
}
#endif 

#endif // AudioSmart_MISC_INTERFACE_API_H_