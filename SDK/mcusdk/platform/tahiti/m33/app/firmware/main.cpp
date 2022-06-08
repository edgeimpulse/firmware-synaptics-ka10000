
#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG
/* --------------------------------------------------------------------------------------------- */
#ifndef __DOMAIN_NS
#define __DOMAIN_NS
#endif

#include "mcu.h"
#include "driver.h"
#include "gpio_drv.h"
#include "madc_drv.h"
#include "command.h"
#include "AudioSmart_sendcmd_common.h"

#include "fdma_drv.h"
#include "uart_drv.h"
#include "usr_demo.h"
#include "fdma_drv.h"
#include "repl.h"

#include "ei_device_synaptics_ka10000.h"
#include "ei_synaptics_fs_commands.h"
#include "at_cmds.h"
#include "ei_camera.h"
#include "ei_run_impulse.h"

#define PRINT_LEVEL_SET ASSERT_LEVEL_DBG

/* Extern reference -------------------------------------------------------- */
extern int ei_main();
extern void ei_printf(const char *format, ...);
extern "C" int fwk_init(void);

#define UART_RX_PENDING       1

TimerHandle_t timer_handle;
uint64_t timer_ms = 0;

extern "C" {

void (timer_cb)( TimerHandle_t xTimer )
{
    if(xTimer == timer_handle) {
        timer_ms++;
    }
}

uint64_t get_time_ms(void)
{
    return timer_ms;
}

void ei_fw_demo_run(void *pvParam)
{

    int ret = 0;

    ret = fwk_init();
    if (ret)
        ei_printf("Failed to initialize executor framework\n");

    timer_handle = xTimerCreate("1msTimer", pdMS_TO_TICKS(1), pdTRUE, 0, timer_cb);
    xTimerStart(timer_handle, 0);

    ei_printf(
        "Hello from Edge Impulse Device SDK.\r\n"
        "Compiled on %s %s\r\n",
        __DATE__,
        __TIME__);

    ei_camera_init();

    /* Intialize configuration */
    static ei_config_ctx_t config_ctx = { 0 };
    config_ctx.get_device_id = EiDevice.get_id_function();
    config_ctx.get_device_type = EiDevice.get_type_function();
    config_ctx.wifi_connection_status = EiDevice.get_wifi_connection_status_function();
    config_ctx.wifi_present = EiDevice.get_wifi_present_status_function();
    config_ctx.load_config = &ei_synaptics_fs_load_config;
    config_ctx.save_config = &ei_synaptics_fs_save_config;
    config_ctx.list_files = NULL;
    config_ctx.read_buffer = EiDevice.get_read_sample_buffer_function();
    config_ctx.take_snapshot = &ei_camera_take_snapshot_encode_and_output;
    config_ctx.start_snapshot_stream = &ei_camera_start_snapshot_stream_encode_and_output;

    EI_CONFIG_ERROR cr = ei_config_init(&config_ctx);

    if (cr != EI_CONFIG_OK) {
        ei_printf("Failed to initialize configuration (%d)\n", cr);
    }
    else {
        ei_printf("Loaded configuration\n");
    }

    /* Setup the command line commands */
    ei_at_register_generic_cmds();
    ei_at_cmd_register("RUNIMPULSE", "Run the impulse", run_nn_normal);
    ei_at_cmd_register("RUNIMPULSEDEBUG=", "Run the impulse with extra (base64) debug output (USEMAXRATE?(y/n))", run_nn_debug);
    ei_at_cmd_register("RUNIMPULSECONT", "Run the impulse in continuous mode", run_nn_continuous_normal);
    ei_printf("Type AT+HELP to see a list of commands.\r\n> ");

    volatile uint8_t data;
    while(1) {
        data = uart_getchar();
        if(data != 0) {
            rx_callback(data);
        }
    }


}

long ei_fw_demo_hook(void *pvParam )
{
    os_TaskHookMsg* EventMsg = (os_TaskHookMsg*) pvParam;

    switch(EventMsg->msg)
    {
        case UART_RX_PENDING:
#ifdef CONFIG_MCU_DRV_UART_FDMA
            run_uart_loopback();
#endif
        break;
    }

    return 0;
}

int normal_main(void)
{

    os_TaskInit     usrTaskInit;
    memset(&usrTaskInit, 0, sizeof(os_TaskInit));

    usrTaskInit.ContextID     = _ID('U','I','M','O');
    usrTaskInit.TaskFunc      = ei_fw_demo_run;
    usrTaskInit.TaskHookFunc  = ei_fw_demo_hook;
    usrTaskInit.TaskName      = "edgeimpulse_fw_demo";
    usrTaskInit.TaskStackSize = 2048U;
    usrTaskInit.TaskPriority  = OS_TASK_MID_PRIORITY;
    usrTaskInit.TaskTZModule  = 0;

    // set up comms port
    EiDevice.set_default_data_output_baudrate();

    if (os_TaskCreate(&usrTaskInit) == 0)
    {
        ei_printf("CREATE_USER_MODE_TASK - edgeimpulse_fw_demo failed\n ");
    }

#ifndef CONFIG_CHIP_NAME_KATANA
    AudioSmart_mid_level_init(NULL); //want to use CmdNotifier, so init it here
#endif
    return 0;
}

}
