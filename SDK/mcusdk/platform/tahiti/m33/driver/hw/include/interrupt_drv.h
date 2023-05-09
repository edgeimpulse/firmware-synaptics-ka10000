/** \file  interrupt_drv.h
 * \brief
 *
 * \author Shahabuddin K
 * \version 0.1
 *
 * \remark Copyright:           (c) 2015 Conexant Systems Inc.
 */
#ifndef _MCU_INTERRUPT_DRV_H_
#define _MCU_INTERRUPT_DRV_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif
        
#ifdef CONFIG_MCU_DRV_INTERRUPT_CTRL

/* ------------------------------------------------------------------------------ */
/* ARM-M33 CPU-1 SSE200 Interrupt Source (32 interrupts)						              */
/* Start of IRQ definition						                                            */
/* ------------------------------------------------------------------------------ */
#define IRQ_NON_SEC_WDT_RST_REQ   	0
#define IRQ_NON_SEC_WDT_INTR      	1
#define IRQ_TMR_S32K      			2
#define IRQ_TMR_0				   	3
#define IRQ_TMR_1   				4
#define IRQ_TMR_DUAL   				5
#define IRQ_MHU_0_CPU1_INTR   		6
#define IRQ_MHU_1_CPU1_INTR   		7
#define RESERVED_0   				8
#define IRQ_MPC   					9
#define IRQ_PPC   					10
#define IRQ_MSC   					11
#define IRQ_BRIDGERR_COMB_INTR   	12
#define IRQ_CPU1_INSTR_CACH_INTR   	13
#define RESERVED_1   				14
#define IRQ_SYS_PPU					15
#define IRQ_CPU0_PPU				16
#define IRQ_CPU1_PPU				17
#define IRQ_CPU0_DBG_PPU			18
#define IRQ_CPU1_DBG_PPU			19
#define RESERVED_2					20
#define RESERVED_3   				21
#define IRQ_RAM0_PPU   				22
#define IRQ_RAM1_PPU   				23
#define IRQ_RAM2_PPU   				24
#define IRQ_RAM3_PPU   				25
#define IRQ_DEBUG_PPU   			26
#define RESERVED_4   				27
#define IRQ_CPU1_CTIIRQ0   			28
#define IRQ_CPU1_CTIIRQ1   			29
#define RESERVED_5					30
#define RESERVED_6	  				31

/* ------------------------------------------------------------------------------ */
// Below are External Interrupts for TAHITI
// These interrupts map from 32 to 95 at the top level (total 64 interrupt source)
/* ------------------------------------------------------------------------------ */
#define IRQ_LOW_BAT_BROWN_OUT   	32 // 0
#define IRQ_PMU_IRQ   				33 // 1
#define IRQ_LLEA_SOFT_IRQ   		34 // 2
#define IRQ_LLEB_SOFT_IRQ   		35 // 3
#define IRQ_CAPEA_SOFT_IRQ   		36 // 4
#define IRQ_CAPEB_SOFT_IRQ   		37 // 5
#define IRQ_ARM_SOFT_IRQ   			38 // 6
#define IRQ_HIFI3_SOFT_IRQ   		39 // 7
#define IRQ_BUS_ERR_SOFT_IRQ   		40 // 8
#define IRQ_FLEXI_DMA_0   		  	41 // 9
#define IRQ_FLEXI_DMA_1   		  	42 // 10
#define IRQ_AUD_DMA_0   		  	43 // 11
#define IRQ_AUD_DMA_1   		  	44 // 12
#define IRQ_SPI_RX   			  	45 // 13
#define IRQ_SPI_TX   			  	46 // 14
#define IRQ_GPIO   				  	47 // 15
#define IRQ_I2C_MASTER   		  	48 // 16
#define IRQ_I2C_SLAVE   		  	49 // 17
#define IRQ_I3C   				  	50 // 18
#define IRQ_CACHE_IRQ    		  	51 // 19
#define IRQ_DAC   				  	52 // 20
#define IRQ_ASIP_TIMER_0_LLEA	  	53 // 21
#define IRQ_ASIP_TIMER_0_LLEB       54 // 22
#define IRQ_ASIP_TIMER_0_CAPEA	  	55 // 23
#define IRQ_ASIP_TIMER_0_CAPEB	  	56 // 24
#define IRQ_ASIP_TIMER_1  	  	  	57 // 25
#define IRQ_ASIP_TIMER_2  	   	  	58 // 26
#define IRQ_ASIP_TIMER_3  	  	  	59 // 27
#define IRQ_ASIP_TIMER_4  	  	  	60 // 28
#define IRQ_ASIP_TIMER_5  	  	  	61 // 29
#define IRQ_ASIP_TIMER_6   			62 // 30
#define IRQ_ASIP_TIMER_7   			63 // 31
#define IRQ_GPTC_TMR_0   			64 // 32
#define IRQ_GPTC_TMR_1   			65 // 33
#define IRQ_SPTC_TMR   				66 // 34
#define IRQ_QSPI_INTR   			67 // 35
#define IRQ_LL_DECIM_3 				68 // 36
#define IRQ_LL_DECIM_2 				69 // 37
#define IRQ_LL_DECIM_1 				70 // 38
#define IRQ_ELK_TO_LLR_0	  	  	71 // 39
#define IRQ_ELK_TO_LLR_1	  	  	72 // 40
#define IRQ_EARLINK_SYS_IRQ   		73 // 41
#define IRQ_AES_INTR   				74 // 42
#define IRQ_VAD_DETECT   			75 // 43
#define IRQ_CLK_DETECT   			76 // 44
#define IRQ_LL_DECIM_0		   		77 // 45
#define IRQ_SYNC_INTER_0   			78 // 46
#define IRQ_SYNC_INTER_1   			79 // 47
#define IRQ_SYNC_INTER_2   			80 // 48
#define IRQ_SYNC_INTER_3   			81 // 49
#define IRQ_SLEEP_TYPEOUT   		82 // 50
#define IRQ_MAILBOX_INTR_0   		83 // 51
#define IRQ_MAILBOX_INTR_1   		84 // 52
#define IRQ_MAILBOX_INTR_2   		85 // 53
#define IRQ_MAILBOX_INTR_3   		86 // 54
#define IRQ_MAILBOX_INTR_4   		87 // 55
#define IRQ_MAILBOX_INTR_5          88 // 56
#define IRQ_SECURITY_VIOL           89 // 57
#define IRQ_HIFI3_INTR   			90 // 58
#define IRQ_LLR_SAMPLES_MISSED		91 // 59
#define IRQ_LOCK_INTR   			92 // 60
#define IRQ_UART_INTR   			93 // 61
#define IRQ_WD_INTR   				94 // 62
#define IRQ_CLASSD_CURRENT          95 // 63

