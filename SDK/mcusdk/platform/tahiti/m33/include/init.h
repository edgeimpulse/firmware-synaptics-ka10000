#ifndef _MCU_INIT_H_
#define _MCU_INIT_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

int early_init(void);
int driver_init_s(void);
int sys_init_start_s(void);

#ifdef __cplusplus
}
#endif

#endif // _MCU_INIT_H_