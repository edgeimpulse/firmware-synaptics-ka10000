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

  File Name: siena_wrapper.c

------------------------------------------------------------------------------*/

#include <windows.h>
#include <assert.h>
#include <siena.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define offsetof(s, m) ((uint)(size_t) & (((s *)0)->m))
#define dwsizeb(a) ((a) / sizeof(dword))
#define wsizeb(a) ((a) / sizeof(unsigned short))

#define MAX_ADDRESS ((1 << 24) - 1)
#define MAX_PACKET_SIZE 125
#define MAX_OFFSET_LEN 3
#define ADR 0x00000004
#define DAT0 0x76543210
#define DAT1 0xfedcba98
#define DATx (DAT0 >> 8) | (DAT1 << 24)

CXI2CINTERFACE *i2c_intf;
static unsigned char deviceAddress;
static int offsetLen;
unsigned int m_blknr = 0;
#define CONSOLE_PRINT 1
#define WINMSG_PRINT 2
static int debug_type = 0;

void sys_print(const char *format, ...);
void sys_sleep(int intreval_ms);

void sys_set_debug_type(int /*type*/)
{
}

int i2c_existed()
{
  if (i2c_intf == NULL)
    return (-1);
  if (i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);

  return 0;
}

int i2c_init(int slave_address, int offset_len, char *card)
{
  if ((i2c_intf = SienaOpenI2cDevice()) == NULL)
    return (-1);
  if (i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);

  deviceAddress = (unsigned char)slave_address;
  offsetLen = offset_len;
  return 0;
}

int i2c_write(int address, const void *buffer, int size)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);
  int i;
  unsigned char payload[MAX_PACKET_SIZE];

  int len = size * 4;

  assert(len <= (MAX_PACKET_SIZE - offsetLen));
  if (len > (MAX_PACKET_SIZE - offsetLen))
  {
    return -1;
  }

  for (i = 0; i < offsetLen; i++)
    payload[i] = (address >> ((offsetLen - 1 - i) * 8)) & 0xFF;

  memcpy(&(payload[offsetLen]), (unsigned char *)buffer, len);

  return i2c_intf->I2cWrite(i2c_intf, deviceAddress, len + offsetLen, payload);
}

int i2c_read(int address, void *buffer, int size)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);

  int i;

  address &= 0xFFFFFF;
  unsigned char offset[MAX_OFFSET_LEN] = {0};

  int len = size * 4;

  assert(len <= (MAX_PACKET_SIZE));
  if (len > (MAX_PACKET_SIZE))
  {
    return -1;
  }

  for (i = 0; i < offsetLen; i++)
    offset[i] = (address >> ((offsetLen - 1 - i) * 8)) & 0xFF;

  return i2c_intf->I2cWriteThenRead(i2c_intf, deviceAddress, offsetLen, offset, len, (unsigned char *)buffer);
}

int i2c_close()
{
  if (i2c_intf)
    i2c_intf->CloseDevice(i2c_intf);
  return (0);
}

int i2c_gpio_setport(unsigned char portNumber, unsigned char Direction, unsigned char Value)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);
  return i2c_intf->GpioSetPort(i2c_intf, portNumber, Direction, Value);
}

int i2c_gpio_setpin(unsigned char portNumber, unsigned char pinNumber, unsigned char Value)
{
  if (i2c_intf == NULL || i2c_intf->hHandle == INVALID_HANDLE_VALUE)
    return (-1);
  return i2c_intf->GpioSetPin(i2c_intf, portNumber, pinNumber, Value);
}
