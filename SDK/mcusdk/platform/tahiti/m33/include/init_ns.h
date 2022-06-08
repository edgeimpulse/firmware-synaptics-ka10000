#ifndef _MCU_INIT_NS_H_
#define _MCU_INIT_NS_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

int early_init_ns(void);
int driver_init_ns(void);
int sys_init_start_ns(void);

#ifdef __cplusplus
}
#endif

#endif // _MCU_INIT_NS_H_