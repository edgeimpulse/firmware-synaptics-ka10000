//-----------------------------------------------------------------------------
// The confidential and proprietary information contained in this file may
// only be used by a person authorised under and to the extent permitted
// by a subsisting licensing agreement from ARM Limited or its affiliates.
//
//        (C) COPYRIGHT 2016, 2017 ARM Limited or its affiliates.
//            ALL RIGHTS RESERVED
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from ARM Limited or its affiliates.
//
//      SVN Information
//
//      Checked In          : 2017-07-18 15:23:02 +0100 (Tue, 18 Jul 2017) 
//
//      Revision            : 12261 
//
//      Release Information : r1p0-00eac0
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Abstract : CMSIS SYSTEM Memory Access Layer Header File for
//            ARMv8MBL Device Series
//-----------------------------------------------------------------------------
/**************************************************************************//**
 * @file     sysmem_test.h
 * @brief    System Memory Layer Header File for
 *           Tahiti Device Series
 * @version  V0.01
 * @date     01. March 2019
 ******************************************************************************/

#ifndef SYS_MEM_H
#define SYS_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_CHIP_VERSION_Z1
#include "../../include/memmap_z1.h"
#endif

#ifdef CONFIG_CHIP_VERSION_A1
#include "../../include/memmap_a1.h"
#endif

/* DEFINE MEM TEST REACH */
// #define FULL_MEM_TEST
// #define BIG_FPGA
#define SMALL_CAPE_FPGA
// #define SMALL_HIFI_FPGA

/* FULL_MEMORY TEST */
#ifdef FULL_MEM_TEST
#define CAPEA_TEST
#define VECTOR_TEST
#define SHARED_MEM_TEST
#endif

/* BIG_FPGA TEST */
#ifdef BIG_FPGA
#define LLEA_TEST
#define LLEB_TEST
#define CAPEA_TEST
#define CAPEB_TEST
#define ROM_TEST
#define VECTOR_TEST
#define SHARED_MEM_TEST
#endif

/* SMALL_CAPE_FPGA TEST */
#ifdef SMALL_CAPE_FPGA
#define CAPEA_TEST /*requires the CAPEA to be clock gated in CR_CLK_DSP_0*/
#define VECTOR_TEST
#define SHARED_MEM_TEST
#endif

/* SMALL_HIFI_FPGA TEST */
#ifdef SMALL_HIFI_FPGA
#define CAPEA_TEST /*requires the CAPEA to be clock gated in CR_CLK_DSP_0*/
#define VECTOR_TEST
#define SHARED_MEM_TEST
#define HIFI3_TEST
#else

/* Standard SHARED and VECTOR ONLY */
#define VECTOR_TEST
#define SHARED_MEM_TEST 
#endif

/*Check 8 and 16 width*/
//#define CHECK_ALL_WIDTHS
#define W8  1
#define W16 2
#define W24 4
#define W32 8

#define MEM_INFO
#ifdef	MEM_INFO
#define MEM_PRINT   Print_DBG
#else
#define MEM_PRINT
#endif

#define MEM_ERR
#ifdef	MEM_ERR
#define MEM_PRINT_ERR   Print_DBG
#else
#define MEM_PRINT_ERR
#endif

//#define MEM_DBG
#ifdef	MEM_DBG
#define MEM_PRINT_DBG   Print_DBG
#else
#define MEM_PRINT_DBG
#endif



/*DSP Memory*/

#define LLEA_LOC_0_LOC_1_START      AHB_LLEA_LOCAL_0_BASE_S
#define LLEA_LOC_0_LOC_1_SIZE       (0x00008000UL)
#define LLEA_LOC_0_LOC_1_END        REGION_LIMIT(LLEA_LOC_0_LOC_1_START, LLEA_LOC_0_LOC_1_SIZE)

#define LLEB_LOC_0_LOC_3_START      AHB_LLEB_LOCAL_0_BASE_S
#define LLEB_LOC_0_LOC_3_SIZE       (0x00010000UL)
#define LLEB_LOC_0_LOC_3_END        REGION_LIMIT(LLEB_LOC_0_LOC_3_START, LLEB_LOC_0_LOC_3_SIZE)

#define LLEB_ROM_START              AHB_LLEB_ROM_BASE_S
#define LLEB_ROM_SIZE               (0x00002000UL)
#define LLEB_ROM_END                REGION_LIMIT(LLEB_ROM_START, LLEB_ROM_SIZE)

