#ifndef _MCU_H_
#define _MCU_H_

#include <autoconf.h>

#include <string.h>
#include <stdint.h>
#include <arm_cmse.h>

#include "cmsis.h"
#include "list.h"
#include "mcu_ids.h"

#if defined(CONFIG_MCU_M33_BOOT) || defined(CONFIG_MCU_M33_LIVE)
#undef CONFIG_MCU_RTOS_FRTOS
#undef CONFIG_MCU_SECURE_MODE_RTOS
#endif

#if defined(CONFIG_MCU_RTOS_FRTOS) && ((defined(CONFIG_MCU_SECURE_MODE_RTOS) && !defined(__DOMAIN_NS)) || (defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)))
	#include "FreeRTOS.h"
	#include "os_mem.h"
	#include "os_sem.h"
	#include "os_queue.h"
	#include "os_task.h"
	#include "os_timer.h"
	#include "os_kernel.h"
#elif defined(CONFIG_MCU_RTOS_RTX) && ((defined(CONFIG_MCU_SECURE_MODE_RTOS) && !defined(__DOMAIN_NS)) || (defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)))
	// place holder
#elif defined(CONFIG_MCU_RTOS_NONE) && ((defined(CONFIG_MCU_SECURE_MODE_RTOS) && !defined(__DOMAIN_NS)) || (defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)))
	// No RTOS Selected. So running bare-metal code.
#endif // CONFIG_MCU_RTOS_FRTOS/RTX

#if defined(CONFIG_MCU_DRV_HEAP_S) || defined(CONFIG_MCU_DRV_HEAP_NS)
#include "heap.h"
#include "../rtos/os_al/os_mem.h"
#endif

#include "major.h"
#include "minor.h"
#include "devices.h"
#include "errno.h"
#include "mcu_types.h"
#include "driver.h"
#include "print.h"

typedef int                 BOOL;

#define Is_ICacheEnabled() \
   (*( (volatile uint32_t *)0x50010004) & 1)

#if defined(CONFIG_MCU_RTOS_FRTOS) && ((defined(CONFIG_MCU_SECURE_MODE_RTOS) && !defined(__DOMAIN_NS)) || (defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)))   
	// TBD
#endif

#if (defined(__DOMAIN_NS) && defined(CONFIG_MCU_DRV_HEAP_FOR_OS_HEAP_NS)) || defined(__DOMAIN_S)
	#define __INITCODE__ __attribute__((section(".reclaim_call"))) 
#else
	#define __INITCODE__ 
#endif

#ifdef CONFIG_M33_LINKING_SYSMEM
#define __SYSDATA__     __attribute__((section(".sm_data_mem")))
#define __SYSRODATA__   __attribute__((section(".sm_rodata_mem")))
#define __SYSBSS__		__attribute__((section(".sm_bss_mem")))
#else
#define __SYSDATA__
#define __SYSRODATA__
#define __SYSBSS__
#endif

#ifndef MEMORY_INIT_MAGIC_NUMBER
#define  MEMORY_INIT_MAGIC_NUMBER 0x1a1a1a1a
#endif 

#define CAPE_ID_A       1
#define CAPE_ID_B       2
#define DEBUG_CMD_MGAIC  (0xDEB0DEB0)

#endif // _MCU_H_
