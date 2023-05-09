#ifndef _USR_DEMO_H_
#define _USR_DEMO_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef CONFIG_SDK_MCU

#include "userconf.h"

#ifndef CONFIG_USER_KCONFIG_ACTIVE
    //------------------------------- USER CONFIGS ------------------------------------------
    #define MCU_SDK_BASIC_DEMO                  0   //Basic demos like UART, MADC, GPIO, Delay, FDMA

    //------------------------------ OPTIONAL DEMOS -----------------------------------------
    #ifdef CONFIG_MCU_G_ACCELOMETER
        #define MCU_SDK_ACCELEROMETER_DEMO      1   //G_accelerometer demo, ask support team for more detail
    #else
        #define MCU_SDK_ACCELEROMETER_DEMO      0
    #endif

    #define MCU_SDK_QSPI_API_DEMO               0   //QSPI API demo, ask support team for more detail

    #ifdef CONFIG_MCU_DRV_CAMERA
        #define MCU_SDK_CAMERA_DEMO             0   //CAMERA demo
    #endif

    #ifdef CONFIG_MCU_DRV_ADC
        #define MCU_SDK_ADC_DEMO                0   //ADC demo
    #endif

    #define MCU_SDK_SFS_DEMO                    0

    #define MCU_SDK_PM_DEMO                     0

    #define MCU_SDK_BLUETOOTH_DEMO              0

    #define MCU_SDK_TENSAIFLOW_DEMO             0

    #define MCU_SDK_KPD_DEMO                    0

    #define MCU_SDK_KMR_DEMO                    0

#else //USER_KCONFIG_ACTIVE
    //------------------------------ Setup From Kconfig -----------------------------------------
    #define MCU_SDK_BASIC_DEMO              CONFIG_SDK_BASIC_DEMO
    #define MCU_SDK_CAMERA_DEMO             CONFIG_SDK_CAMERA_DEMO
	#define MCU_SDK_ADC_DEMO                CONFIG_MCU_SDK_ADC_DEMO
    #define MCU_SDK_SFS_DEMO                CONFIG_MCU_SDK_SFS_DEMO
    #define MCU_SDK_PM_DEMO                 CONFIG_MCU_SDK_PM_DEMO
    #define MCU_SDK_BLUETOOTH_DEMO          CONFIG_MCU_SDK_BLUETOOTH_DEMO
    #define MCU_SDK_TENSAIFLOW_DEMO         CONFIG_MCU_SDK_TENSAIFLOW_DEMO
    #define MCU_SDK_KPD_DEMO                CONFIG_MCU_SDK_KPD_DEMO
    #define MCU_SDK_KMR_DEMO                CONFIG_MCU_SDK_KMR_DEMO
    #define MCU_SDK_CAMERA_ENABLE           CONFIG_MCU_SDK_CAMERA_ENABLE
    #define MCU_SDK_PRINT_IMAGE_ENABLE      CONFIG_MCU_SDK_PRINT_IMAGE_ENABLE
    #define MCU_SDK_FIRMWARE                CONFIG_FIRMWARE
#endif


//------------------------------ CONFIG CHECKS ------------------------------------------

#if (defined(MCU_SDK_QSPI_API_DEMO) && (MCU_SDK_QSPI_API_DEMO == 1))
    #ifndef CONFIG_MCU_QSPI_API_DEMO
        #error QSPI-API demo is not enabled in the config
    #endif
#endif
//#if (defined(MCU_SDK_ACCELEROMETER_DEMO) && (MCU_SDK_ACCELEROMETER_DEMO == 1))
//    #ifndef  CONFIG_MCU_G_ACCELOMETER
//        #error G-ACCELOMETER demo is not enabled in the config
//    #endif
//#endif


#endif //CONFIG_SDK_MCU

#ifdef __cplusplus
}
#endif

#endif // _MCU_INIT_NS_H_
