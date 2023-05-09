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

  File Name: AudioSmart_usb_interface_api.c

------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "libusb.h"
#include "AudioSmart_usb_interface_api.h"

#if defined(_WIN32)
#define msleep(msecs) Sleep(msecs)
#else
#include <unistd.h>
#define msleep(msecs) usleep(1000*msecs)
#endif

#define HID_GET_REPORT             0x01
#define HID_GET_IDLE               0x02
#define HID_GET_PROTOCOL           0x03
#define HID_SET_REPORT             0x09
#define HID_SET_IDLE               0x0A
#define HID_SET_PROTOCOL           0x0B

#define HID_REPORT_TYPE_INPUT      0x01
#define HID_REPORT_TYPE_OUTPUT     0x02
#define HID_REPORT_TYPE_FEATURE    0x03

libusb_device_handle *handle = NULL;
int32_t nb_ifaces = 0;
int32_t hid_iface = 0;

int32_t AudioSmart_usb_interface_init(uint32_t vid, uint32_t pid)
{
  libusb_device *dev;
  struct libusb_config_descriptor *conf_desc;
  libusb_context *ctx = NULL;
  int32_t iface;
  int32_t r;
  uint8_t i;

  r = libusb_init(&ctx);
  if (r < 0)
    return r;

  const struct libusb_version *pver = NULL;
  pver = libusb_get_version();
  if (pver != NULL)
  {
    AUDIOSMART_DEBUG_PRINT(stdout, "libusb version: %d.%d.%d.%d\n", pver->major, pver->minor, pver->micro, pver->nano);
  }

  libusb_set_debug(ctx, 0);

  handle = libusb_open_device_with_vid_pid(ctx, (uint16_t)vid, (uint16_t)pid);

  if (handle == NULL)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Error: unable to open USB device %04X:%04X.\n", vid, pid);
    return -1;
  }

  dev = libusb_get_device(handle);
  r = libusb_get_config_descriptor(dev, 0, &conf_desc);
  if (r < 0)
    return r;
  nb_ifaces = conf_desc->bNumInterfaces;

  //ToDo: Clean this up
  //libusb_set_auto_detach_kernel_driver(handle, 1);
  r = 0;
  for (iface = 0; iface < nb_ifaces; iface++)
  {
    if (conf_desc->interface[iface].altsetting[0].bInterfaceClass == 3)
    {
      hid_iface = iface;
      AUDIOSMART_DEBUG_PRINT(stderr, "\nClaiming HID interface %d...Class:%0X\n", hid_iface, conf_desc->interface[hid_iface].altsetting[0].bInterfaceClass);
      break;
    }
  }

#if !defined(__APPLE__)
  if (libusb_kernel_driver_active(handle, hid_iface))
  {
    r = libusb_detach_kernel_driver(handle, hid_iface);
    if (r != LIBUSB_SUCCESS)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "   Failed to detach interface %d - error %d\n", hid_iface, r);
    }
    else
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "   Detach\n");
    }
  }
  else
    AUDIOSMART_DEBUG_PRINT(stderr, "   Kernel isn't attached to interface %d\n", hid_iface);
#endif

#if 0
  r = libusb_set_configuration(handle, 1);
  if (r != LIBUSB_SUCCESS) {
    AUDIOSMART_DEBUG_PRINT(stderr,"   Failed to set configuration to 1 - error %d\n",r);
  } else {
    AUDIOSMART_DEBUG_PRINT(stderr,"   Set configuration to 1\n");
  }
#endif

#if !defined(__APPLE__)
  r = libusb_claim_interface(handle, hid_iface);
  if (r != LIBUSB_SUCCESS)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "   Failed.\n");
  }
  else
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "   Success.\n");
  }
#endif

  AUDIOSMART_DEBUG_PRINT(stderr, "\nReading string descriptors:\n");

  for (i = 1; i < 3; i++)
  {
    char string[128];
    if (libusb_get_string_descriptor_ascii(handle, i, (unsigned char *)string, 128) >= 0)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "   String (0x%02X): \"%s\"\n", i, string);
    }
  }

  return (r);
}

