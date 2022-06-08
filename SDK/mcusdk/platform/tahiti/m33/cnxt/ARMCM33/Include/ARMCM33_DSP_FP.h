/**************************************************************************//**
 * @file     ARMCM33_DSP_FP.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ARMCM33 Device Series (configured for ARMCM33 with FPU, with DSP extension)
 * @version  V5.00
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ARMCM33_DSP_FP_H
#define ARMCM33_DSP_FP_H

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* --------------------  ARMCM33 Processor Exceptions Numbers  -------------------- */
  NonMaskableInt_IRQn           = -14,      /*  2 Non Maskable Interrupt */
  HardFault_IRQn                = -13,      /*  3 HardFault Interrupt */
  MemoryManagement_IRQn         = -12,      /*  4 Memory Management Interrupt */
  BusFault_IRQn                 = -11,      /*  5 Bus Fault Interrupt */
  UsageFault_IRQn               = -10,      /*  6 Usage Fault Interrupt */
  SecureFault_IRQn              =  -9,      /*  7 Secure Fault Interrupt */
  SVCall_IRQn                   =  -5,      /* 11 SV Call Interrupt */
  DebugMonitor_IRQn             =  -4,      /* 12 Debug Monitor Interrupt */
  PendSV_IRQn                   =  -2,      /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,      /* 15 System Tick Interrupt */

