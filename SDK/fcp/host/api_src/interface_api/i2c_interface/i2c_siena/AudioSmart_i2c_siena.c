/*------------------------------------------------------------------------------
  Copyright (C) 2019 Synaptics Inc.
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

  File Name: AudioSmart_i2c_dev.c

------------------------------------------------------------------------------*/
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "AudioSmart_i2c_interface_api.h"
#ifdef INTERFACE_I2C_SIENA
#include "i2c.h"
#include <siena.h>

#define MAX_PACKET_SIZE 125
#define MAX_OFFSET_LEN 3

CXI2CINTERFACE *i2c_intf;
static uint8_t deviceAddress;
static int32_t offsetLen;

int32_t AudioSmart_i2c_interface_init(uint32_t slave_address, int32_t offset_len, int8_t *card)
{
  if ((i2c_intf = SienaOpenI2cDevice()) == NULL)
      return (-1);
  if (i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);
   SienaSetI2cSpeed(i2c_intf , 1);
  deviceAddress = (uint8_t)slave_address;
  offsetLen = offset_len;
  return 0;
}

int32_t AudioSmart_i2c_interface_write(int32_t address, uint32_t *buffer, int32_t size)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);
  int32_t i;
  uint8_t payload[MAX_PACKET_SIZE];

  int32_t len = size * 4;

  assert(len <= (MAX_PACKET_SIZE - offsetLen));
  if (len > (MAX_PACKET_SIZE - offsetLen))
  {
    return -1;
  }

  for (i = 0; i < offsetLen; i++)
    payload[i] = (address >> ((offsetLen - 1 - i) * 8)) & 0xFF;

  memcpy(&(payload[offsetLen]), (uint8_t *)buffer, len);

  return i2c_intf->I2cWrite(i2c_intf, deviceAddress, len + offsetLen, payload);
}

int32_t AudioSmart_i2c_interface_read(int32_t address, uint32_t *buffer, int32_t size)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);

  int32_t i;

  address &= 0xFFFFFF;
  uint8_t offset[MAX_OFFSET_LEN] = {0};

  int32_t len = size * 4;

  assert(len <= (MAX_PACKET_SIZE));
  if (len > (MAX_PACKET_SIZE))
  {
    return -1;
  }

  for (i = 0; i < offsetLen; i++)
    offset[i] = (address >> ((offsetLen - 1 - i) * 8)) & 0xFF;

  return i2c_intf->I2cWriteThenRead(i2c_intf, deviceAddress, offsetLen, offset, len, (uint8_t *)buffer);
}

int32_t AudioSmart_i2c_interface_write_bytes (int32_t address, uint8_t *buffer, int32_t size)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);
  int32_t i;
  uint8_t payload[MAX_PACKET_SIZE];

  int32_t len = size;

  assert(len <= (MAX_PACKET_SIZE - offsetLen));
  if (len > (MAX_PACKET_SIZE - offsetLen))
  {
    return -1;
  }

  for (i = 0; i < offsetLen; i++)
    payload[i] = (address >> ((offsetLen - 1 - i) * 8)) & 0xFF;

  memcpy(&(payload[offsetLen]), (uint8_t *)buffer, len);

  return i2c_intf->I2cWrite(i2c_intf, deviceAddress, len + offsetLen, payload);
}

int32_t AudioSmart_i2c_interface_read_bytes (int32_t address, uint8_t *buffer, int32_t size)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);

  int32_t i;

  address &= 0xFFFFFF;
  uint8_t offset[MAX_OFFSET_LEN] = {0};

  int32_t len = size;

  assert(len <= (MAX_PACKET_SIZE));
  if (len > (MAX_PACKET_SIZE))
  {
    return -1;
  }

  for (i = 0; i < offsetLen; i++)
    offset[i] = (address >> ((offsetLen - 1 - i) * 8)) & 0xFF;

  return i2c_intf->I2cWriteThenRead(i2c_intf, deviceAddress, offsetLen, offset, len, (uint8_t *)buffer);
}

int32_t AudioSmart_i2c_interface_close()
{
  if (i2c_intf)
    i2c_intf->CloseDevice(i2c_intf);
  return (0);
}

int32_t AudioSmart_set_gpio_reset(int32_t high)
{
  int32_t ret = 0;

  if (high)
    ret = i2c_intf->GpioSetPort(i2c_intf, 3, 0x10, 0x10);
  else
    ret = i2c_intf->GpioSetPort(i2c_intf, 3, 0x10, 0x0);

  return ret;
}

int32_t AudioSmart_i2c_set_speed(bool b400Khz)
{
  return i2c_intf->SetI2cSpeed(i2c_intf, b400Khz);
}
#endif//INTERFACE_I2C_SIENA