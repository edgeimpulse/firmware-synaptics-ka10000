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
#if (defined(MCU_SDK_CAMERA_DEMO) && (MCU_SDK_CAMERA_DEMO == 1))
#include "camera_drv.h"

#ifdef CONFIG_CAMERA_SERIAL_MODE
    #define CM_BUF_SIZE 34992
    #define CM_TB_OFST  0
    #define CM_PT_SIZE  34992
#else
    #define CM_BUF_SIZE 26244*8
    #define CM_TB_OFST  26244*7
    #define CM_PT_SIZE  26244
#endif

#ifndef CONFIG_TENSAIFLOW
    #define CAMERA_DEMO_BUFFER_ADDR 0x41510000
#else
    #ifndef CONFIG_USER_KCONFIG_ACTIVE
        //Internal test for no Tensaiflow App running
        #define CAMERA_DEMO_BUFFER_ADDR 0x41510000
    #else
        /* NOTICE: Please define CAMERA_DEMO_BUFFER_ADDR
                Define target address in share memory before compile,
                if no Tensaiflow enabled, address 0x41510000 can be used any other 
                location can hold certain size image
                buf if Tensaiflow is enabled, shared buffer are controled by the
                tensai, and please be careful with the demos running on Tensaiflow
        */

        #ifndef CAMERA_DEMO_BUFFER_ADDR
            #error Please define CAMERA_DEMO_BUFFER_ADDR before build
        #endif
    #endif
#endif


os_SemHandle	cam_notify;
os_TaskHandle   cam_task_handle;


void user_callback(void *msg)
{
    //Print_ERR("Captured Image!!0x%x\n", (uint32_t)msg);
    //print_image(msg, 34992);
    os_SemGive(cam_notify);
}

void camera_demo_task(void *param)
{
    Print_ERR("Camera Demo\n");

    camera_drv_init();

    camera_setup_t cfg;
    cfg.buffer      = (uint32_t *)CAMERA_DEMO_BUFFER_ADDR;
    cfg.buffer_size = CM_BUF_SIZE; //To make sure hold up 324*324
    cfg.done_cb     = user_callback;
    camera_setup(&cfg);

    Print_ERR("Camera Wait\n");
    os_TaskSleep(15000); //Wait other core to finish startup print
    Print_ERR("Start 1st Sync & Capture\n");
    camera_capture(cfg.buffer, cfg.buffer_size);
    os_SemTake(cam_notify, OS_SEM_WAIT_FOREVER);
    Print_ERR("Start Capture loop\n");

    while(1)
    {
        camera_capture(cfg.buffer, cfg.buffer_size);
        os_SemTake(cam_notify, OS_SEM_WAIT_FOREVER);
        Print_ERR("Captured Image!!\n");
        print_image((uint32_t *)0x41510000 + CM_TB_OFST, CM_PT_SIZE);
        os_TaskSleep(3000);
    }
}

int camera_demo_init(void)
{
    os_TaskInit TaskInit;
    memset(&TaskInit, 0, sizeof(TaskInit));

    os_SemInit SemInit;
    SemInit.uSemType = SEM_BINARY;
    cam_notify       = os_SemCreate(&SemInit);

    TaskInit.ContextID     = _ID('C', 'A', 'M', 'D');
    TaskInit.TaskFunc      = camera_demo_task;
    TaskInit.TaskHookFunc  = NULL;
    TaskInit.TaskName      = "CAMD";
    TaskInit.TaskStackSize = 256;
    TaskInit.TaskPriority  = OS_TASK_HIGH_PRIORITY;
    TaskInit.TaskTZModule  = 2U;

    cam_task_handle = os_TaskCreate(&TaskInit);

    if (cam_task_handle == NULL)
    {
        Print_ERR("Create Task failed\n");
        return -1;
    }

    return 0;

}

#endif //CONFIG_MCU_DRV_CAMERA
