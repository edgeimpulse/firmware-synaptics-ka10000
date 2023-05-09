/****************************************************************************************
*****************************************************************************************
***                                                                                   ***
***                                 Copyright (c) 2007                                ***
***                                                                                   ***
***                                Conexant Systems, Inc.                             ***
***                                                                                   ***
***                                 All Rights Reserved                               ***
***                                                                                   ***
***                                    CONFIDENTIAL                                   ***
***                                                                                   ***
***               NO DISSEMINATION OR USE WITHOUT PRIOR WRITTEN PERMISSION            ***
***                                                                                   ***
*****************************************************************************************
**
**  File Name:
**      macro.h
**
**  Abstract:
**      
**
**  Product Name:
**      Conexant High Definition Audio Device.
**
**  Environment:
**      Hardware Requirements
**          Hermosa Codec
**      Application requirements
**          None
**      Driver requirements
**          Version 4.33.0.0 or above
**      Operating System requirements
**          Microsoft Windows* XP or Later
**      DDK requirements
**          Microsoft WDK build 600 and later
**      SDK requirements
**          Microsoft DirectX SDK (August 2007 release).
**      OS level
**          User mode only
**
**  Testing Requirements:
**      Proper installation of the driver in the system.
**
********************************************************************************
**  Revision History    
**      Date        Description                                 Author
**      11/15/07    Created.                                    Simon Ho
********************************************************************************
*****************************************************************************************/
#pragma once



#define ARRAY_SIZE(p) (sizeof(p)/sizeof((p)[0]))


#define WHILE(a) \
while(__pragma(warning(disable:4127)) a __pragma(warning(disable:4127)))


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { free (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_COFREE
#define SAFE_COFREE(p)  {if(p) { CoTaskMemFree(p);(p)=NULL;}}
#endif 

#ifndef SAFE_REGCLOSE
#define SAFE_REGCLOSE(p)  {if(p) { ::RegCloseKey(p);(p)=NULL;}}
#endif 

#ifndef SAFE_CLOSEHANDLE
#define SAFE_CLOSEHANDLE(p) {if(p&& p!= INVALID_HANDLE_VALUE) { ::CloseHandle(p);(p)=NULL;}}
#endif 

#ifndef SAFE_CLOSESERVICEHANDLE
#define SAFE_CLOSESERVICEHANDLE(p) {if(p) { ::CloseServiceHandle(p);(p)=NULL;}}
#endif 


#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )

const static UINT INVALID_UINT_ID = (UINT)-1;	

#define HRESULTTOWIN32(hres)                                \
            ((HRESULT_FACILITY(hres) == FACILITY_WIN32)     \
                ? HRESULT_CODE(hres)                        \
                : (hres))

//-------------------------------------------------------------------------
// Description:
//
// Closes a handle and assigns NULL.
//
// Parameters:
//
//      h - [in] handle to close
//
#define SAFE_CLOSE_HANDLE(h)                                    \
    if (NULL != h)                                              \
    {                                                           \
        CloseHandle(h);                                         \
        h = NULL;                                               \
    }
