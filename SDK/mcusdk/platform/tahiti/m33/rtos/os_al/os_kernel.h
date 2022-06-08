#ifndef OS_KERNEL_H_
#define OS_KERNEL_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Kernel state
typedef enum {
  osKernelInactive        =  0,         ///< Inactive.
  osKernelReady           =  1,         ///< Ready.
  osKernelRunning         =  2,         ///< Running.
  osKernelLocked          =  3,         ///< Locked.
  osKernelSuspended       =  4,         ///< Suspended.
  osKernelError           = -1,         ///< Error.
  osKernelReserved        = 0x7FFFFFFFU ///< Prevents enum down-size compiler optimization.
} os_KernelState_t;

/**********************************************************************************
 os_KernelInitialize()
 initialize kernel with required resource.
 Returns: int (0 or -1)
**********************************************************************************/
int os_KernelInitialize (void);

/**********************************************************************************
 os_KernelStart()
 Start kernel scheduler.
 Returns: int (0 or -1)
**********************************************************************************/
int os_KernelStart (void);

/**********************************************************************************
 osKernelGetState()
 Get the current RTOS Kernel state.
 Returns: os_KernelState_t (current RTOS Kernel state)
**********************************************************************************/
os_KernelState_t os_KernelGetState (void);

#ifdef __cplusplus
}
#endif

#endif // #ifndef OS_KERNEL_H_