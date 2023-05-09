////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Synaptics that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, SYNAPTICS
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2018 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_DRIVER_H_
#define _MCU_DRIVER_H_

#include <mcu.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MCU_DRIVERS MAJOR_LAST

#define DECLARE_STANDARD_DEVICE_API                       \
    int (*_init)(void);                                   \
    int (*_open)(uint32_t dev, uint32_t flags);           \
    int (*_close)(uint32_t dev);                          \
    int (*_ioctl)(uint32_t dev, uint32_t cmd, void *param);

#define DEFINE_STANDARD_DEVICE_API(dev) \
    dev##_init,     \
    dev##_open,     \
    dev##_close,    \
    dev##_ioctl

typedef struct drvr_s
{
    DECLARE_STANDARD_DEVICE_API;
} DRVR_API_T;


typedef struct MCU_Driver_s
{
    unsigned int init;
    DRVR_API_T *drv_ops[MAX_MCU_DRIVERS];
}MCU_Driver_t;

extern int _sys_driver_interface_init(void);
extern int _sys_register_driver(uint32_t dev, DRVR_API_T *ops);
extern int _sys_unregister_driver(uint32_t dev);
extern int _sys_open(uint32_t dev, uint32_t flags);
extern int _sys_close(uint32_t dev);
extern int _sys_ioctl(uint32_t dev, uint32_t cmd, void *param);
extern int _sys_ioctl_plain(uint32_t dev, uint32_t cmd, uint32_t param);
extern struct drvr_s *_sys_get_driver_info(uint32_t dev);

#define os_driver_interface_init    _sys_driver_interface_init
#define os_open                     _sys_open
#define os_close                    _sys_close
#define os_ioctl                    _sys_ioctl
#define os_ioctl_plain              _sys_ioctl_plain
#define os_register_driver          _sys_register_driver
#define os_unregister_driver        _sys_unregister_driver
#define os_get_driver_info          _sys_get_driver_info

//--------------------------------------------- IO standard commands
#define DRV_STD_CMD_PWR_CTRL        1
#define DRV_STD_CMD_SET_TX          2
#define DRV_STD_CMD_SET_RX          3
#define DRV_STD_CMD_CLEAR_TX        4
#define DRV_STD_CMD_CLEAR_RX        5
#define DRV_STD_CMD_FLUSH           6
#define DRV_STD_CMD_OBTAIN_MUTEX    7
#define DRV_STD_CMD_RELEASE_MUTEX   8
#define DRV_STD_CMD_USER_SPACE      32
//---------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // _MCU_DRIVER_H_
