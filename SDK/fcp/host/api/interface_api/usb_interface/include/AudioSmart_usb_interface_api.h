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

  File Name: AudioSmart_usb_interface_api.h

------------------------------------------------------------------------------*/
/**
 * \file AudioSmart_usb_interface_api.h
 * \brief This file contains the interface API related to communicating with
 *        the Synaptics Audio DSP using USB Interface.
 *
 * \addtogroup USBInterfaceAPI
 * @{
 *
 */
#ifndef AudioSmart_USB_INTERFACE_API_H_
#define AudioSmart_USB_INTERFACE_API_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "syna_types.h"

/**
 * \fn int32_t  AudioSmart_usb_interface_init (uint32_t vid, uint32_t pid);
 * \brief This function initializes the USB interface to communicate with the Synaptics Audio DSP
 *
 * \param vid    uint32_t representing the Vendor ID for communication using the USB interface
 * \param pid    uint32_t representing the Product ID for communication using the USB interface
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t  AudioSmart_usb_interface_init (uint32_t vid, uint32_t pid);

/**
 * \fn int32_t  AudioSmart_usb_interface_write (uint32_t *data, int32_t size);
 * \brief This function writes to the Synaptics Audio DSP, using the Host
 *        hardware interface
 *
 * \param data uint32_t pointer to the data to be written to the Synaptics Audio DSP
 * \param size uint32_t representing the number of bytes of data to be written to the Synaptics Audio DSP
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t  AudioSmart_usb_interface_write (uint32_t *data, int32_t size);

/**
 * \fn int32_t  AudioSmart_usb_interface_read (uint32_t *data, int32_t size);
 * \brief This function reads from the Synaptics Audio DSP, using the Host
 *        hardware interface
 *
 * \param data uint32_t pointer to memory where the data is read to from the Synaptics Audio DSP
 * \param size uint32_t representing the number of bytes of data to be read from the Synaptics Audio DSP
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t  AudioSmart_usb_interface_read (uint32_t *data, int32_t size);

/**
 * \fn int32_t AudioSmart_usb_interface_close (void);
 * \brief This function closes the Host interface communicating with the Synaptics Audio DSP
 *
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_usb_interface_close (void);


/**
 * \fn int32_t  AudioSmart_usb_hid_interface_init_with_handle(void **phandle, uint32_t vid, uint32_t pid,int32_t *nb_ifaces,int32_t *hid_iface);
 * \brief This function initializes the USB interface to communicate with the Synaptics Audio devices
 *
 * \note  This api only support linux or android platform which use the libusb as usb library
 * \param phandle       uint32_t representing the pointer to a usb handler 
 * \param vid    		    uint32_t representing the Vendor ID for communication using the USB interface
 * \param pid    		    uint32_t representing the Product ID for communication using the USB interface
 * \param nb_ifaces     int32_t* representing the number of usb interface for current USB device
 * \param hid_iface     int32_t* representing the hid interface number
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t  AudioSmart_usb_hid_interface_init_with_handle(void **phandle, uint32_t vid, uint32_t pid,int32_t *nb_ifaces,int32_t *hid_iface);

/**
 * \fn int32_t  AudioSmart_usb_hid_interface_write_with_handle(void *handle,int32_t *hid_iface,uint8_t *data, int32_t size);
 * \brief This function writes to the Synaptics Audio Device, using the Host
 *        hardware interface
 * 
 * \note  This api only support linux or android platform which use the libusb as usb library
 * \param handle        uint32_t representing  a usb handler 
 * \param hid_iface     int32_t* representing the hid interface number
 * \param data uint8_t  pointer to the data to be written to the Synaptics Audio device  
 * \param size uint32_t representing the number of bytes of data to be written to the Synaptics Audio device 
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t  AudioSmart_usb_hid_interface_write_with_handle(void *handle,int32_t *hid_iface,uint8_t *data, int32_t size);

/**
 * \fn int32_t  AudioSmart_usb_hid_interface_read_with_handle(void *handle,int32_t *hid_iface,uint8_t *data, int32_t size);
 * \brief This function reads from the Synaptics Audio DSP, using the Host
 *        hardware interface
 * 
 * \note  This api only support linux or android platform which use the libusb as usb library
 * \param handle        uint32_t representing  a usb handler 
 * \param hid_iface     int32_t* representing the hid interface number
 * \param data uint32_t pointer to memory where the data is read to from the Synaptics Audio device
 * \param size uint32_t representing the number of bytes of data to be read from the Synaptics Audio device 
 * \return status int32_t representing the number of bytes transferred or
 *               < 0 if error
 */
int32_t  AudioSmart_usb_hid_interface_read_with_handle(void *handle,int32_t *hid_iface,uint8_t *data, int32_t size);

/**
 * \fn int32_t AudioSmart_usb_hid_interface_close_with_handle(void *handle,int32_t *nb_ifaces,int32_t *hid_iface);
 * \brief This function closes the Host interface communicating with the Synaptics Audio device
 * 
 * \note  This api only support linux or android platform which use the libusb as usb library
 * \param handle        uint32_t representing  a usb handler 
 * \param nb_ifaces     int32_t* representing the number of usb interface for current USB device
 * \param hid_iface     int32_t* representing the hid interface number
 * \return status int32_t representing the status of the function.
 *               = 0 if successful
 *               < 0 if error
 */
int32_t AudioSmart_usb_hid_interface_close_with_handle(void *handle,int32_t *nb_ifaces,int32_t *hid_iface);


#ifdef __cplusplus
}
#endif 


#endif /* AudioSmart_USB_INTERFACE_API_H_ */
/*@}*/