/* ---------------------------------------------------------------------------------------------------------------- */
// End of IRQ definition
/* ---------------------------------------------------------------------------------------------------------------- */

typedef int (*irq_func_t) (UINT32 param);
extern void ARM_IRQ_Handler(void);

/* ---------------------------------------------------------------------------------------------------------------- */
#define sys_request_irq				    irq_request
#define sys_release_irq				    irq_release
#define sys_enable_irq_nr			    irq_enable
#define sys_disable_irq_nr			  irq_disable
#define sys_get_irq_context			  irq_get_context
#define sys_restore_irq				    irq_restore
#define sys_disable_irq_save		  irq_disable_save

typedef enum {
    IRQ_SECTION_OS_MANAGED_OS_SAFE      = 0,
    IRQ_SECTION_OS_UNMANAGED_OS_UNSAFE  = 1,
} irq_section_t;

typedef int (*irq_func_t)	(unsigned int param);

__STATIC_FORCEINLINE uint32_t irq_get_context(void) {
    IPSR_Type IPSR;
    IPSR.w = __get_IPSR();
    return IPSR.b.ISR;
}

__STATIC_FORCEINLINE uint32_t irq_disable_save(void) {
    uint32_t saved_primask;
    __asm volatile
    (
    " mrs %[SAVED_PRIMASK], primask       \n"
    " cpsid i                             \n"
    : [SAVED_PRIMASK] "=r" (saved_primask)
    :
    : "memory"
    );

    return !!saved_primask;
}

__STATIC_FORCEINLINE uint32_t irq_disable_save_ex(uint32_t new_prio) {
    uint32_t saved_basepri;
    __asm volatile
    (
    " mrs %[SAVED_BASEPRI], basepri        \n"
    " msr basepri, %[NEW_BSSEPRI]          \n"
    : [SAVED_BASEPRI] "=&r"(saved_basepri)
    : [NEW_BSSEPRI] "r"(new_prio)
    : "memory"
    );

    return saved_basepri;
}

__STATIC_FORCEINLINE void irq_restore(uint32_t result) {
    if (!result) {
        __enable_irq();
    }
}

__STATIC_FORCEINLINE void irq_restore_ex(uint32_t saved_prio) {
    __set_BASEPRI(saved_prio);
}

int drv_interrupt_start(void);

int irq_request(unsigned int irq, unsigned int priority, irq_func_t cb_func, unsigned int cb_param);

int irq_request_ex(unsigned int irq, unsigned int relative_prio, irq_section_t irq_section, irq_func_t cb_func, unsigned int cb_param);

int irq_release(unsigned int irq);

int irq_enable(unsigned int irq);

int irq_disable(unsigned int irq);



#endif //CONFIG_MCU_DRV_INTERRUPT_CTRL

#ifdef __cplusplus
}
#endif

#endif //_MCU_INTERRUPT_DRV_H_
