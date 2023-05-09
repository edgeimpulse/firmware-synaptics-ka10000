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

  File Name: AudioSmart_i2c_interface_api.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_i2c_interface_api.h
 * \brief This file contains the interface API related to communicating with
 *        the Synaptics Audio DSP using I2C Interface.
 *
 * \addtogroup I2CInterfaceAPI
 * @{
 *
 */
#ifndef AudioSmart_I2C_INTERFACE_API_H_
#define AudioSmart_I2C_INTERFACE_API_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "syna_types.h"

#if defined CONFIG_TARGET_OS_ANDROID
  #include <android/log.h>
  #define logInfo(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
  #define logError(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
  #define logWarn(...)  __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#else
  #define logError	printf 
  #define logInfo	printf 
  #define logWarn	printf 
#endif


#define I2C_BUS_CODEC_NAME_T "name"
#define I2C_DEIVCE_CODEC_NAME "as33970"
#define I2C_BUS_DEIVCE_PATH "/sys/bus/i2c/devices/"
#define I2C_BUS_SLAVE_ADDR_NAME_DEC "41"
#define I2C_BUS_DSP_REST_NAME "reset_dsp"


#if INTERFACE_I2C_BT_SPP
/**
 * \fn int32_t AudioSmart_i2c_bt_spp_interface_init (uint32_t i2c_address, int32_t offset_len)
 * \brief This function initializes the I2C interface to communicate with the Synaptics Audio DSP
 *
 * \param i2c_address uint32_t representing the I2C address for communicate using I2C interface
 * \param offset_len  int32_t representing the I2C offset length (should be 2 for 16-bit I2C addresses)
 * \param is_primary uint32_t representing primary or secondary will be upgraded
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_bt_spp_interface_init (uint32_t i2c_address, int32_t offset_len, uint32_t is_primary);
#else
#if INTERFACE_I2C_F3
/**
 * \fn int32_t AudioSmart_i2c_f3_interface_init (uint32_t i2c_address, int32_t offset_len, int16_t vid, int16_t pid)
 * \brief This function initializes the I2C interface to communicate with the Synaptics Audio DSP
 *
 * \param i2c_address uint32_t representing the I2C address for communicate using I2C interface
 * \param offset_len  int32_t representing the I2C offset length (should be 2 for 16-bit I2C addresses)
 * \param speed    int32_t representing the I2C speed
 * \param vid   vendor id of device
 * \param pid   product if of device
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_f3_interface_init (uint32_t i2c_address, int32_t offset_len, int32_t speed, uint16_t vid, uint16_t pid);
#else
/**
 * \fn int32_t AudioSmart_i2c_interface_init (uint32_t i2c_address, int32_t offset_len, int8_t *card_name)
 * \brief This function initializes the I2C interface to communicate with the Synaptics Audio DSP
 *
 * \param i2c_address uint32_t representing the I2C address for communicate using I2C interface
 * \param offset_len  int32_t representing the I2C offset length (should be 2 for 16-bit I2C addresses)
 * \param card_name   Pointer to a string containing the I2C card name
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_init (uint32_t i2c_address, int32_t offset_len, int8_t *card_name);
#endif
#endif
/**
 * \fn int32_t AudioSmart_i2c_interface_write (int32_t address, uint32_t *data, int32_t size)
 * \brief This function writes to the Synaptics Audio DSP, using the Host
 *        hardware interface
 *
 * \param address int32_t representing the I2C register address to write to
 * \param data uint32_t pointer to the data to be written to the Synaptics Audio DSP
 * \param size uint32_t representing the number of 32 bit words of data to be written to the Synaptics Audio DSP
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_write (int32_t address, uint32_t *data, int32_t size);

/**
 * \fn int32_t  AudioSmart_i2c_interface_read (int32_t address, uint32_t *data, int32_t size)
 * \brief This function reads from Synaptics Audio DSP, using the Host
 *        hardware interface
 *
 * \param address int32_t representing the I2C register address to read from
 * \param data uint32_t pointer to memory where the data is read to from Synaptics Audio DSP
 * \param size uint32_t representing the number of 32 bit words of data to be read from the Synaptics Audio DSP
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_read (int32_t address, uint32_t *data, int32_t size);

/**
 * \fn int32_t AudioSmart_i2c_interface_write_bytes (int32_t address, uint8_t *data, int32_t size)
 * \brief This function writes to the Synaptics Audio DSP, using the Host
 *        hardware interface
 *
 * \param address int32_t representing the I2C register address to write to
 * \param data uint8_t pointer to the data to be written to the Synaptics Audio DSP
 * \param size int32_t representing the number of bytes of data to be written to the Synaptics Audio DSP
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_write_bytes (int32_t address, uint8_t *data, int32_t size);

/**
 * \fn int32_t  AudioSmart_i2c_interface_read_bytes (int32_t address, uint8_t *data, int32_t size)
 * \brief This function reads from Synaptics Audio DSP, using the Host
 *        hardware interface
 *
 * \param address int32_t representing the I2C register address to read from
 * \param data uint8_t pointer to memory where the data is read to from Synaptics Audio DSP
 * \param size int32_t representing the number of bytes of data to be read from the Synaptics Audio DSP
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_read_bytes (int32_t address, uint8_t *data, int32_t size);

/**
 * \fn int32_t AudioSmart_i2c_interface_close (void)
 * \brief This function closes the Host interface communicating with Synaptics Audio DSP
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_close (void);

/**
 * \fn int32_t AudioSmart_i2c_interface_hdr_info (bool ForceBooted)
 * \brief This function earse the Host header information
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_hdr_info(bool ForceBooted);

/**
 * \fn int32_t AudioSmart_i2c_interface_set_to_download_mode (void)
 * \brief This function set Tahiti to download mode
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_interface_set_to_download_mode(void);

/**
 * \fn int32_t AudioSmart_set_gpio_reset(int32_t high)
 * \brief This function resets the Synaptics Audio DSP and is meant only for Windows
 *
 * \param high int32_t representing high or low value for the GPIO pin
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_set_gpio_reset(int32_t high);
#ifdef WIN32
/**
 * \fn int32_t AudioSmart_i2c_set_speed(bool b400Khz)
 * \brief This function sets the I2C speed to 400Khz and is meant only for Windows
 *
 * \param b400Khz boolean which sets the I2C speed to 400Khz if true, or the default if 0
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_i2c_set_speed(bool b400Khz);
#endif

#ifdef __cplusplus
}
#endif

#endif /* AudioSmart_I2C_INTERFACE_API_H_ */
/*@}*/
