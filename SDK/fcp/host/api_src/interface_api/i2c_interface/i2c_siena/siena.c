/*------------------------------------------------------------------------------
  Copyright (C) 2010-2012 Conexant Systems Inc.
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

  File Name: siena.c

------------------------------------------------------------------------------*/

#include "siena.h"
#include <InitGuid.h>
#include <Guiddef.h>
#include "i2c.h"
#include <stdio.h>
#include <process.h>

#pragma warning(disable:4244 4242 4715 ) 
#include <conio.h>

#pragma warning(disable:4127 4800) // conditional experssion is constant
// {0967f52a-39ce-4594-9e4a-f35482d5d5ae}
DEFINE_GUID(GUID_INTERFACE_CXSIENA, 
0x0967f52a, 0x39ce, 0x4594, 0x9e, 0x4a, 0xf3, 0x54, 0x82, 0xd5, 0xd5, 0xae);

#pragma pack(push, 1)

#define MAX_SIENA_I2C_BUF_SIZE 125
typedef struct tag_CXSIENA_I2C_WRITE_THEN_READ
{
  BYTE    ChipAddr;
  BYTE    cbWriteBuf;
  BYTE    WriteBuf[8];
  BYTE    cbReadBuf;
  BYTE    ReadBuf[MAX_SIENA_I2C_BUF_SIZE];
}CXSIENA_I2C_WRITE_THEN_READ,*PCXSIENA_I2C_WRITE_THEN_READ;

typedef struct tag_CXSIENA_I2C_WRITE
{
  BYTE    ChipAddr;
  BYTE    cbWriteBuf;
  BYTE    WriteBuf[MAX_SIENA_I2C_BUF_SIZE];
}CXSIENA_I2C_WRITE,*PCXSIENA_I2C_WRITE;

typedef struct tag_CXSIENA_I2C_READ
{
  BYTE    ChipAddr;
  BYTE    cbReadBuf;
  BYTE    ReadBuf[MAX_SIENA_I2C_BUF_SIZE];
}CXSIENA_I2C_READ,*PCXSIENA_I2C_READ;

typedef struct tag_CXSIENA_GPIO_PORT
{
  BYTE    PortNumber;
  BYTE    PortDirection;
  BYTE    PortValue;
}CXSIENA_GPIO_PORT,*PCXSIENA_GPIO_PORT;

typedef struct tag_CXSIENA_GPIO_PIN
{
  BYTE    PortNumber;
  BYTE    PinNumber;
  BYTE    PinValue;
}CXSIENA_GPIO_PIN,*PCXSIENA_GPIO_PIN;

typedef struct tag_CXSIENA_I2C_SPEED
{
  BYTE    I2cSpeed;// 0: 100KHz, 1: 400KHz
}CXSIENA_I2C_SPEED,*PCXSIENA_I2C_SPEED;

#pragma pack(pop)

//
// IOCTL_CXSIENA_SET_GPIO_PORT
//
// Input Buffer:
//     Structure of type CXSIENA_GPIO_PORT
//
// Output Buffer:
//     Structure of type CXSIENA_GPIO_PORT
//
#define IOCTL_CXSIENA_SET_GPIO_PORT         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b00, METHOD_BUFFERED, FILE_ANY_ACCESS)
//
// IOCTL_CXSIENA_GET_GPIO_PORT
//
// Input Buffer:
//     Structure of type CXSIENA_GPIO_PORT
//
// Output Buffer:
//     Structure of type CXSIENA_GPIO_PORT
//
#define IOCTL_CXSIENA_GET_GPIO_PORT         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b01, METHOD_BUFFERED, FILE_ANY_ACCESS)
//
// IOCTL_CXSIENA_SET_GPIO_PIN
//
// Input Buffer:
//     Structure of type CXSIENA_GPIO_PIN
//
// Output Buffer:
//     nULL
//
#define IOCTL_CXSIENA_SET_GPIO_PIN         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b02, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_CXSIENA_GET_GPIO_PIN
//
// Input Buffer:
//     Structure of type CXSIENA_GPIO_PIN
//
// Output Buffer:
//     nULL
//
#define IOCTL_CXSIENA_GET_GPIO_PIN         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b03, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_CXSIENA_WRITE_I2C
//
// Input Buffer:
//     Structure of type CXSIENA_GPIO_PIN
//
// Output Buffer:
//     Structure of type CXSIENA_GPIO_PIN
//
#define IOCTL_CXSIENA_WRITE_I2C         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b04, METHOD_BUFFERED, FILE_ANY_ACCESS)


