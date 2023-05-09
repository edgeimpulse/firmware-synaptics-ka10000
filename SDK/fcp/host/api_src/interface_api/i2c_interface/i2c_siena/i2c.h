/*------------------------------------------------------------------------------
  Copyright (C) 2010-2011 Conexant Systems Inc.
  All rights reserved.

  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Conexant Systems, Inc.
  ("Conexant")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Conexant Systems Inc.

  Except as otherwise specifically provided through an express agreement
  with Conexant that governs the confidentiality, possession, use
  and distribution of the information contained in this file, CONEXANT
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: i2c.h

  Description: Common header for i2c functions.

------------------------------------------------------------------------------*/

#ifndef _I2C_H_
#define _I2C_H_

#include<Windows.h>
#include <WinIoCtl.h>
#include <stdio.h>
#include <setupapi.h>
#include "macro.h"
#include <tchar.h>

struct CXI2CINTERFACE;

typedef void (*FnCloseI2cDevice)(CXI2CINTERFACE* hI2cDevice);
typedef int (*FnI2cWrite)(CXI2CINTERFACE* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf, unsigned char* pBuf);
typedef int (*FnI2cWriteThenRead)(CXI2CINTERFACE* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf,
    unsigned char* pBuf, unsigned long cbReadBuf, unsigned char*pReadBuf);
typedef int (*FnSetI2cSpeed)(CXI2CINTERFACE* hI2cDevice,bool  b400Khz);  
typedef int (*FnGpioSetPort)(CXI2CINTERFACE* hI2cDevice,
            unsigned char portNumber,
            unsigned char portDirection,
            unsigned char portValue);
typedef int (*FnGpioSetPin)(CXI2CINTERFACE* hI2cDevice,
            unsigned char portNumber,
            unsigned char pinNumber,
            unsigned char pinValue);

struct CXI2CINTERFACE{
   HANDLE                   hHandle;
   FnCloseI2cDevice         CloseDevice;
   FnI2cWrite               I2cWrite;
   FnI2cWriteThenRead       I2cWriteThenRead;
   FnSetI2cSpeed            SetI2cSpeed;  
   FnGpioSetPort            GpioSetPort;
   FnGpioSetPin             GpioSetPin;
};

#include "siena.h"

//#include "HIDHandler.h"
//#include "USBIOX.H"
extern wchar_t g_szHwId[MAX_PATH];

#endif