/* --------------------  ARMCM33 Specific Interrupt Numbers  ---------------------- */
  nvic_ADMA_CC_ACCVIO_IRQn                 =  0,
  nvic_TIMERS_ACCVIO_IRQn                  =  1,
  nvic_IPC_M3_ACCVIO_IRQn                  =  2,
  nvic_IPC_M33_ACCVIO_IRQn                 =  3,
  nvic_IPC_A53_0_ACCVIO_IRQn               =  4,
  nvic_IPC_A53_1_ACCVIO_IRQn               =  5,
  nvic_IPC_HIFI_ACCVIO_IRQn                =  6,
  nvic_SCM_M33_ACCVIO_IRQn                 =  7,
  nvic_SCM_A53_ACCVIO_IRQn                 =  8,
  nvic_AOCLK_M33_ACCVIO_IRQn               =  9,
  nvic_AOCLK_A53_ACCVIO_IRQn               =  10,
  nvic_PWR1CLK_M33_ACCVIO_IRQn             =  11,
  nvic_PWR1CLK_A53_ACCVIO_IRQn             =  12,
  nvic_AORST_M33_ACCVIO_IRQn               =  13,
  nvic_AORST_A53_ACCVIO_IRQn               =  14,
  nvic_PWR1RST_M33_ACCVIO_IRQn             =  15,
  nvic_PWR1RST_A53_ACCVIO_IRQn             =  16,
  nvic_PWRMGT_M33_ACCVIO_IRQn              =  17,
  nvic_PWRMGT_A53_ACCVIO_IRQn              =  18,
  nvic_SLOWAPB_ACCVIO_IRQn                 =  19,
  nvic_FASTAPB_ACCVIO_IRQn                 =  20,
  nvic_AXIAPB_ACCVIO_IRQn                  =  21,
  nvic_AHBSLV_ACCVIO_IRQn                  =  22,
  nvic_IDAU_RG0_ACCVIO_IRQn                =  23,
  nvic_IDAU_RG1_ACCVIO_IRQn                =  24,
  nvic_IDAU_RG2_ACCVIO_IRQn                =  25,
  nvic_IDAU_RG3_ACCVIO_IRQn                =  26,
  nvic_IDAU_RG4_ACCVIO_IRQn                =  27,
  nvic_IDAU_RG5_ACCVIO_IRQn                =  28,
  nvic_IDAU_RG6_ACCVIO_IRQn                =  29,
  nvic_IDAU_RG7_ACCVIO_IRQn                =  30,
  nvic_IDAU_RG8_ACCVIO_IRQn                =  31,
  nvic_IDAU_RG9_ACCVIO_IRQn                =  32,
  nvic_IDAU_RG10_ACCVIO_IRQn               =  33,
  nvic_IDAU_RG11_ACCVIO_IRQn               =  34,
  nvic_IDAU_RG12_ACCVIO_IRQn               =  35,
  nvic_IDAU_RG13_ACCVIO_IRQn               =  36,
  nvic_IDAU_RG14_ACCVIO_IRQn               =  37,
  nvic_IDAU_RG15_ACCVIO_IRQn               =  38,
  nvic_IDAU_RG16_ACCVIO_IRQn               =  39,
  nvic_IDAU_RG17_ACCVIO_IRQn               =  40,
  nvic_IDAU_RG18_ACCVIO_IRQn               =  41,
  nvic_IDAU_RG19_ACCVIO_IRQn               =  42,
  nvic_IDAU_RG20_ACCVIO_IRQn               =  43,
  nvic_IDAU_RG21_ACCVIO_IRQn               =  44,
  nvic_IDAU_RG22_ACCVIO_IRQn               =  45,
  nvic_IDAU_RG23_ACCVIO_IRQn               =  46,
  nvic_IDAU_RG24_ACCVIO_IRQn               =  47,
  nvic_IDAU_RG25_ACCVIO_IRQn               =  48,
  nvic_IDAU_RG26_ACCVIO_IRQn               =  49,
  nvic_IDAU_RG27_ACCVIO_IRQn               =  50,
  nvic_IDAU_RG28_ACCVIO_IRQn               =  51,
  nvic_IDAU_RG29_ACCVIO_IRQn               =  52,
  nvic_IDAU_RG30_ACCVIO_IRQn               =  53,
  nvic_IDAU_RG31_ACCVIO_IRQn               =  54,
  nvic_M33_FPU_IRQn                        =  55,
  nvic_M33_ICACHE_IRQn                     =  56,
  nvic_CC_AHB_RX_1_0_IRQn                  =  57,
  nvic_CC_AHB_RX_1_1_IRQn                  =  58,
  nvic_CC_AHB_RX_1_2_IRQn                  =  59,
  nvic_CC_AHB_TX_1_0_IRQn                  =  60,
  nvic_CC_AHB_TX_1_1_IRQn                  =  61,
  nvic_CC_AHB_TX_1_2_IRQn                  =  62,
  nvic_CC_AHB_RX_2_0_IRQn                  =  63,
  nvic_CC_AHB_RX_2_1_IRQn                  =  64,
  nvic_CC_AHB_RX_2_2_IRQn                  =  65,
  nvic_CC_AHB_RX_2_3_IRQn                  =  66,
  nvic_CC_AHB_RX_2_4_IRQn                  =  67,
  nvic_CC_AHB_RX_2_5_IRQn                  =  68,
  nvic_CC_AHB_RX_2_6_IRQn                  =  69,
  nvic_CC_AHB_RX_2_7_IRQn                  =  70,
  nvic_CC_AHB_TX_2_0_IRQn                  =  71,
  nvic_CC_AHB_TX_2_1_IRQn                  =  72,
  nvic_CC_AHB_TX_2_2_IRQn                  =  73,
  nvic_CC_AHB_TX_2_3_IRQn                  =  74,
  nvic_CC_AHB_TX_2_4_IRQn                  =  75,
  nvic_CC_AHB_TX_2_5_IRQn                  =  76,
  nvic_CC_AHB_TX_2_6_IRQn                  =  77,
  nvic_CC_AHB_TX_2_7_IRQn                  =  78,
  nvic_CC_SAIF_RX_1_0_IRQn                 =  79,
  nvic_CC_SAIF_RX_1_1_IRQn                 =  80,
  nvic_CC_SAIF_RX_1_2_IRQn                 =  81,
  nvic_CC_SAIF_RX_1_3_IRQn                 =  82,
  nvic_CC_SAIF_TX_1_0_IRQn                 =  83,
  nvic_CC_SAIF_TX_1_1_IRQn                 =  84,
  nvic_CC_SAIF_TX_1_2_IRQn                 =  85,
  nvic_CC_SAIF_TX_1_3_IRQn                 =  86,
  nvic_GPIO_00_IRQn                        =  87,
  nvic_GPIO_01_IRQn                        =  88,
  nvic_GPIO_02_IRQn                        =  89,
  nvic_GPIO_03_IRQn                        =  90,
  nvic_GPIO_04_IRQn                        =  91,
  nvic_GPIO_05_IRQn                        =  92,
  nvic_GPIO_06_IRQn                        =  93,
  nvic_GPIO_07_IRQn                        =  94,
  nvic_GPIO_08_IRQn                        =  95,
  nvic_GPIO_09_IRQn                        =  96,
  nvic_GPIO_10_IRQn                        =  97,
  nvic_GPIO_11_IRQn                        =  98,
  nvic_GPIO_12_IRQn                        =  99,
  nvic_GPIO_13_IRQn                        =  100,
  nvic_GPIO_14_IRQn                        =  101,
  nvic_GPIO_15_IRQn                        =  102,
  nvic_GPIO_16_IRQn                        =  103,
  nvic_GPIO_17_IRQn                        =  104,
  nvic_GPIO_18_IRQn                        =  105,
  nvic_GPIO_19_IRQn                        =  106,
  nvic_GPIO_20_IRQn                        =  107,
  nvic_GPIO_21_IRQn                        =  108,
  nvic_GPIO_22_IRQn                        =  109,
  nvic_GPIO_23_IRQn                        =  110,
  nvic_GPIO_24_IRQn                        =  111,
  nvic_GPIO_25_IRQn                        =  112,
  nvic_GPIO_26_IRQn                        =  113,
  nvic_GPIO_27_IRQn                        =  114,
  nvic_GPIO_28_IRQn                        =  115,
  nvic_GPIO_29_IRQn                        =  116,
  nvic_GPIO_30_IRQn                        =  117,
  nvic_GPIO_31_IRQn                        =  118,
  nvic_GPIO_32_IRQn                        =  119,
  nvic_GPIO_33_IRQn                        =  120,
  nvic_GPIO_34_IRQn                        =  121,
  nvic_GPIO_35_IRQn                        =  122,
  nvic_GPIO_36_IRQn                        =  123,
  nvic_GPIO_37_IRQn                        =  124,
  nvic_GPIO_38_IRQn                        =  125,
  nvic_GPIO_39_IRQn                        =  126,
  nvic_GPIO_40_IRQn                        =  127,
  nvic_GPIO_41_IRQn                        =  128,
  nvic_GPIO_42_IRQn                        =  129,
  nvic_GPIO_43_IRQn                        =  130,
  nvic_GPIO_44_IRQn                        =  131,
  nvic_GPIO_45_IRQn                        =  132,
  nvic_GPIO_46_IRQn                        =  133,
  nvic_GPIO_47_IRQn                        =  134,
  nvic_GPIO_48_IRQn                        =  135,
  nvic_GPIO_49_IRQn                        =  136,
  nvic_GPIO_50_IRQn                        =  137,
  nvic_GPIO_51_IRQn                        =  138,
  nvic_GPIO_52_IRQn                        =  139,
  nvic_GPIO_53_IRQn                        =  140,
  nvic_GPIO_54_IRQn                        =  141,
  nvic_GPIO_55_IRQn                        =  142,
  nvic_GPIO_56_IRQn                        =  143,
  nvic_GPIO_57_IRQn                        =  144,
  nvic_GPIO_58_IRQn                        =  145,
  nvic_GPIO_59_IRQn                        =  146,
  nvic_GPIO_60_IRQn                        =  147,
  nvic_GPIO_61_IRQn                        =  148,
  nvic_GPIO_62_IRQn                        =  149,
  nvic_GPIO_63_IRQn                        =  150,
  nvic_GPIO_64_IRQn                        =  151,
  nvic_GPIO_65_IRQn                        =  152,
  nvic_GPIO_66_IRQn                        =  153,
  nvic_GPIO_67_IRQn                        =  154,
  nvic_GPIO_68_IRQn                        =  155,
  nvic_GPIO_69_IRQn                        =  156,
  nvic_DRAM_MC_IRQn                        =  157,
  nvic_LED_PWM_IRQn                        =  158,
  nvic_USB_IRQn                            =  159,
  nvic_USB_hirdIntr_IRQn                   =  160,
  nvic_RTC_IRQn                            =  161,
  nvic_PWR_IRQn                            =  162,
  nvic_IPC_M33_IRQn                        =  163,
  nvic_DMIC_WAKE_IRQn                      =  164,
  nvic_GPTC0_IRQn                          =  165,
  nvic_GPTC1_IRQn                          =  166,
  nvic_GPTC2_IRQn                          =  167,
  nvic_GPTC3_IRQn                          =  168,
  nvic_GPTC4_IRQn                          =  169,
  nvic_GPTC5_IRQn                          =  170,
  nvic_GPTC6_IRQn                          =  171,
  nvic_GPTC7_IRQn                          =  172,
  nvic_GPTC8_IRQn                          =  173,
  nvic_GPTC0_WD_IRQn                       =  174,
  nvic_GPTC1_WD_IRQn                       =  175,
  nvic_GPTC2_WD_IRQn                       =  176,
  nvic_GPTC3_WD_IRQn                       =  177,
  nvic_GPTC4_WD_IRQn                       =  178,
  nvic_GPTC5_WD_IRQn                       =  179,
  nvic_GPTC6_WD_IRQn                       =  180,
  nvic_GPTC7_WD_IRQn                       =  181,
  nvic_GPTC8_WD_IRQn                       =  182,
  nvic_SPTC0_IRQn                          =  183,
  nvic_SPTC1_IRQn                          =  184,
  nvic_SPTC2_IRQn                          =  185,
  nvic_SPTC3_IRQn                          =  186,
  nvic_EMMC_IRQn                           =  187,
  nvic_SDIO_IRQn                           =  188,
  nvic_I3C0_IRQn                           =  189,
  nvic_I3C1_IRQn                           =  190,
  nvic_NAND_IRQn                           =  191,
  nvic_OCTAL_SPI_IRQn                      =  192,
  nvic_SPI_IRQn                            =  193,
  nvic_UART0_IRQn                          =  194,
  nvic_UART1_IRQn                          =  195,
  nvic_AHBFAB_TO_INTR_IRQn                 =  196,
  nvic_SISSYS_GIC400_tsSSIntr_IRQn         =  197,
  nvic_SISSYS_GIC400_nocsIntr_IRQn         =  198,
  nvic_SISSYS_GIC400_kilopassIntr_IRQn     =  199,
  nvic_SISSYS_GIC400_bcmIntr_IRQn          =  200,
  nvic_SISSYS_GIC400_wdtInt_IRQn           =  201,
  nvic_HiFi4_DebugMode_IRQn                =  202,
  nvic_HiFi4_DoubleExceptionError_IRQn     =  203,
  nvic_HiFi4_PFatalError_IRQn              =  204,
  nvic_ABBGEN_IRQn                         =  205,
  nvic_MIPISS_INTR0_IRQn                   =  206,
  nvic_MIPISS_INTR1_IRQn                   =  207,
  nvic_MIPISS_INTR2_IRQn                   =  208,
  nvic_SYSAUDPLL_intctl_intr_IRQn          =  209,
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* --------  Configuration of the Cortex-M33 Processor and Core Peripherals  ------ */
#define __CM33_REV                0x0000U   /* Core revision r0p1 */
#define __SAUREGION_PRESENT       0U        /* SAU regions present */
#define __MPU_PRESENT             1U        /* MPU present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          4U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* FPU present */
#define __DSP_PRESENT             1U        /* DSP extension present */

#include "core_cm33.h"                      /* Processor and core peripherals */
#include "system_ARMCM33.h"                 /* System Header */

/* --------  End of section using anonymous unions and disabling warnings  -------- */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

#ifdef __cplusplus
}
#endif

#endif  /* ARMCM33_DSP_FP_H */
