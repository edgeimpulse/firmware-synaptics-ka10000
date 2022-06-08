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
/**
 * \file i3cm_drv.h
 * \brief Those APIs will be used for I3C/I2C configuration.
 * \defgroup Programming_API_I3CM
 * \ingroup Programming_API_I3CM
 * @{
 *
 *
 * i3cm_drv.h
 * 
 * \section Introduction
 * The purpose of those APIs is used for I3C/I2C function\n
 * 
 * \section Usage
 * \b Step 1:
 * Understand the I2C slave's protocol according to your slave device's datasheet.
 * 
 * \b Step 2: 
 * Call i2cm_xfer_write when write data to i2c slave device \n
 * Call i2cm_xfer_read when read data from i2c slave device \n
 * \section Example
 * Please refer to i2cmx_demo.c under m33/app/ for more detail example code.
 * 
 *
 */
 
/** @} */ 
#ifndef _MCU_I3CM_DRV_H_
#define _MCU_I3CM_DRV_H_

#include "mcu.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined (CONFIG_MCU_DRV_I3CM)

enum I3CM_IO_CTRL_E{
	I3CM_IO_CTRL_SET_SDA_TX_HOLD =1,
    I3CM_IO_CTRL_SET_CORE_CLOCK  =2,
};

typedef void (*i3cm_callback_t)(void *data);

typedef struct{
    uint8_t is_legacy_i2c;
    uint8_t retry;
    uint8_t dynamic_addr;
    uint8_t static_addr;
}i3c_DAT_t;

struct i3c_msg {
    uint8_t index;  /*address table index*/ 
    uint8_t count;  /*msg count or address aasignment count when DAA */ 
    uint8_t ccc_cmd; // ccc cmd
    uint8_t hdr_cmd; // hdr cmd
    uint32_t xfer_type; // xfer type: get/set/private
    unsigned short len;      /* msg length               */
    uint8_t *buf;      /* pointer to msg data          */
    uint8_t slave_addr ; /*slave_address*/ 
    uint8_t is_legacy_i2c;
    uint32_t i2c_od_speed ; /* legacy i2c bus speed */ 
};

typedef struct i3cm_ops_s
{
    DECLARE_STANDARD_DEVICE_API;
    int32_t (*i3c_setup_address_table)(uint32_t dev, uint32_t table_idx, i3c_DAT_t *entry);
#ifdef CONFIG_MCU_I3C_MASTER_I3C_PROTOCOL_SUPPORT
    int32_t (*i3c_get_address_table)(uint32_t dev, uint32_t table_idx, i3c_DAT_t *entry);
    int32_t (*i3c_master_find_idx_by_addr)(uint32_t dev, uint8_t addr, uint8_t static_flag);
#endif
    int32_t (*i3cm_xfer)(uint32_t dev, struct i3c_msg msgs[], int32_t num);
    int32_t (*i2cm_xfer_write)(uint32_t dev, uint8_t slave_address,uint32_t speed_hz, uint8_t *data, int32_t byte_size);
    int32_t (*i2cm_xfer_read)(uint32_t dev, uint8_t slave_address, uint32_t speed_hz,uint8_t *data, int32_t byte_size);
} i3cm_ops_t;

static inline i3cm_ops_t *i3cm_driver_info(uint32_t dev)
{
    return (i3cm_ops_t *)os_get_driver_info(dev);
}

static inline int32_t i3cm_open(uint dev, uint flags)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
        return op->_open(dev, flags);
    return -ENODEV;
}

static inline int32_t i3cm_close(uint dev)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
        return op->_close(dev);
    return -ENODEV;
}

static int32_t i3cm_xfer(uint32_t dev, struct i3c_msg msgs[], int32_t num)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->i3cm_xfer(dev, msgs, num);
    }
    return -ENODEV;
}

/**
*
* @fn       static int32_t i2cm_xfer_write(uint32_t dev, uint8_t slave_address, uint32_t speed_hz,uint8_t *data, int32_t byte_size)
* @brief 	Write data bytes to external slave device,  speed 100/400KHz
* @param 	dev is driver's device number ,always DEV_I3CM0
* @param 	slave_address is the i2c slave address 
* @param 	speed_hz is the i2c master speed (Hz) 
* @param 	data is buffer poiter for data to be sent
* @param 	byte_size is data size/bytes to be sent, should <= 64bytes
* @return 	actual parameter value or negative error code from errno.h
*/
static int32_t i2cm_xfer_write(uint32_t dev, uint8_t slave_address,uint32_t speed_hz, uint8_t *data, int32_t byte_size)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->i2cm_xfer_write(0, slave_address,speed_hz, data, byte_size);
    }
    return -ENODEV;
}

/**
*
* @fn       static int32_t i2cm_xfer_read(uint32_t dev, uint8_t slave_address,uint32_t speed_hz, uint8_t *data, int32_t byte_size)
* @brief 	Write data bytes to external slave device, speed 100/400KHz
* @param 	dev is driver's device number ,always DEV_I3CM0
* @param 	slave_address is the i2c slave address 
* @param 	speed_hz is the i2c master speed (Hz) 
* @param 	data is buffer poiter for data to be read
* @param 	byte_size is data size/bytes to be read should <= 64bytes
* @return 	actual parameter value or negative error code from errno.h
*/
static int32_t i2cm_xfer_read(uint32_t dev, uint8_t slave_address,uint32_t speed_hz, uint8_t *data, int32_t byte_size)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->i2cm_xfer_read(0, slave_address, speed_hz, data, byte_size);
    }
    return -ENODEV;
}


/**
*
* @fn       static int32_t i2cm_xfer_config(uint32_t dev, uint32_t ctl, void *parm)
* @brief 	set up the key configuration for i2c transfer
* @param 	dev is driver's device number,always DEV_I3CM0
* @param 	ctl is the supported configuration id in the enum I3CM_IO_CTRL_E
* @param 	parm is the pointer to the parameter values
* @return 	actual parameter value or negative error code from errno.h
*/
static int32_t i2cm_xfer_config(uint32_t dev, uint32_t ctl, void *parm)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->_ioctl(0,ctl,parm);
    }
    return -ENODEV;
}

static int32_t i3c_setup_address_table(uint32_t dev, uint32_t table_idx, i3c_DAT_t *entry)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->i3c_setup_address_table(dev, table_idx, entry);
    }
    return -ENODEV;
}

#ifdef CONFIG_MCU_I3C_MASTER_I3C_PROTOCOL_SUPPORT
static int32_t i3c_get_address_table(uint32_t dev, uint32_t table_idx, i3c_DAT_t *entry)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->i3c_get_address_table(dev, table_idx, entry);
    }
    return -ENODEV;
}

static int32_t i3c_master_find_idx_by_addr(uint32_t dev, uint8_t addr, uint8_t static_flag)
{
    i3cm_ops_t *op;
    if ((op = i3cm_driver_info(dev)) != NULL)
    {
        return op->i3c_master_find_idx_by_addr(dev, addr, static_flag);
    }
    return -ENODEV;
}
#endif

int32_t drv_i3cm_start(void);

#endif // CONFIG_MCU_DRV_I3CM

#ifdef __cplusplus
}
#endif

#endif // _MCU_I3CM_DRV_H_

