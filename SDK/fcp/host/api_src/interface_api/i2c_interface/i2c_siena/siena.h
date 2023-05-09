
#include "i2c.h"

#ifndef _SIENA_H_
#define _SIENA_H_
CXI2CINTERFACE *SienaOpenI2cDevice();
int  SienaSetI2cSpeed(CXI2CINTERFACE* hI2cDevice, bool b400Khz);

namespace CXI2CBAG
{

    enum I2C_DEVICE{
        I2C_DEVICE_CHANNEL_HID   = 1,
        I2C_DEVICE_SIENA         = 2,
        I2C_DEVICE_USB2ISP       = 3, 
    };

    void* __stdcall OpenI2cDevice(I2C_DEVICE device, const wchar_t * szHWID);
    void __stdcall CloseI2cDevice(void* hI2cDevice);
    int __stdcall I2cWrite(void* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf, unsigned char* pBuf);
    int __stdcall I2cWriteThenRead(void* hI2cDevice, unsigned char ChipAddr, unsigned long cbBuf,
        unsigned char* pBuf, unsigned long cbReadBuf, unsigned char*pReadBuf);
    int __stdcall SetI2cSpeed(void * hI2cDevice,bool  b400Khz);  
    int __stdcall GpioSetPort(void* hI2cDevice,
        unsigned char portNumber,
        unsigned char portDirection,
        unsigned char portValue);
    int __stdcall GpioSetPin(void* hI2cDevice,
        unsigned char portNumber,
        unsigned char pinNumber,
        unsigned char pinValue);


    const unsigned int  MAX_SIENA_WRITE_LEN = 125;
};


#endif