#define CAPEA_LOC_0_LOC_5_START     AHB_CAPEA_LOCAL_0_BASE_S
#define CAPEA_LOC_0_LOC_5_SIZE      (0x00030000UL)
#define CAPEA_LOC_0_LOC_5_END       REGION_LIMIT(CAPEA_LOC_0_LOC_5_START, CAPEA_LOC_0_LOC_5_SIZE)

#define CAPEA_ROM_START             AHB_CAPEA_ROM_BASE_S
#define CAPEA_ROM_SIZE              (0x00004000UL)
#define CAPEA_ROM_END               REGION_LIMIT(CAPEA_ROM_START, CAPEA_ROM_SIZE)

#define CAPEB_LOC_0_LOC_3_START     AHB_CAPEB_LOCAL_0_BASE_S
#define CAPEB_LOC_0_LOC_3_SIZE      (0x00020000UL + AHB_CAPEB_LOCAL_4_SIZE_S + AHB_CAPEB_LOCAL_5_SIZE_S)
#define CAPEB_LOC_0_LOC_3_END       REGION_LIMIT(CAPEB_LOC_0_LOC_3_START, CAPEB_LOC_0_LOC_3_SIZE)

#define CAPEB_ROM_START             AHB_CAPEB_ROM_BASE_S
#define CAPEB_ROM_SIZE              (0x00008000UL)
#define CAPEB_ROM_END               REGION_LIMIT(CAPEB_ROM_START, CAPEB_ROM_SIZE)

#define VECT_0_TO_4_START           AHB_VECTOR_0_TO_4_BASE_S
#define VECT_0_TO_4_SIZE            (0x0003C000UL - AHB_VECTOR_0_TO_4_HALF_SIZE_FOR_Z1)
#define VECT_0_TO_4_END             REGION_LIMIT(VECT_0_TO_4_START, VECT_0_TO_4_SIZE)

/* Shared Memory*/
#define	MB_SHARE_NS_BASE_ADDR      AHB_SHARED_MEMORY_BANK_BASE_NS
#define	MB_SHARE_S_BASE_ADDR       AHB_SHARED_MEMORY_BANK_BASE_S
#define	BB_SHARE_BASE_OFST         AHB_SHARED_MEM_BANK_BACKDOOR_OFFSET

/* FDMA testing requires NS memory*/
#ifdef CONFIG_MCU_FDMA_TESTS
#define	MB_SHARE_BASE              (MB_SHARE_NS_BASE_ADDR)
#else
#define	MB_SHARE_BASE              (MB_SHARE_S_BASE_ADDR)
#endif /*CONFIG_MCU_FDMA_TESTS*/
#define	BB_SHARE_BASE              (MB_SHARE_BASE+BB_SHARE_BASE_OFST)

#define	MB_SHARE_00_START        (MB_SHARE_BASE)
#define MB_SHARE_00_SIZE         (0x00010000UL)
#define MB_SHARE_00_END          REGION_LIMIT(MB_SHARE_00_START, MB_SHARE_00_SIZE)

#define	MB_SHARE_01_START        (MB_SHARE_BASE+0x10000)
#define MB_SHARE_01_SIZE         (0x00010000UL)
#define MB_SHARE_01_END          REGION_LIMIT(MB_SHARE_01_START, MB_SHARE_01_SIZE)

#define	MB_SHARE_02_START        (MB_SHARE_BASE+0x20000)
#define MB_SHARE_02_SIZE         (0x00010000UL)
#define MB_SHARE_02_END          REGION_LIMIT(MB_SHARE_02_START, MB_SHARE_02_SIZE)

#define	MB_SHARE_03_START        (MB_SHARE_BASE+0x30000)
#define MB_SHARE_03_SIZE         (0x00010000UL)
#define MB_SHARE_03_END          REGION_LIMIT(MB_SHARE_03_START, MB_SHARE_03_SIZE)

#define	MB_SHARE_04_START        (MB_SHARE_BASE+0x40000)
#define MB_SHARE_04_SIZE         (0x00010000UL)
#define MB_SHARE_04_END          REGION_LIMIT(MB_SHARE_04_START, MB_SHARE_04_SIZE)

#define	MB_SHARE_05_START        (MB_SHARE_BASE+0x50000)
#define MB_SHARE_05_SIZE         (0x00010000UL)
#define MB_SHARE_05_END          REGION_LIMIT(MB_SHARE_05_START, MB_SHARE_05_SIZE)

#define	MB_SHARE_06_START        (MB_SHARE_BASE+0x60000)
#define MB_SHARE_06_SIZE         (0x00010000UL)
#define MB_SHARE_06_END          REGION_LIMIT(MB_SHARE_06_START, MB_SHARE_06_SIZE)