//
// IOCTL_CXSIENA_WRITE_THEN_READ_I2C
//
// Input Buffer:
//     Structure of type CXSIENA_I2C_WRITE_THEN_READ
//
// Output Buffer:
//     pointer to read data 
//
#define IOCTL_CXSIENA_READ_I2C                 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b05, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_CXSIENA_WRITE_THEN_READ_I2C
//
// Input Buffer:
//     Structure of type CXSIENA_I2C_WRITE_THEN_READ
//
// Output Buffer:
//     pointer to read data 
//
#define IOCTL_CXSIENA_WRITE_THEN_READ_I2C         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b06, METHOD_BUFFERED, FILE_ANY_ACCESS)


//
// IOCTL_CXSIENA_WRITE_THEN_READ_I2C
//
// Input Buffer:
//     Structure of type CXSIENA_I2C_SPEED
//
// Output Buffer:
//     pointer to read data (1 BYTE)
//
#define IOCTL_CXSIENA_I2C_SPEED               CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8b12, METHOD_BUFFERED, FILE_ANY_ACCESS)

void SienaCloseI2cDevice   (CXI2CINTERFACE* hI2cDevice);
int  SienaI2cWrite         (CXI2CINTERFACE* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf    , unsigned char* pBuf);
int  SienaI2cWriteThenRead (CXI2CINTERFACE* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf    , unsigned char* pBuf, 
                                                                               unsigned long cbReadBuf, unsigned char*pReadBuf);
int  SienaSetI2cSpeed(CXI2CINTERFACE* hI2cDevice, BOOL b400Khz);
int  SienaGpioSetPort(CXI2CINTERFACE* hI2cDevice,
                      BYTE portNumber,
                      BYTE portDirection,
                      BYTE portValue);
int  SienaGpioSetPin (CXI2CINTERFACE* hI2cDevice,
                      BYTE portNumber,
                      BYTE pinNumber,
                      BYTE pinValue);

static CXI2CINTERFACE SienaI2c=
{
  NULL,
  SienaCloseI2cDevice,
  SienaI2cWrite,
  SienaI2cWriteThenRead,
  SienaSetI2cSpeed,
  SienaGpioSetPort,
  SienaGpioSetPin
};

static int FoundSiena = 0;

