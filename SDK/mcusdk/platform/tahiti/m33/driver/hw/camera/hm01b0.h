#ifndef _HM01B0_H_
#define _HM01B0_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef CONFIG_CAMERA_SELECT_HM01B0

void set_work_mode(int mode);
int reset_hm01b0();
int init_hm01b0();
void camera_set_reg(uint16_t addr, uint8_t data);
uint8_t camera_get_reg(uint16_t addr);

#endif

#ifdef __cplusplus
}
#endif

#endif // _HM01B0_HW_H_