#define	MB_SHARE_07_START        (MB_SHARE_BASE+0x70000)
#define MB_SHARE_07_SIZE         (0x00010000UL)
#define MB_SHARE_07_END          REGION_LIMIT(MB_SHARE_07_START, MB_SHARE_07_SIZE)

#define	MB_SHARE_08_START        (MB_SHARE_BASE+0x80000)
#define MB_SHARE_08_SIZE         (0x00010000UL)
#define MB_SHARE_08_END          REGION_LIMIT(MB_SHARE_08_START, MB_SHARE_08_SIZE)

#define	MB_SHARE_09_START        (MB_SHARE_BASE+0x90000)
#define MB_SHARE_09_SIZE         (0x00010000UL)
#define MB_SHARE_09_END          REGION_LIMIT(MB_SHARE_09_START, MB_SHARE_09_SIZE)

#define	MB_SHARE_10_START        (MB_SHARE_BASE+0xA0000)
#define MB_SHARE_10_SIZE         (0x00010000UL)
#define MB_SHARE_10_END          REGION_LIMIT(MB_SHARE_10_START, MB_SHARE_10_SIZE)

#ifdef CONFIG_CHIP_VERSION_Z1 			// Z1 has 13 64kB banks
#define	MB_SHARE_11_START        (MB_SHARE_BASE+0xB0000)
#define MB_SHARE_11_SIZE         (0x00010000UL)
#define MB_SHARE_11_END          REGION_LIMIT(MB_SHARE_11_START, MB_SHARE_11_SIZE)

#define	MB_SHARE_12_START        (MB_SHARE_BASE+0xC0000)
#define MB_SHARE_12_SIZE         (0x00010000UL)
#define MB_SHARE_12_END          REGION_LIMIT(MB_SHARE_12_START, MB_SHARE_12_SIZE)

#define	MB_SHARE_13_START        (MB_SHARE_BASE+0xD0000)
#define MB_SHARE_13_SIZE         (0x00010000UL)
#define MB_SHARE_13_END          REGION_LIMIT(MB_SHARE_13_START, MB_SHARE_13_SIZE)

#define	MB_SHARE_00_13_START     (MB_SHARE_BASE)
#define MB_SHARE_00_13_SIZE      (0x000E0000UL)
#define MB_SHARE_00_13_END       REGION_LIMIT(MB_SHARE_00_13_START, MB_SHARE_00_13_SIZE)

#elif defined CONFIG_CHIP_VERSION_A1	// A1 has 11 64kB banks
#define	MB_SHARE_00_10_START     (MB_SHARE_BASE)
#define MB_SHARE_00_10_SIZE      (0x000B0000UL)
#define MB_SHARE_00_10_END       REGION_LIMIT(MB_SHARE_00_10_START, MB_SHARE_00_10_SIZE)
#endif

#define	BB_SHARE_00_START        (BB_SHARE_BASE)
#define BB_SHARE_00_SIZE         (0x00010000UL)
#define BB_SHARE_00_END          REGION_LIMIT(BB_SHARE_00_START, BB_SHARE_00_SIZE)

#define	BB_SHARE_01_START        (BB_SHARE_BASE+0x10000)
#define BB_SHARE_01_SIZE         (0x00010000UL)
#define BB_SHARE_01_END          REGION_LIMIT(BB_SHARE_01_START, BB_SHARE_01_SIZE)

#define	BB_SHARE_02_START        (BB_SHARE_BASE+0x20000)
#define BB_SHARE_02_SIZE         (0x00010000UL)
#define BB_SHARE_02_END          REGION_LIMIT(BB_SHARE_02_START, BB_SHARE_02_SIZE)

#define	BB_SHARE_03_START        (BB_SHARE_BASE+0x30000)
#define BB_SHARE_03_SIZE         (0x00010000UL)
#define BB_SHARE_03_END          REGION_LIMIT(BB_SHARE_03_START, BB_SHARE_03_SIZE)

#define	BB_SHARE_04_START        (BB_SHARE_BASE+0x40000)
#define BB_SHARE_04_SIZE         (0x00010000UL)
#define BB_SHARE_04_END          REGION_LIMIT(BB_SHARE_04_START, BB_SHARE_04_SIZE)

#define	BB_SHARE_05_START        (BB_SHARE_BASE+0x50000)
#define BB_SHARE_05_SIZE         (0x00010000UL)
#define BB_SHARE_05_END          REGION_LIMIT(BB_SHARE_05_START, BB_SHARE_05_SIZE)

