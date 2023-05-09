#include "mcu.h"
#include "driver.h"
#include "usr_demo.h"

int usr_app_init_ns(void)
{
    int ret = 0;

#if (defined(MCU_SDK_BASIC_DEMO) && (MCU_SDK_BASIC_DEMO == 1))
    extern int usr_demo_start(void);
    usr_demo_start();
#endif

#if (defined(MCU_SDK_TENSAIFLOW_DEMO) && (MCU_SDK_TENSAIFLOW_DEMO == 1))
    extern int tensaiflow_demo_start(void);
    tensaiflow_demo_start();
#endif

#if (defined(MCU_SDK_ACCELEROMETER_DEMO) && (MCU_SDK_ACCELEROMETER_DEMO == 1))
    extern os_TaskHandle g_accelometer_start(void);
    g_accelometer_start();
#endif

#if (defined(MCU_SDK_QSPI_API_DEMO) && (MCU_SDK_QSPI_API_DEMO == 1))
    init_qspi_api_demo_task();
#endif

#if (defined(MCU_SDK_CAMERA_DEMO) && (MCU_SDK_CAMERA_DEMO == 1))
    extern int camera_demo_init(void);
    camera_demo_init();
#endif

#if (defined(MCU_SDK_SFS_DEMO) && (MCU_SDK_SFS_DEMO == 1))
    extern int sfs_demo_init(void);
    sfs_demo_init();
#endif

#if (defined(MCU_SDK_PM_DEMO) && (MCU_SDK_PM_DEMO == 1))
    extern int pm_demo_init(void);
    pm_demo_init();
#endif

#if (defined(MCU_SDK_BLUETOOTH_DEMO) && (MCU_SDK_BLUETOOTH_DEMO == 1))
    extern int bt_demo_init(void);
    bt_demo_init();
#endif

#if (defined(MCU_SDK_ADC_DEMO) && (MCU_SDK_ADC_DEMO == 1))
    extern int adc_demo_init(void);
    adc_demo_init();
#endif

#if (defined(MCU_SDK_STANDALONE) && (MCU_SDK_STANDALONE == 1))
    extern int normal_main();
    normal_main();
#endif

#if (defined(MCU_SDK_FIRMWARE) && (MCU_SDK_FIRMWARE == 1))
    extern int normal_main();
    normal_main();
#endif
    return ret;
}