CXI2CINTERFACE *SienaOpenI2cDevice()
{
  HANDLE hI2cDevice = INVALID_HANDLE_VALUE;
  bool    bFound = FALSE;
  HDEVINFO                            hDevInfoSet         = NULL;
  SP_DEVICE_INTERFACE_DATA            DevIntData          = {sizeof(SP_DEVICE_INTERFACE_DATA)};
  PSP_DEVICE_INTERFACE_DETAIL_DATA    pDevIntDetailData   = NULL;
  DWORD                               cbDevIntDetailData  = 0;
  DWORD                               cbRequired          = 0;
  //
  DWORD                               dwErrCode           = 0;
  //    BOOL                                bQuit               = FALSE;

  if(FoundSiena==1) return &SienaI2c; 

  //HidD_GetHidGuid (&guidHidClass);
  do {
    hDevInfoSet = SetupDiGetClassDevs(NULL, NULL , NULL, DIGCF_DEVICEINTERFACE | DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if( hDevInfoSet == INVALID_HANDLE_VALUE) break;

    //
    // Allocate a function class device data structure to
    // receive the information about this particular device.
    //
    for(UINT i=0; SetupDiEnumDeviceInterfaces (hDevInfoSet,
        0, // No care about specific PDOs
        &GUID_INTERFACE_CXSIENA,
        i, //
        &DevIntData); )
    {
      //
      // First find out required length of the buffer
      //
      if(pDevIntDetailData) pDevIntDetailData->cbSize = sizeof( SP_DEVICE_INTERFACE_DETAIL_DATA);

      if(!SetupDiGetDeviceInterfaceDetail (
          hDevInfoSet,
          &DevIntData,
          pDevIntDetailData, // probing so no output buffer yet
          cbDevIntDetailData, // probing so output buffer length of zero
          &cbRequired,
          NULL) )
      {
        // if buffer is too small, then realloc a new one.
        if((dwErrCode = GetLastError()) == ERROR_INSUFFICIENT_BUFFER)
        {
          if( cbRequired > cbDevIntDetailData)
          {
            cbDevIntDetailData = cbRequired;
            SAFE_DELETE(pDevIntDetailData);
            pDevIntDetailData = 
                (PSP_DEVICE_INTERFACE_DETAIL_DATA)    new BYTE[cbRequired];
            ::ZeroMemory(pDevIntDetailData,cbRequired);
            continue;
          }
        }
        else
        {
          break;
        }
      }

      hI2cDevice = CreateFile (pDevIntDetailData->DevicePath,
          GENERIC_READ,
          FILE_SHARE_READ|FILE_SHARE_WRITE,
          NULL,        // no SECURITY_ATTRIBUTES structure
          OPEN_EXISTING, // No special create flags
          NULL, // Now we open the device as normal device.
          NULL);       // No template file

      bFound = TRUE;
	  FoundSiena = 1;
           
      break;
    }
  } while(0);

  SAFE_DELETE(pDevIntDetailData);
  SetupDiDestroyDeviceInfoList(hDevInfoSet);

  if(!bFound)
  {
    // printf("Device is not present!\n");
  }
  SienaI2c.hHandle =hI2cDevice;
  return &SienaI2c;
}


void SienaCloseI2cDevice(CXI2CINTERFACE* hI2cDevice)
{
  if(hI2cDevice){
    ::CloseHandle( hI2cDevice->hHandle);
     hI2cDevice->hHandle = INVALID_HANDLE_VALUE;
  }
  FoundSiena = 0;
}


int SienaI2cWrite(CXI2CINTERFACE* hI2cDevice, unsigned char ChipAddr, 
                  unsigned long   cbBuf     , unsigned char* pBuf)
{
  CXSIENA_I2C_WRITE  i2cBuf = {0};
  int          nRet ;

  DWORD cbReturn = 0;

  if (hI2cDevice == NULL)
    return (-1);
  if (cbBuf > ARRAY_SIZE(i2cBuf.WriteBuf))
    return (-1);

  i2cBuf.ChipAddr     = ChipAddr;
  i2cBuf.cbWriteBuf   = (BYTE)cbBuf;

  memcpy(i2cBuf.WriteBuf,pBuf,cbBuf);

  nRet = DeviceIoControl( hI2cDevice->hHandle,IOCTL_CXSIENA_WRITE_I2C,&i2cBuf,sizeof(i2cBuf),&i2cBuf,sizeof(i2cBuf),&cbReturn,NULL);
    
  if(!nRet)
  {
    printf("I2c write failed.\n");
  }
  return nRet;
}

int SienaI2cWriteThenRead(CXI2CINTERFACE* hI2cDevice, unsigned char ChipAddr, 
                          unsigned long cbBuf       , unsigned char * pBuf  , 
                          unsigned long cbReadBuf   , unsigned char*pReadBuf )
{
  CXSIENA_I2C_WRITE_THEN_READ  i2cBuf = {0};
  BOOL         bReturn;

  DWORD cbReturn = 0;

  if (hI2cDevice == NULL)
    return (-1);
  if (cbBuf     > ARRAY_SIZE(i2cBuf.WriteBuf))
    return (-1);
  if (cbReadBuf > ARRAY_SIZE(i2cBuf.ReadBuf))
    return (-1);
    
  i2cBuf.ChipAddr   = ChipAddr;
  i2cBuf.cbWriteBuf = (BYTE) cbBuf;
  i2cBuf.cbReadBuf  = (BYTE)cbReadBuf;
  memcpy(i2cBuf.WriteBuf,pBuf,cbBuf);
  bReturn =  DeviceIoControl( hI2cDevice->hHandle,IOCTL_CXSIENA_WRITE_THEN_READ_I2C,&i2cBuf,sizeof(i2cBuf),&i2cBuf,sizeof(i2cBuf),&cbReturn,NULL);

  if( bReturn)
  {
    memcpy(pReadBuf,i2cBuf.ReadBuf ,cbReadBuf);
  }
  else
  {
#if !defined(CONFIG_I2C_DISABLE_RESET) && !defined(CONFIG_TARGET_TAHITI) && !defined(CONFIG_TARGET_KATANA)
//removed dbg msg
//host will poll until read is successful
    printf("I2c read failed.\n");
#endif
  }
  return bReturn; 
}

int SienaSetI2cSpeed(CXI2CINTERFACE* hI2cDevice,bool b400Khz)
{
  CXSIENA_I2C_SPEED Speed;
  DWORD            bReturn;
  Speed.I2cSpeed = b400Khz?1:0;
  BYTE DATA;
  
  if (hI2cDevice == NULL)
    return (-1) ;
  return DeviceIoControl( hI2cDevice->hHandle,IOCTL_CXSIENA_I2C_SPEED,&Speed,sizeof(Speed),&DATA,1,&bReturn,NULL);
}

int SienaGpioSetPort(CXI2CINTERFACE* hI2cDevice,
            BYTE portNumber,
            BYTE portDirection,
            BYTE portValue)
{
  CXSIENA_GPIO_PORT gpio;

  gpio.PortNumber =     portNumber;
  gpio.PortDirection = portDirection;
  gpio.PortValue     = portValue;
  DWORD         bReturn;

  if (hI2cDevice == NULL)
    return (-1);
  return DeviceIoControl( hI2cDevice->hHandle,IOCTL_CXSIENA_SET_GPIO_PORT,&gpio,sizeof(gpio),NULL,0,&bReturn,NULL);
}

int SienaGpioSetPin(CXI2CINTERFACE* hI2cDevice,
            BYTE portNumber,
            BYTE pinNumber,
            BYTE pinValue)
{
  CXSIENA_GPIO_PIN gpio;

  gpio.PortNumber =     portNumber;
  gpio.PinNumber = pinNumber;
  gpio.PinValue     = pinValue;
  DWORD         bReturn;

  if (hI2cDevice == NULL)
    return (-1);
  return DeviceIoControl( hI2cDevice->hHandle,IOCTL_CXSIENA_SET_GPIO_PIN,&gpio,sizeof(gpio),NULL,0,&bReturn,NULL);
}









wchar_t g_szHwId[MAX_PATH]={0};

#ifdef _DEBUG
#define ERROR_MSG(_msg_) printf _msg_
#else
#define ERROR_MSG(_x_) 
#endif 

#define R_CHANNEL_HID_SYMBOLLINK      _T("VID_0572&PID_1410")

namespace CXI2CBAG{
    void* __stdcall OpenI2cDevice(I2C_DEVICE  device, const wchar_t * /*szHWID*/)
    {
        void * hI2cHandle = NULL;

        switch(device)
        {
        case I2C_DEVICE_SIENA:
            {
                CXI2CINTERFACE *pI2c = SienaOpenI2cDevice();
                hI2cHandle =(HANDLE)  pI2c;
                if(pI2c)
                {
                    pI2c->SetI2cSpeed(pI2c,true);
                }
                
            }
            break;

        default:
            ERROR_MSG(("Open I2c failed, the deivce type is not supported!"));
            
        }
        return hI2cHandle;
    }
    void __stdcall CloseI2cDevice(void* hI2cDevice)
    {
        CXI2CINTERFACE *pI2c = (CXI2CINTERFACE*) hI2cDevice;
        if( pI2c)
        {
            pI2c->CloseDevice(pI2c);
        }
    }
    int __stdcall I2cWrite(void* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf, unsigned char* pBuf)
    {   
        CXI2CINTERFACE *pI2c = (CXI2CINTERFACE*) hI2cDevice;
        if( pI2c)
        {
            return pI2c->I2cWrite(pI2c,  ChipAddr,  cbBuf, pBuf);
        }
			return 0;

    }
    int __stdcall I2cWriteThenRead(void* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf,
        unsigned char* pBuf, unsigned long cbReadBuf, unsigned char*pReadBuf)
    {
        CXI2CINTERFACE *pI2c = (CXI2CINTERFACE*) hI2cDevice;
        if( pI2c)
        {
            return pI2c->I2cWriteThenRead(pI2c,  ChipAddr, cbBuf, pBuf,  cbReadBuf, pReadBuf);
        }
		return 0;
    }
    int __stdcall SetI2cSpeed(void * hI2cDevice,bool  b400Khz)
    {
        CXI2CINTERFACE *pI2c = (CXI2CINTERFACE*) hI2cDevice;
        if( pI2c)
        {
            return pI2c->SetI2cSpeed(pI2c,b400Khz);
        }
    }
    int __stdcall GpioSetPort(void* hI2cDevice,
        unsigned char portNumber,
        unsigned char portDirection,
        unsigned char portValue)
    {
        CXI2CINTERFACE *pI2c = (CXI2CINTERFACE*) hI2cDevice;
        if( pI2c)
        {
            return pI2c->GpioSetPort(pI2c,
                portNumber,
                portDirection,
                portValue);
        }
        return false;
    }
    int __stdcall GpioSetPin(void* hI2cDevice,
        unsigned char portNumber,
        unsigned char pinNumber,
        unsigned char pinValue)
    {
        CXI2CINTERFACE *pI2c = (CXI2CINTERFACE*) hI2cDevice;
        if( pI2c)
        {
            return pI2c->GpioSetPin(pI2c,
                portNumber,
                pinNumber,
                pinValue);
        }
        return false;
    }


}




