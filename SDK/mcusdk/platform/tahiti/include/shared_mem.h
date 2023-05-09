
#ifndef _SHARED_MEM_H_
#define _SHARED_MEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(CONFIG_CHIP_VERSION_Z1)
#include "memmap_z1.h"
#elif defined(CONFIG_CHIP_VERSION_A0)
#include "memmap_a0.h"
#elif defined(CONFIG_CHIP_VERSION_A1)
#include "memmap_a1.h"
#elif defined(CONFIG_CHIP_VERSION_B0)
#include "memmap_a0.h"
#else
#error "CHIP VERSION ERROR"
#endif

//---------------------------------- Macros -----------------------------------------------------
#define AHB_SEC_CONV(x)       ((AHB_SHARED_MEM_BANK0_BASE     + x))
#define AHB_CONV(x)           ((AHB_SHARED_MEM_BANK0_BASE_NS  + x))
#define ASIP_CONV(x)          ((ASIP_SHARED_MEM_BANK0_BASE    + x))
#define ASIP_BACKDOOR_CONV(x) ((ASIP_SHARED_MEM_BACKDOOR_BANK0_BASE + x))

#define IS_HIFI_LOC_MEM(x)    ((uint32_t)(x)&0x02000000)
#define HIFI_LOC_2_AHB(x)     ((uint32_t)x)
#define HIFI_SM_2_AHB(x)      (((uint32_t)(x)&0x001FFFFF)+ 0x41500000)
#define HIFI_MEM_2_AHB(x)     (void*)(IS_HIFI_LOC_MEM(x)? HIFI_LOC_2_AHB(x) : HIFI_SM_2_AHB(x))

//---------------------------------- Defines Start -----------------------------------------------
#define M33_POWER_STATUS_OFFSET             CONFIG_M33_POWER_STATE_SHARED_MEM_OFFSET        //0
#define CAPEA_POWER_STATUS_OFFSET           CONFIG_CAPEA_POWER_STATE_SHARED_MEM_OFFSET      //4
#define CAPEB_POWER_STATUS_OFFSET           CONFIG_CAPEB_POWER_STATE_SHARED_MEM_OFFSET      //8
                                                                                            //C     - Not used
                                                                                            //10    - Not used
#define HIFI3_POWER_STATUS_OFFSET           CONFIG_HIFI3_POWER_STATE_SHARED_MEM_OFFSET      //14
#define VID_PID_ADDR_OFFSET                 CONFIG_VID_PID_SHARED_MEM_OFFSET                //18
#define BL_VERSION_OFFSET                   CONFIG_BL_VERSION_SHARED_MEM_OFFSET             //1C-2C - BL Version
#define M33_BOOT_STATUS                     0x30                                            //30
#define CAPE_A_BOOT_STATUS                  0x34
#define CAPE_B_BOOT_STATUS                  0x38
#define LLE_A_BOOT_STATUS                   0x3C
#define GLOBAL_PRINT_SILENCE                0x40
#define CAPE_A_PERSISTENT_X_DATA            0x44
#define CAPE_A_PERSISTENT_Y_DATA            0x48
#define CAPE_B_PERSISTENT_X_DATA            0x4C
#define CAPE_B_PERSISTENT_Y_DATA            0x50
#define DSP_HIFI3_PERSISTENT_SEG0_DATA      0x54
#define DSP_HIFI3_PERSISTENT_SEG1_DATA      0x58
#define SHMEM_CTRL_STRUCT_PTR               0x5C