#define	BB_SHARE_06_START        (BB_SHARE_BASE+0x60000)
#define BB_SHARE_06_SIZE         (0x00010000UL)
#define BB_SHARE_06_END          REGION_LIMIT(BB_SHARE_06_START, BB_SHARE_06_SIZE)

#define	BB_SHARE_07_START        (BB_SHARE_BASE+0x70000)
#define BB_SHARE_07_SIZE         (0x00010000UL)
#define BB_SHARE_07_END          REGION_LIMIT(BB_SHARE_07_START, BB_SHARE_07_SIZE)

#define	BB_SHARE_08_START        (BB_SHARE_BASE+0x80000)
#define BB_SHARE_08_SIZE         (0x00010000UL)
#define BB_SHARE_08_END          REGION_LIMIT(BB_SHARE_08_START, BB_SHARE_08_SIZE)

#define	BB_SHARE_09_START        (BB_SHARE_BASE+0x90000)
#define BB_SHARE_09_SIZE         (0x00010000UL)
#define BB_SHARE_09_END          REGION_LIMIT(BB_SHARE_09_START, BB_SHARE_09_SIZE)

#define	BB_SHARE_10_START        (BB_SHARE_BASE+0xA0000)
#define BB_SHARE_10_SIZE         (0x00010000UL)
#define BB_SHARE_10_END          REGION_LIMIT(BB_SHARE_10_START, BB_SHARE_10_SIZE)

#ifdef CONFIG_CHIP_VERSION_Z1 			// Z1 has 13 64kB banks
#define	BB_SHARE_11_START        (BB_SHARE_BASE+0xB0000)
#define BB_SHARE_11_SIZE         (0x00010000UL)
#define BB_SHARE_11_END          REGION_LIMIT(BB_SHARE_11_START, BB_SHARE_11_SIZE)

#define	BB_SHARE_12_START        (BB_SHARE_BASE+0xC0000)
#define BB_SHARE_12_SIZE         (0x00010000UL)
#define BB_SHARE_12_END          REGION_LIMIT(BB_SHARE_12_START, BB_SHARE_12_SIZE)

#define	BB_SHARE_13_START        (BB_SHARE_BASE+0xD0000)
#define BB_SHARE_13_SIZE         (0x00010000UL)
#define BB_SHARE_13_END          REGION_LIMIT(BB_SHARE_13_START, BB_SHARE_13_SIZE)

#define	BB_SHARE_00_13_START     (BB_SHARE_BASE)
#define BB_SHARE_00_13_SIZE      (0x000E0000UL)
#define BB_SHARE_00_13_END       REGION_LIMIT(BB_SHARE_00_13_START, BB_SHARE_00_13_SIZE)

#elif defined CONFIG_CHIP_VERSION_A1	// A1 has 11 64kB banks
#define	BB_SHARE_00_10_START     (BB_SHARE_BASE)
#define BB_SHARE_00_10_SIZE      (0x000B0000UL)
#define BB_SHARE_00_10_END       REGION_LIMIT(BB_SHARE_00_10_START, BB_SHARE_00_10_SIZE)
#endif


#define HIFI3_IRAM_START		 AHB_HIFI3_IRAM_BASE_S
#define HIFI3_IRAM_SIZE          AHB_HIFI3_IRAM_SIZE_S
#define HIFI3_IRAM_END           REGION_LIMIT(HIFI3_IRAM_START, HIFI3_IRAM_SIZE)

#define HIFI3_DRAM0_START        AHB_HIFI3_DRAM0_BASE_S
#define HIFI3_DRAM0_SIZE         AHB_HIFI3_DRAM0_SIZE_S
#define HIFI3_DRAM0_END          REGION_LIMIT(HIFI3_DRAM0_START, HIFI3_DRAM0_SIZE)

#define HIFI3_DRAM1_START        AHB_HIFI3_DRAM1_BASE_S
#define HIFI3_DRAM1_SIZE         AHB_HIFI3_DRAM1_SIZE_S
#define HIFI3_DRAM1_END          REGION_LIMIT(HIFI3_DRAM1_START, HIFI3_DRAM1_SIZE)


typedef struct {
    uint32_t (*_wr_reg)(volatile uint32_t* src, volatile uint32_t* dst, uint32_t len);
    uint8_t name[20];
} MEM_TIMING_S;

typedef struct {
    uint32_t* base_addr;
    uint32_t base_len;
} MEM_ACCESS_S;


