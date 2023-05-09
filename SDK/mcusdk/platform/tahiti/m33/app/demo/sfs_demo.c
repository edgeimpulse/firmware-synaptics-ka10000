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
#if (defined(MCU_SDK_SFS_DEMO) && (MCU_SDK_SFS_DEMO == 1))
#include "sfs_drv.h"

os_TaskHandle sfs_task_handle;

/*
    WARN: This code need to test with the katana_TEST_raw_data.blob under scripts
*/
void sfs_demo_task(void *param)
{
    Print_ERR("SFS Demo\n");

    //----------- SFS Demo -----------
    int err = 0;
    uint32_t id = _ID('T', 'E', 'S', 'T');
    sfs_file_info_t info;
    memset(&info, 0, sizeof(sfs_file_info_t));


    os_TaskSleep(6000);
    
    err = sfs_fopen(id, &info);
    if (err != 0)
    {
        Print_ERR("Error: Sfs open error:%d\n", err);
    }
    
    //uint8_t data[32];
    uint8_t *data;
    data = (uint8_t *)0x41518000;
    
    for (int k = 0; k < 32; k++)
    {
        data[k] = 0x55;
    }

    uint32_t a = DWT->CYCCNT;
    err = sfs_fread(&info, data, 262144);
    uint32_t b = DWT->CYCCNT;
    if (err != 262144)
    {
        Print_ERR("Error: Sfs read error:%d\n", err);
    }

    err = sfs_fclose(&info);
    if (err != 0)
    {
        Print_ERR("Error: Sfs close error:%d\n", err);
    }

    Print_ERR("Time=%d\n", b-a);

    for (int k = 0; k < 32; k++)
    {
        Print_ERR("0x%x ", data[k]);
    }
    Print_ERR("\n----\n");

    for (int k = 262100; k < (262100 + 32); k++)
    {
        Print_ERR("0x%x ", data[k]);
    }
    Print_ERR("\n");

    while (1)
    {

        os_TaskSleep(1000);
    }
}

int sfs_demo_init(void)
{
    os_TaskInit TaskInit;
    memset(&TaskInit, 0, sizeof(TaskInit));

    TaskInit.ContextID     = _ID('S', 'F', 'S', 'T');
    TaskInit.TaskFunc      = sfs_demo_task;
    TaskInit.TaskHookFunc  = NULL;
    TaskInit.TaskName      = "SFST";
    TaskInit.TaskStackSize = 256;
    TaskInit.TaskPriority  = OS_TASK_HIGH_PRIORITY;
    TaskInit.TaskTZModule  = 2U;

    sfs_task_handle = os_TaskCreate(&TaskInit);

    if (sfs_task_handle == NULL)
    {
        Print_ERR("Create Task failed\n");
        return -1;
    }
}

#endif //MCU_SDK_SFS_DEMO