int32_t AudioSmart_usb_interface_write(uint32_t *data, int32_t size)
{
  int32_t r;
  uint8_t hid_buffer[256];

  if (handle == NULL)
    return (-1);

  memset(hid_buffer, 0, sizeof(hid_buffer));
  hid_buffer[0] = 1;
  hid_buffer[1] = 0;
  memcpy(hid_buffer + 2, data, size);

  size += 2;

  r = libusb_control_transfer(handle,
                              LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                              HID_SET_REPORT,
                              (HID_REPORT_TYPE_OUTPUT << 8) | 1,
                              hid_iface,
                              (uint8_t *)hid_buffer,
                              (uint16_t)size,
                              5000);

  if (r < 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Error: failed to write to USB-HID - %d\n", r);
  }

  return (r);
}

int32_t AudioSmart_usb_interface_read(uint32_t *data, int32_t size)
{
  int32_t r;
  uint8_t hid_buffer[256];
  int32_t m;

  if (handle == NULL)
    return (-1);

  size += 2;
  memset(hid_buffer, 0, sizeof(hid_buffer));
  hid_buffer[0] = 1;

  //ToDo: Clean this up
  for (m = 0; m < 5; m++)
  {
    r = libusb_control_transfer(handle,
                                (LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE),
                                HID_GET_REPORT,
                                (HID_REPORT_TYPE_INPUT << 8) | 1,
                                hid_iface,
                                hid_buffer,
                                (uint16_t)size,
                                5000);

    if (r < 0)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "Error: failed read from USB-HID - %d\n", r);
    }

    if (hid_buffer[5] & 0x80)
      break;

    msleep(1 + (m << 2));
  }

  memcpy(data, hid_buffer + 2, size - 2);

  return (r);
}

int32_t AudioSmart_usb_interface_close(void)
{
  int32_t iface;

  if (handle == NULL)
    return (-1);

  for (iface = 0; iface < nb_ifaces; iface++)
  {
    libusb_release_interface(handle, iface);
  }

#if !defined(__APPLE__)
  libusb_attach_kernel_driver(handle, hid_iface);
#endif

  libusb_close(handle);
  libusb_exit(NULL);

  return 0;
}

//////////////////////////////////////////////////////////////////////////
#define CONFIG_LIBUSB_INTERFACE_POD 1

#ifdef  CONFIG_LIBUSB_INTERFACE_POD

int32_t AudioSmart_usb_hid_interface_init_with_handle(void **phandle, uint32_t vid, uint32_t pid,int32_t *nb_ifaces,int32_t *hid_iface)
{
  libusb_device *dev;
  struct libusb_config_descriptor *conf_desc;
  libusb_context *ctx = NULL;
  int32_t iface;
  int32_t r;
  uint8_t i;

  if (phandle == NULL || hid_iface == NULL || nb_ifaces == NULL)
    return (-1);

  r = libusb_init(&ctx);
  if (r < 0)
    return r;

  libusb_device_handle * handle = NULL; 
  const struct libusb_version *pver = NULL;
  pver = libusb_get_version();
  if (pver != NULL)
  {
    AUDIOSMART_DEBUG_PRINT(stdout, "libusb version: %d.%d.%d.%d\n", pver->major, pver->minor, pver->micro, pver->nano);
  }

  libusb_set_debug(ctx, 0);

  handle = libusb_open_device_with_vid_pid(ctx, (uint16_t)vid, (uint16_t)pid);

  if (handle == NULL)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Error: unable to open USB device %04X:%04X.\n", vid, pid);
    return -1;
  }

  dev = libusb_get_device(handle);
  r = libusb_get_config_descriptor(dev, 0, &conf_desc);
  if (r < 0)
    return r;

  if(nb_ifaces == NULL || hid_iface ==NULL ) return -1; 

  *nb_ifaces = conf_desc->bNumInterfaces;

  //ToDo: Clean this up
  //libusb_set_auto_detach_kernel_driver(handle, 1);
  r = 0;
  for (iface = 0; iface < *nb_ifaces; iface++)
  {
    if (conf_desc->interface[iface].altsetting[0].bInterfaceClass == 3)
    {
      *hid_iface = iface;
      AUDIOSMART_DEBUG_PRINT(stderr, "\nClaiming HID interface %d...Class:%0X\n", *hid_iface, conf_desc->interface[*hid_iface].altsetting[0].bInterfaceClass);
      break;
    }
  }

