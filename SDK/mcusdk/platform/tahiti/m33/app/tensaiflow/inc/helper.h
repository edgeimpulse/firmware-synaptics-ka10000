//////////////////////////////////////////////////////////////////////////
// The confidential and proprietary information contained in this file may
// only be used by a person authorized under and to the extent permitted
// by a subsisting licensing agreement from Eta Compute Inc.
//
//        (C) Copyright 2019 Eta Compute Inc
//                All Rights Reserved
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from Eta Compute Inc.
//////////////////////////////////////////////////////////////////////////
#ifndef H_HELPER_
#define H_HELPER_
#include "config.h"
#include "autoconf.h"
#include "mcu.h"

#define GET_CYCLE_CNT (DWT->CYCCNT)
#define GET_CPU_FREQ (CONFIG_M33_SYSTEM_CLOCK)
#endif
