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

  File Name: AudioSmart_usb_interface_api_winhid.c

  Description: Implementation of helper functions over Windows hid.dll.

------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>

#include "syna_types.h"
#include "AudioSmart_usb_interface_api.h"

#define msleep(msecs) Sleep(msecs)

#ifdef __cplusplus
extern "C" {
#endif

#include <setupapi.h>
//hid.dll header
#include "hidsdi.h"

#ifdef __cplusplus
} // extern "C"
#endif

static HANDLE m_DeviceHandle = INVALID_HANDLE_VALUE;
static int m_DevisIsOpen = 0;

int AudioSmart_usb_interface_init(unsigned int vid, unsigned int pid)
{
    GUID HidGuid;
    SP_DEVICE_INTERFACE_DATA devInfoData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;
    HIDD_ATTRIBUTES Attributes;
    HDEVINFO hDevInfo;
    BOOL LastDevice;
    int MemberIndex;
    LONG Result;
    ULONG Length;
    ULONG Required;
    PHIDP_PREPARSED_DATA PreparsedData;

    HIDP_CAPS m_HidCaps;
    AUDIOSMART_DEBUG_PRINT(stdout, "\n\t Using win_hid API which is based on hid.dll of windows kernel.\n\n");
    if (!m_DevisIsOpen)
    {
        // Open device
        Length = 0;
        detailData = NULL;

        // Get the GUID for all system HIDs
        HidD_GetHidGuid(&HidGuid);

        hDevInfo =
            SetupDiGetClassDevs(
                &HidGuid,
                NULL,
                NULL,
                DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

        //Step through the available devices looking for the one we want.
        //Quit on detecting the desired device or checking all available devices without success.
        MemberIndex = 0;
        LastDevice = 0;
        if (INVALID_HANDLE_VALUE != hDevInfo)
        {
        do
        {
            devInfoData.cbSize = sizeof(devInfoData);

            Result =
                SetupDiEnumDeviceInterfaces(
                    hDevInfo,
                    0,
                    &HidGuid,
                    MemberIndex,
                    &devInfoData);
            if (Result != 0)
            {
            //Get the Length value.
            Result =
                SetupDiGetDeviceInterfaceDetail(
                    hDevInfo,
                    &devInfoData,
                    NULL,
                    0,
                    &Length,
                    NULL);

            //Allocate memory for the hDevInfo structure, using the returned Length
            detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, Length);

            if (detailData != NULL)
            {
                //Set cbSize in the detailData structure.
                detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

                //Call the function again, this time passing it the returned buffer size
                Result =
                    SetupDiGetDeviceInterfaceDetail(
                        hDevInfo,
                        &devInfoData,
                        detailData,
                        Length,
                        &Required,
                        NULL);

                // Open a m_DeviceHandle to the device.
                m_DeviceHandle =
                    CreateFile(
                        detailData->DevicePath,
                        0,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        (LPSECURITY_ATTRIBUTES)NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);

                if (m_DeviceHandle != INVALID_HANDLE_VALUE)
                {
                // Requests information from the device.
                Attributes.Size = sizeof(Attributes);

                Result =
                    HidD_GetAttributes(
                        m_DeviceHandle,
                        &Attributes);

                CloseHandle(m_DeviceHandle);
                m_DeviceHandle = INVALID_HANDLE_VALUE;

                if (Result)
                {
                    if ((Attributes.VendorID == vid) && (Attributes.ProductID == pid))
                    {
                    m_DeviceHandle =
                        CreateFile(
                            detailData->DevicePath,
                            GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            0,
                            OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED,
                            0);

                    if (m_DeviceHandle != INVALID_HANDLE_VALUE)
                    {

                        if (HidD_GetPreparsedData(m_DeviceHandle, &PreparsedData))
                        {
                        if (HIDP_STATUS_SUCCESS == HidP_GetCaps(PreparsedData, &m_HidCaps))
                        {
                            //AUDIOSMART_DEBUG_PRINT(stdout, "Input Report Byte Length : %d\n", m_HidCaps.InputReportByteLength);
                            //AUDIOSMART_DEBUG_PRINT(stdout, "Output Report Byte Length: %d\n", m_HidCaps.OutputReportByteLength);

                            if (m_HidCaps.InputReportByteLength > 2 && m_HidCaps.InputReportByteLength > 2)
                            {
                            m_DevisIsOpen = 1;
                            }
                        }

                        HidD_FreePreparsedData(PreparsedData);
                        }

                        if (!m_DevisIsOpen)
                        {
                        CloseHandle(m_DeviceHandle);
                        m_DeviceHandle = INVALID_HANDLE_VALUE;
                        }
                    }
                    }
                }
                }
                else
                {
                AUDIOSMART_DEBUG_PRINT(stdout, "ERROR: Can not open device vid=0x%x, pid=0x%x\n", vid, pid);
                }
            }
            //Free the memory used by the detailData structure (no longer needed).
            LocalFree(detailData);
            }
            else
            {
            AUDIOSMART_DEBUG_PRINT(stdout, "ERROR: Can not find device\n");
            break;
            }

            MemberIndex++;
        } while (!m_DevisIsOpen);
        SetupDiDestroyDeviceInfoList(hDevInfo);
        }
    }

  return m_DevisIsOpen ? 0 : -1;
}

int AudioSmart_usb_interface_write(unsigned int *data, int size)
{
  int r = 0;
  unsigned char hid_buffer[256] = {0};

  if (m_DeviceHandle == NULL)
    return -1;

  //Two Bytes Report ID.
  hid_buffer[0] = 1;
  hid_buffer[1] = 0;
  memcpy(hid_buffer + 2, data, size);
  size += 2;

  r = HidD_SetOutputReport(m_DeviceHandle, hid_buffer, size);
  if (r == 0)
  {
    AUDIOSMART_DEBUG_PRINT(stdout, "Error: failed to HidD_SetOutputReport - %d, (%08X)\n", r, GetLastError());
    return -1;
  }

  return r;
}

int AudioSmart_usb_interface_read(unsigned *data, int size)
{
  int r = 0;
  unsigned char hid_buffer[256] = {0};
  if (m_DeviceHandle == NULL)
    return -1;

  //Two Bytes Report ID.
  hid_buffer[0] = 1;
  hid_buffer[1] = 0;
  size += 2;

  r = HidD_GetInputReport(m_DeviceHandle, hid_buffer, size);
  if (r == 0)
  {
    AUDIOSMART_DEBUG_PRINT(stdout, "Error: failed to HidD_GetInputReport - %d, (%08X)", r, GetLastError());
    return -1;
  }

  memcpy(data, hid_buffer + 2, size - 2);
  return r;
}

int AudioSmart_usb_interface_close()
{
  if (m_DevisIsOpen)
  {
    // Close device
    CancelIo(m_DeviceHandle);
    CloseHandle(m_DeviceHandle);

    m_DeviceHandle = INVALID_HANDLE_VALUE;
    m_DevisIsOpen = 0;
  }
  return 0;
}
