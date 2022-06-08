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
#ifndef _PM_LITE_DRV_H
#define _PM_LITE_DRV_H

#ifdef CONFIG_MCU_PM_LITE
//--------------------------------- Defines ------------------------------------------

typedef enum {
	CLK_SOURCE_BCLK0,
	CLK_SOURCE_BCLK1,
	CLK_SOURCE_RCOSC_STD,
	CLK_SOURCE_RCOSC_LP,
	CLK_SOURCE_PLL_BCLK0,
	CLK_SOURCE_PLL_BCLK1,
	CLK_SOURCE_PLL_RCOSC_STD,
	CLK_SOURCE_PLL_RCOSC_LP,
	CLK_SOURCE_PLL_EARLINK,
	CLK_SOURCE_CODEC_CLK,
} PM_CLK_SOURCE;

typedef struct pm_clk_cfg_tag
{
    uint32_t clk_src;               //SOURCE_RCOSC_STD/SOURCE_PLL_RCOSC_STD
    uint32_t clk_src_freq;          //1.536MHz, 3.072MHz, 6.144MHz, 12.288MHz, 24.576MHz, 36MHz, 48MHz...
    uint32_t arm_freq;              //Cortex-M33 frequency
    uint32_t ahb_freq;              //AHB freq
    uint32_t hifi_freq;             //HIFI3 freq
    union
    {
        uint32_t d32;
        struct
        {
            uint32_t cape_a  : 1;   //CAPE-A core enable - shared memory will only be enabled if CAPE-A/B active
            uint32_t cape_b  : 1;   //CAPE-B core enable - shared memory will only be enabled if CAPE-A/B active
            uint32_t lle_a   : 1;   //LLE-A core enable - vector memory will only be enabled if LLE-A/B active
            uint32_t lle_b   : 1;   //LLE-B core enable - vector memory will only be enabled if LLE-A/B active
            uint32_t hifi3   : 1;   //Hifi3 core enable
            uint32_t reserved: 27;
        }b;
    }coproc_state;                  
    uint32_t capea_freq;            //CAPE-A freq
    uint32_t capeb_freq;            //CAPE-B freq
    uint32_t lle_freq;              //LLE-A, LLE-B freq
    uint32_t codec_clk_src;         //SOURCE_RCOSC_STD, SOURCE_PLL_RCOSC_STD, SOURCE_BCLKx, SOURCE_PLL_BCLKx
    uint32_t codec_clk_src_freq;    //Only support 0, 512KHz, 1.536MHz, 3.072MHz,  44.1KHz will be added later
}pm_clk_tree_s;

/*! clock tree update flags:*/
#define PM_CLK_CHG_ALL          0x0     /* Update all clock tree*/
#define PM_CLK_CHG_CODEC        0x01    /* Only update codec part in a clock tree */
#define PM_CLK_CHG_PROC_FREQ    0x02    /* Only update processor frequency part in a clock tree */
#define PM_CLK_CHG_PROC_EN      0x04    /* Only update processor  */

/**
 * @brief User callback function type 
 *  param: user pre-configed data and now send back as input parameter
 *  *cfg:  current clock tree
 */
typedef void (*pm_user_cb)(int param, pm_clk_tree_s *cfg);

typedef struct pm_callback_tag
{
    pm_user_cb  cb;                 //User callback funtion pointer
    int         param;              //parameter send to callback when executed, the param will set to -1 when the force boost mode is triggered
}pm_callback_s;

typedef struct pm_set_clk_tag
{
    pm_clk_tree_s   clk_tree;               //User setup clock tree
    pm_callback_s   pre_clk_chg_callback;   //User callback *BEFORE* clock update, could used to setup peripherals or applications
    pm_callback_s   post_clk_chg_callback;  //User callback *AFTER* clock update, could used to setup peripherals or applications
}pm_set_clk_s;


#define PM_LP_WAKE_SRC_GPIO     0x1
#define PM_LP_WAKE_SRC_AVAD     0x2
//#define PM_LP_WAKE_SRC_TIMER    0x4   //TODO: TBD

typedef struct wake_cfg_tag
{
    uint32_t gp_pin_mask;
    uint32_t timer_val;
}wake_cfg_s;


struct pm_lite_ops_s
{
    DECLARE_STANDARD_DEVICE_API;
    int (*_pm_set_clocks)(pm_set_clk_s *cfg, int update_flag);
    int (*_pm_get_clocks)(pm_clk_tree_s *clk);
    int (*_pm_set_lowpower)(int wake_source, wake_cfg_s wake_cfg, pm_callback_s wake_cb);
};

extern struct pm_lite_ops_s _pm_lite_ops;

//--------------------------------- Functions ------------------------------------------
STATIC_INLINE struct pm_lite_ops_s *pm_lite_driver_info()
{
    return (struct pm_lite_ops_s *)os_get_driver_info(DEV_PM_LITE);
}


/**
 * @brief Send message to PM to change the clock according to the clock tree
 * 
 * @param cfg : clock tree and the pre/post clock change callback
 * @param update_flag : PM_CLK_CHG_CODEC     => Only update codec  part, ignore all other items in cfg
 *                      PM_CLK_CHG_PROC_FREQ => Only update processor part, ignore all other items in cfg
 *                      PM_CLK_CHG_PROC_EN   => Only update processor enable part, ignore all other items in cfg
 *                      0                    => Update whole cfg
 * @return STATIC_INLINE : 0 for success, negative value for failure
 */
STATIC_INLINE int pm_set_clocks(pm_set_clk_s *cfg, int update_flag)
{
    struct pm_lite_ops_s *op;
    if ((op = pm_lite_driver_info()) != NULL)
        return op->_pm_set_clocks(cfg, update_flag);
    return -ENODEV;
}

/**
 * @brief Read back current running clock tree
 * 
 * @param clk : clock structure pointer
 * @return STATIC_INLINE : 0 for success, negative value for failure
 */
STATIC_INLINE int pm_get_clocks(pm_clk_tree_s *clk)
{
    struct pm_lite_ops_s *op;
    if ((op = pm_lite_driver_info()) != NULL)
        return op->_pm_get_clocks(clk);
    return -ENODEV;
}

/**
 * @brief Set system falls to low power mode, all processor stop working unless one of 
 *          of PM_LP_WAKE_SRC_xxx event happened and will boost process back to the status
 *          before system goes to sleep and execute the wake callback
 * 
 * @param wake_source : refert to PM_LP_WAKE_SRC_xxxx
 * @param wake_cfg :    setup for gpin enabled pin mux or timer value
 *                          gp_pin_mask: gpio enable pin mask,  bit 0 - 11 indicate gpio0-gpio11, 
 *                              if pin_mask=0 gpio0 will be enabled by default
 *                          timer_val:   timer out count
 * @param wake_cb :     callback after wakeup
 * @return int : 0 = success; negative value for any failure
 */
STATIC_INLINE int pm_set_lowpower(int wake_source, wake_cfg_s wake_cfg, pm_callback_s wake_cb)
{
    struct pm_lite_ops_s *op;
    if ((op = pm_lite_driver_info()) != NULL)
        return op->_pm_set_lowpower(wake_source, wake_cfg, wake_cb);
    return -ENODEV;
}

#endif //CONFIG_MCU_PM_LITE
#endif
