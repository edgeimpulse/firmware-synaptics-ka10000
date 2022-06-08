#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
#ifndef __DOMAIN_NS
#define __DOMAIN_NS
#endif

#include "mcu.h"
#include "driver.h"
#include "uart_drv.h"
#include "usr_demo.h"
#include "ctrl.h"
#include "base_module_ex.h"
#ifdef CONFIG_LLE_DEMO_IPC
#include "ipc_drv.h"
#endif
#include "os_timer.h"
#include "helper.h"
#include "config.h"
#include "userconf.h"
#include "mem_alloc_al.h"
#if (defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS))
#if (defined(MCU_SDK_TENSAIFLOW_DEMO) && (MCU_SDK_TENSAIFLOW_DEMO == 1))

typedef enum
{
    TENSAI_RESERVED = 0,
    TENSAI_IMG_PRINT_ENABLE = 1,
} TensaiCmdCode;

/* this will setup executor framework*/
int fwk_init(void);
void infer(uint32_t *, uint32_t *);

void tensaiflow_demo_run(void *pvParam)
{
    int ret = 0;
    uint32_t time, cycles;
    unsigned long t1, c1, t2, c2;

    ret = fwk_init();
    if (ret)
        Print_DBG("Failed to initialize executor framework\n");
    while (1)
    {
        Print_DBG("Begin infer test\n\r");
        t1 = os_GetTickCount();
        c1 = GET_CYCLE_CNT;
        infer(&time, &cycles);
        t2 = os_GetTickCount();
        c2 = GET_CYCLE_CNT;
        Print_DBG("total infer time in %d msec, in cycles %d\n\r", time, cycles);
#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
        Print_DBG("total time (capture + preprocess image + infer) in %d msec, in cycles %d\n\r", t2 - t1, c2 - c1);
#else
        Print_DBG("total time (file read + infer) in %d msec, in cycles %d\n\r", t2 - t1, c2 - c1);
#endif
        os_TaskSleep(100);
    }
}

int TensaiflowAppCommand(Command *cmd)
{
    int ret_val = MODULE_RC_SUCCESS;
    switch ((TensaiCmdCode)(CMD_ITEM(cmd->head.command_id)))
    {
    case TENSAI_IMG_PRINT_ENABLE:
    {
#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
        if (CMD_SET(cmd->head.command_id) == cmd->head.command_id)
        {
            ret_val = printImageEnable(&(cmd->data[0]), 1);
        }
        else //command get
        {
            ret_val = printImageEnable(&(cmd->data[0]), 0);
        }
#endif //#ifdef CONFIG_MCU_SDK_CAMERA_ENABLE
    }
    break;
    default:
        ret_val = -EINVAL;
        break;
    }
    return (ret_val);
}

long tensaiflow_demo_hook(void *pvParam)
{
    os_TaskHookMsg *EventMsg = (os_TaskHookMsg *)pvParam;
    switch (EventMsg->msg)
    {
    case SIG_CMD:
    {
        Command *cmd = (Command *)EventMsg->parm;
        cmd->head.num_32b_words = TensaiflowAppCommand(cmd);
        cmd->head.reply = 1;
    }
    break;
    default:
        return -1;
    }
    return 0;
}

int tensaiflow_demo_start(void)
{
    os_TaskInit testInit;
    TaskHandle_t tHandle;

    memset(&testInit, 0, sizeof(os_TaskInit));

    testInit.ContextID = _ID('T', 'E', 'S', 'A');
    testInit.TaskFunc = tensaiflow_demo_run;
    testInit.TaskHookFunc = tensaiflow_demo_hook;
    testInit.TaskName = "compiler_test_app";
    testInit.TaskStackSize = (1024 - 128); //1024U;
    testInit.TaskPriority = OS_TASK_MID_PRIORITY;
    testInit.TaskTZModule = 0;

    tHandle = os_TaskCreate(&testInit);
    if (tHandle == 0)
    {
        Print_DBG("CREATE task failed\n ");
    }

    return 0;
}

#endif //(defined(MCU_SDK_TENSAIFLOW_DEMO) && (MCU_SDK_TENSAIFLOW_DEMO == 1))
#endif //(defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS))
