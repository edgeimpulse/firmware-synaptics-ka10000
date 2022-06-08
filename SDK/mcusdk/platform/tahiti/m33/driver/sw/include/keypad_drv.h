////////////////////////////////////////////////////////////////////////////////
//
//  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Conexant Systems, Inc.
//  ("Conexant")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Conexant Systems Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Conexant that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, CONEXANT
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2009-2013 Conexant Systems Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_KEYPAD_INCLUDED_
#define _DRV_KEYPAD_INCLUDED_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_MCU_DRV_KEYPAD

// keypad symbolic function status
typedef enum
{
    KEY_UNINIT,
    KEY_UNAVAILABLE,
    KEY_READY,
    KEY_REFRESH,
} eKEY_STATUS;

#ifdef CONFIG_ENABLE_EIPC_KEYPAD_MODE
// key button id
typedef enum
{
    KEY_UNUSED=0xff,
    GPIO_0=0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    BTN0_ID,
    BTN1_ID,
    BTN2_ID,
    BTN3_ID,

    KEY_ID_MAX = 14,
    KEY_VOICE_ASSIST = 14,   //No need to poll VOICE_ASSIST since it is a virtual KEY for system.

    ENCODER_CW = 15,
    ENCODER_ACW = 16,
    KEY_UART_RX =17, 
    KEY_UART_TX =18,
    KEY_SFWUG   =19, 
} eKEY_ID;

#define KEY_FUNC_MAX KEY_ID_MAX
#define eKEY_FUNC    eKEY_ID
#else

// key logical function
typedef enum
{
    KEY_UNUSED,
    KEY_VOL_UP,
    KEY_VOL_DOWN,
    KEY_MIC_MUTE,
    KEY_ANC_CTRL,
    KEY_TEAMS_CTRL,
    KEY_MEDIA_CTRL,
    KEY_CUSTOM_0 = 7,
    KEY_CUSTOM_1 = 8,

    KEY_FUNC_MAX = 9,
    KEY_VOICE_ASSIST = 9,   //No need to poll VOICE_ASSIST since it is a virtual KEY for system.
    KEY_UART_RX = 17, 
    KEY_UART_TX =18,
} eKEY_FUNC;
#endif

#define ACTIVE_LEVEL_HIGH   1
#define ACTIVE_LEVEL_LOW    0

#define KEY_SEL_IO          100
#define KEY_ACTIVE_LEVEL    101
#define KEY_MADC_THRESHOLD  102
#define KEY_CALLBACK        103
#define KEY_REFRESH_STATE   104
#define KEY_POLLING_TIME    105

#define BTN_0_MIN           77  // SW6
#define BTN_0_MAX           145
#define BTN_1_MIN           146 // SW7
#define BTN_1_MAX           249
#define BTN_2_MIN           250 // SW8
#define BTN_2_MAX           415
#define BTN_3_MIN           416 // SW9
#define BTN_3_MAX           557

#define GPIO_UNDEF          0xFFFF

// keypad io type
typedef enum
{
    KEY_TYPE_GPIO,
    KEY_TYPE_MADC,
} eKEY_IO_TYPE;

// keypad command
typedef enum
{
    KEYPAD_GET_AVAILABLE_MAP_IDX,
} eKEY_CMD;

// keypad event
typedef enum
{
    KEY_NO_EVENT,
    KEY_SHORT_PRESS_EVENT,
    KEY_LONG_PRESS_EVENT,
    KEY_DOUBLE_PRESS_EVENT,
    KEY_PRESSED_EVENT,
    KEY_RELEASE_EVENT,
    KEY_TRIPLE_PRESS_EVENT,
    KEY_GPIO_IRQ
} eKEY_EVENT;

// keypad event callback message
typedef struct keypad_event_msg_s
{
    eKEY_FUNC       kfunc;
    eKEY_EVENT      kevent;
    unsigned int    ktime_duration;
    void            *context;
} keypad_event_msg;

// Keypad event callback function
typedef void (*keypad_event_cb_pf)(keypad_event_msg *keypad_msg);

typedef struct keypad_event_callback_s
{
    keypad_event_cb_pf  pfunc;
    keypad_event_msg    kmesg;
} keypad_event_callback;

typedef struct keymapctrlcfg_s
{
    uint func;
    uint button_cfg_type;
    int  data[8];
} keymapctrlcfg;

typedef struct keymapctrl_s
{
    eKEY_FUNC func;
    eKEY_IO_TYPE io_type;
    unsigned int gp;
    unsigned int active_level;
    unsigned int short_press_time;
    unsigned int long_press_time;
    unsigned int multi_press_time;
    unsigned int multi_press_debounce;
    unsigned int times_pressed;
    unsigned int triple_press_enabled;
    unsigned long multi_press_count;
    unsigned long multi_press_elapsed;
    unsigned int madc_active_min_value;
    unsigned int madc_active_max_value;
    keypad_event_callback callback;
} keymapctrl;

struct keypad_ops_s
{
    DECLARE_STANDARD_DEVICE_API
    int (*_set_func)(keymapctrl *keyfunc);
    int (*_update_func)(keymapctrlcfg *func_id);
};

static inline struct keypad_ops_s *keypad_driver_info(uint dev)
{
    return (struct keypad_ops_s *)os_get_driver_info(dev);
}

static inline int keypad_set_func(uint dev, keymapctrl *keyfunc)
{
    struct keypad_ops_s *ops = keypad_driver_info(dev);
    if (ops == NULL)
        return -ENOENT;
    return ops->_set_func(keyfunc);
}

static inline int keypad_update_func(uint dev, keymapctrlcfg *keyfunc)
{
    struct keypad_ops_s *ops = keypad_driver_info(dev);
    if (ops == NULL)
        return -ENOENT;
    return ops->_update_func(keyfunc);
}

extern void keypad_reset(void);
extern int drv_keypad_start(void);

#endif // CONFIG_MCU_DRV_KEYPAD

#ifdef __cplusplus
}
#endif

#endif // _DRV_KEYPAD_INCLUDED_