#if !defined(__APPLE__)
  if (libusb_kernel_driver_active(handle, *hid_iface))
  {
    r = libusb_detach_kernel_driver(handle, *hid_iface);
    if (r != LIBUSB_SUCCESS)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "   Failed to detach interface %d - error %d\n", *hid_iface, r);
    }
    else
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "   Detach\n");
    }
  }
  else
    AUDIOSMART_DEBUG_PRINT(stderr, "   Kernel isn't attached to interface %d\n", *hid_iface);
#endif

#if 0
  r = libusb_set_configuration(handle, 1);
  if (r != LIBUSB_SUCCESS) {
    AUDIOSMART_DEBUG_PRINT(stderr,"   Failed to set configuration to 1 - error %d\n",r);
  } else {
    AUDIOSMART_DEBUG_PRINT(stderr,"   Set configuration to 1\n");
  }
#endif

#if !defined(__APPLE__)
  r = libusb_claim_interface(handle, *hid_iface);
  if (r != LIBUSB_SUCCESS)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "   Failed.\n");
  }
  else
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "   Success.\n");
  }
#endif

  AUDIOSMART_DEBUG_PRINT(stderr, "\nReading string descriptors:\n");

  for (i = 1; i < 3; i++)
  {
    char string[128];
    if (libusb_get_string_descriptor_ascii(handle, i, (unsigned char *)string, 128) >= 0)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "   String (0x%02X): \"%s\"\n", i, string);
    }
  }

  *phandle = handle;
  fprintf(stdout, "hid_interface = %d, number_interface =%d \n", *hid_iface, *nb_ifaces);
  fprintf(stdout, "handle address = 0x%x\n", *phandle);
  
  return (r);
}

int32_t AudioSmart_usb_hid_interface_write_with_handle(void *handle,int32_t *hid_iface,uint8_t *data, int32_t size)
{
  int32_t r;

  if (handle == NULL || data == NULL || hid_iface == NULL || size <=0 )
    return (-1);
  
  r = libusb_control_transfer((libusb_device_handle *)handle,
                              LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                              HID_SET_REPORT,
                              (HID_REPORT_TYPE_OUTPUT << 8) | data[0],
                              *hid_iface,
                              data,
                              (uint16_t)size,
                              5000);

  if (r < 0)
  {
    AUDIOSMART_DEBUG_PRINT(stderr, "Error: failed to write to USB-HID - %d\n", r);
  }

  return (r);
}

int32_t AudioSmart_usb_hid_interface_read_with_handle(void *handle,int32_t *hid_iface,uint8_t *data, int32_t size)
{
  int32_t r;

  if (handle == NULL || data == NULL || hid_iface == NULL || size <=0 )
    return (-1);



    r = libusb_control_transfer((libusb_device_handle *)handle,
                                (LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE),
                                HID_GET_REPORT,
                                (HID_REPORT_TYPE_INPUT << 8) | data[0],
                                *hid_iface,
                                (uint8_t *)data,
                                (uint16_t)size,
                                5000);

    if (r < 0)
    {
      AUDIOSMART_DEBUG_PRINT(stderr, "Error: failed read from USB-HID - %d\n", r);
    }

  return (r);
}

int32_t AudioSmart_usb_hid_interface_close_with_handle(void *handle,int32_t *nb_ifaces,int32_t *hid_iface)
{
  int32_t iface;


  if (handle ==NULL  || nb_ifaces == NULL || hid_iface == NULL)
    return (-1);

  //for (iface = 0; iface < *nb_ifaces; iface++)
 // {
    libusb_release_interface((libusb_device_handle *)handle, *hid_iface);
  //}

#if !defined(__APPLE__)
  libusb_attach_kernel_driver((libusb_device_handle *)handle, *hid_iface);
#endif

  libusb_close((libusb_device_handle *)handle);
  //libusb_exit(NULL);

  return 0;
}


#endif