/*This is a system map containing the base and length of each mem block*/
typedef struct {
#ifdef LLEA_TEST
    MEM_ACCESS_S llea_local0_1;
#endif
#ifdef LLEB_TEST
    MEM_ACCESS_S lleb_local0_3;
#ifdef ROM_TEST
    MEM_ACCESS_S lleb_rom;
#endif
#endif
#ifdef CAPEA_TEST
	MEM_ACCESS_S cape_a_local0_5;
#ifdef ROM_TEST
  	MEM_ACCESS_S cape_a_rom;
#endif
#endif
#ifdef CAPEB_TEST
	MEM_ACCESS_S cape_b_local0_3;
#ifdef ROM_TEST
  	MEM_ACCESS_S cape_b_rom;
#endif
#endif
#ifdef VECTOR_TEST
    MEM_ACCESS_S vector0_4;
#endif
#ifdef SHARED_MEM_TEST
    MEM_ACCESS_S sh_mem_00;
    MEM_ACCESS_S sh_mem_01;
    MEM_ACCESS_S sh_mem_02;
    MEM_ACCESS_S sh_mem_03;
    MEM_ACCESS_S sh_mem_04;
    MEM_ACCESS_S sh_mem_05;
    MEM_ACCESS_S sh_mem_06;
    MEM_ACCESS_S sh_mem_07;
    MEM_ACCESS_S sh_mem_08;
    MEM_ACCESS_S sh_mem_09;
    MEM_ACCESS_S sh_mem_10;
	#ifdef CONFIG_CHIP_VERSION_Z1 // Z1 has 13 64kB banks
    MEM_ACCESS_S sh_mem_11;
    MEM_ACCESS_S sh_mem_12;
    MEM_ACCESS_S sh_mem_13;    
	#endif

    MEM_ACCESS_S bb_mem_00;
    MEM_ACCESS_S bb_mem_01;
    MEM_ACCESS_S bb_mem_02;
    MEM_ACCESS_S bb_mem_03;
    MEM_ACCESS_S bb_mem_04;
    MEM_ACCESS_S bb_mem_05;
    MEM_ACCESS_S bb_mem_06;
    MEM_ACCESS_S bb_mem_07;
    MEM_ACCESS_S bb_mem_08;
    MEM_ACCESS_S bb_mem_09;
    MEM_ACCESS_S bb_mem_10;
	#ifdef CONFIG_CHIP_VERSION_Z1 // Z1 has 13 64kB banks
    MEM_ACCESS_S bb_mem_11;
    MEM_ACCESS_S bb_mem_12;
    MEM_ACCESS_S bb_mem_13;
	#endif
#endif
#ifdef HIFI3_TEST
	MEM_ACCESS_S hifi3_iram;
  	MEM_ACCESS_S hifi3_dram0;
   	MEM_ACCESS_S hifi3_dram1;
#endif
} SYSTEM_MEM_BANK_S;


typedef enum
{
    GES_GPIO_00,
    GES_GPIO_01,
    GES_GPIO_02,
    GES_GPIO_03,
    GES_GPIO_04,
    GES_GPIO_05,
    GES_GPIO_06,
    GES_GPIO_07,
    GES_GPIO_08,
    GES_GPIO_09,
    GES_GPIO_10,
    GES_GPIO_11,
    GES_GPIO_12,
    GES_GPIO_13,
    GES_GPIO_14,
    GES_GPIO_15,
    GES_GPIO_16,
    GES_GPIO_17,
    GES_GPIO_18,
    GES_GPIO_19,
    GES_GPIO_20,
    GES_GPIO_21,
    GES_GPIO_22,
    GES_GPIO_23,
    GES_GPIO_24,
    GES_GPIO_25,
    GES_GPIO_26,
    GES_EARL_SLV,
    GES_I2S1_MST,
    GES_I2S1_SLV,    
    GES_I2S0_MST,
    GES_I2S0_SLV,
    GES_SHR_MEM_CLK,
    GES_AHB_CLK,    
    GES_STD_RCO,
    GES_LP_RCO,    
    GES_MAX,
} gptc0_event_select;

#define GPTCO_EN    0x01
#define GPTCO_RUN   0x02
#define GPTCO_CNTNS 0x10

uint32_t vr_one_width32_inc_mem(volatile uint32_t* base_addr, uint32_t len);
uint32_t cnfg_tick_count(gptc0_event_select tick_src);
uint32_t rd_tick_count(void);
uint32_t pio_mem_access_system(uint32_t type);
uint32_t store_w32_var(volatile uint32_t* src, volatile uint32_t* dst, uint32_t len);
uint32_t set_mem_patt(volatile uint32_t* base_addr, uint32_t len, uint32_t patt);
#endif /*SYS_MEM_H*/