//---------------------------------- AHB View -----------------------------------------------------
#define MCU_VIEW_M33_POWER_STATE_ADDR                       AHB_CONV(M33_POWER_STATUS_OFFSET)  
#define MCU_VIEW_CAPEA_POWER_STATE_ADDR                     AHB_CONV(CAPEA_POWER_STATUS_OFFSET)
#define MCU_VIEW_CAPEB_POWER_STATE_ADDR                     AHB_CONV(CAPEB_POWER_STATUS_OFFSET)
#define MCU_VIEW_HIFI3_POWER_STATE_ADDR                     AHB_CONV(HIFI3_POWER_STATUS_OFFSET)
#define MCU_NS_VIEW_VID_PID_ADDR                            AHB_CONV(VID_PID_ADDR_OFFSET)
#define MCU_NS_VIEW_BL_VERSION_ADDR                         AHB_CONV(BL_VERSION_OFFSET)
#define MCU_S_VIEW_VID_PID_ADDR                             AHB_CONV(VID_PID_ADDR_OFFSET)
#define MCU_S_VIEW_BL_VERSION_ADDR                          AHB_CONV(BL_VERSION_OFFSET)
#define MCU_VIEW_GLOBAL_PRINT_SILENCE                       AHB_CONV(GLOBAL_PRINT_SILENCE)
#define MCU_VIEW_CAPE_A_PERSISTENT_X_DATA_ADDR              AHB_CONV(CAPE_A_PERSISTENT_X_DATA)
#define MCU_VIEW_CAPE_A_PERSISTENT_Y_DATA_ADDR              AHB_CONV(CAPE_A_PERSISTENT_Y_DATA)
#define MCU_VIEW_CAPE_B_PERSISTENT_X_DATA_ADDR              AHB_CONV(CAPE_B_PERSISTENT_X_DATA)
#define MCU_VIEW_CAPE_B_PERSISTENT_Y_DATA_ADDR              AHB_CONV(CAPE_B_PERSISTENT_Y_DATA)
#define MCU_VIEW_DSP_HIFI3_PERSISTENT_SEG0_DATA_ADDR        AHB_CONV(DSP_HIFI3_PERSISTENT_SEG0_DATA)
#define MCU_VIEW_DSP_HIFI3_PERSISTENT_SEG1_DATA_ADDR        AHB_CONV(DSP_HIFI3_PERSISTENT_SEG1_DATA)
#define MCU_VIEW_SHMEM_CTRL_ADDR_PTR                        AHB_CONV(SHMEM_CTRL_STRUCT_PTR)

//---------------------------------- ASIP View ----------------------------------------------------
#define CAPEA_POWER_STATE_ADDR                              ASIP_CONV(CAPEA_POWER_STATUS_OFFSET)  
#define CAPEB_POWER_STATE_ADDR                              ASIP_CONV(CAPEB_POWER_STATUS_OFFSET)   
#define CAPE_VIEW_GLOBAL_PRINT_SILENCE                      ASIP_CONV(GLOBAL_PRINT_SILENCE)
#define CAPE_VIEW_CAPE_A_PERSISTENT_X_DATA_ADDR             ASIP_CONV(CAPE_A_PERSISTENT_X_DATA)
#define CAPE_VIEW_CAPE_A_PERSISTENT_Y_DATA_ADDR             ASIP_CONV(CAPE_A_PERSISTENT_Y_DATA)
#define CAPE_VIEW_CAPE_B_PERSISTENT_X_DATA_ADDR             ASIP_CONV(CAPE_B_PERSISTENT_X_DATA)
#define CAPE_VIEW_CAPE_B_PERSISTENT_Y_DATA_ADDR             ASIP_CONV(CAPE_B_PERSISTENT_Y_DATA)
#define CAPE_VIEW_SHMEM_CTRL_ADDR_PTR                       ASIP_CONV(SHMEM_CTRL_STRUCT_PTR)

//---------------------------------- LLE View ----------------------------------------------------
#define LLE_VIEW_GLOBAL_PRINT_SILENCE                       ASIP_BACKDOOR_CONV(GLOBAL_PRINT_SILENCE)
#define LLE_VIEW_SHMEM_CTRL_ADDR_PTR                        ASIP_BACKDOOR_CONV(SHMEM_CTRL_STRUCT_PTR)

//---------------------------------- HIFI View ----------------------------------------------------
#define HIFI_VIEW_DSP_HIFI3_PERSISTENT_SEG0_DATA_ADDR       AHB_CONV(DSP_HIFI3_PERSISTENT_SEG0_DATA)
#define HIFI_VIEW_DSP_HIFI3_PERSISTENT_SEG1_DATA_ADDR       AHB_CONV(DSP_HIFI3_PERSISTENT_SEG1_DATA)
#define HIFI_VIEW_SHMEM_CTRL_ADDR_PTR                       AHB_CONV(SHMEM_CTRL_STRUCT_PTR)





//---------------------------------- ???????? ----------------------------------------------------
#define STORE_TIMESTAMP(x, t)                                                               \
        extern volatile uint32_t SystemTickCnt;                                             \
        t[x] = SystemTickCnt;//t[x] = DWT->CYCCNT;//
#define CALC_TIMEDURATION(x,y,t)                                                             \
        ((t[x] >= t[y]) ? (t[x] - t[y]) : (0xFFFFFFFF - t[y] + t[x]))

#ifdef __cplusplus
}
#endif

#endif // _SHARED_MEM_H_
