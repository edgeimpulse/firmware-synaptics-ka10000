////////////////////////////////////////////////////////////////////////////////
//
// NDA AND NEED-TO-KNOW REQUIRED
//
// Copyright (C) 2021 Synaptics Incorporated. All rights reserved.
//
// This file contains information that is proprietary to Synaptics
// Incorporated ("Synaptics"). The holder of this file shall treat all
// information contained herein as confidential, shall use the
// information only for its intended purpose, and shall not duplicate,
// disclose, or disseminate any of this information in any manner
// unless Synaptics has otherwise provided express, written
// permission.
//
// Use of the materials may require a license of intellectual property
// from a third party or from Synaptics. This file conveys no express
// or implied licenses to any intellectual property rights belonging
// to Synaptics.
//
// INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
// SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
// INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
// INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
// CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
// OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
// BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
// COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
// DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
// TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
//
////////////////////////////////////////////////////////////////////////////////

/* ---------------------------------------------------------------------------------------------
	Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes.
		Valid options are:
			> ASSERT_DISABLE
			> ASSERT_LEVEL_ERR
			> ASSERT_LEVEL_WARN
			> ASSERT_LEVEL_INFO
			> ASSERT_LEVEL_ALL
*/
#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
/* --------------------------------------------------------------------------------------------- */
#ifndef __DOMAIN_NS
#define __DOMAIN_NS
#endif

#include "mcu.h"
#include "driver.h"
#include "usr_demo.h"
#if (defined(MCU_SDK_PM_DEMO) && (MCU_SDK_PM_DEMO == 1))
#include "pm_lite_drv.h"

os_TaskHandle pm_task_handle;
const pm_clk_tree_s pm_test_clock = 
{
    CLK_SOURCE_RCOSC_STD,   //sysclk source
    6144000,                //sysclk source freq
    6144000,                //M33 freq
    6144000,                //AHB freq
    6144000,                //HIFI3 freq
    0x0,                    //HIFI3, CAPE-A/B, LLE-A/B disable
    6144000,                //CAPE-A freq
    6144000,                //CAPE-B freq
    6144000,                //LLE freq
    CLK_SOURCE_RCOSC_STD,   //Codec/Analog clk source
    6144000                 //Codec/Analog clk freq
};

void pre_peripheral_ctrl(int param, pm_clk_tree_s *cfg)
{
    Print_DBG("Pre Clock Change\n");
}

void post_peripheral_ctrl(int param, pm_clk_tree_s *cfg)
{
    if(-1 == param)
        Print_DBG("Clock force boost due to I2C IRQ\n");
    
    Print_DBG("Post Clock Change\n");
}

void wakeup_user_func(int param, pm_clk_tree_s *cfg)
{
    Print_DBG("System Back\n");
}

void pm_demo_task(void *param)
{
    int err = 0;
    Print_ERR("PM Demo - delay 10s\n");
    os_TaskSleep(10000);

    pm_set_clk_s cfg;
    memcpy(&cfg.clk_tree, &pm_test_clock, sizeof(pm_clk_tree_s));

    cfg.pre_clk_chg_callback.cb     = pre_peripheral_ctrl;
    cfg.pre_clk_chg_callback.param  = 0x55;
    cfg.post_clk_chg_callback.cb    = post_peripheral_ctrl;
    cfg.post_clk_chg_callback.param = 0xAA;
    err = pm_set_clocks(&cfg, PM_CLK_CHG_ALL);

    os_TaskSleep(5000);
    
    Print_ERR("PM GPIO LPM\n");

    pm_callback_s usr_wake = {wakeup_user_func, 0x55};
    wake_cfg_s wake_cfg    = {0x1, 0};   //gpio 0, timer do not support yet 
    err = pm_set_lowpower(PM_LP_WAKE_SRC_GPIO, wake_cfg, usr_wake);

    while (1)
    {
        Print_ERR("M33 Alive\n");
        os_TaskSleep(5000);
    }
}

int pm_demo_init(void)
{
    os_TaskInit TaskInit;
    memset(&TaskInit, 0, sizeof(TaskInit));

    TaskInit.ContextID     = _ID('P', 'M', 'D', 'M');
    TaskInit.TaskFunc      = pm_demo_task;
    TaskInit.TaskHookFunc  = NULL;
    TaskInit.TaskName      = "PMDM";
    TaskInit.TaskStackSize = 128;
    TaskInit.TaskPriority  = OS_TASK_HIGH_PRIORITY;
    TaskInit.TaskTZModule  = 2U;

    pm_task_handle = os_TaskCreate(&TaskInit);

    if (pm_task_handle == NULL)
    {
        Print_ERR("Create Task failed\n");
        return -1;
    }
}

#endif //MCU_SDK_PM_DEMO
