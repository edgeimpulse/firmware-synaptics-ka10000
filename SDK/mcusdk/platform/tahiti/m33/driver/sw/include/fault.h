////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Synaptics that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, SYNAPTICS
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2018 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_DRV_FAULT_H_
#define _MCU_DRV_FAULT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if 1 //def CONFIG_MCU_DRV_FAULT

typedef struct _exc_frame_s {
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;       // IP
    uint32_t R14;       // LR
    uint32_t R15;       // PC
    uint32_t XPSR;
    float S_0_to_15[16];
    uint32_t FPSCR;
    uint32_t Reserved;
} exc_frame_t;

typedef struct _fault_info_s {
    exc_frame_t *ef;
    uint32_t fault;
    uint32_t fault_addr;
    uint32_t fault_addr_valid;    
} fault_info_t;

#define MCU_FAULT_MAKE_FAULT(fault_major, fault_minor)\
    (((fault_major) << MCU_FAULT_MAJOR_OFFSET) | fault_minor)

#define MCU_FAULT_MAJOR_OFFSET          (16U)

#define MCU_FAULT_UNKNOWN               (-1)

#define MCU_FAULT_HARD                  (3)
#define MCU_FAULT_HARD_VECTTBL          MCU_FAULT_MAKE_FAULT(MCU_FAULT_HARD, 1)     // Vector table read error on exception processing

#define MCU_FAULT_MEMMAN                (4)
#define MCU_FAULT_MEMMAN_IACCVIOL       MCU_FAULT_MAKE_FAULT(MCU_FAULT_MEMMAN, 1)   // Instruction access violation
#define MCU_FAULT_MEMMAN_DACCVIOL       MCU_FAULT_MAKE_FAULT(MCU_FAULT_MEMMAN, 2)   // Data Access Violation
#define MCU_FAULT_MEMMAN_MUNSTKERR      MCU_FAULT_MAKE_FAULT(MCU_FAULT_MEMMAN, 3)   // Unstacking error
#define MCU_FAULT_MEMMAN_MSTKERR        MCU_FAULT_MAKE_FAULT(MCU_FAULT_MEMMAN, 4)   // Stacking error flag
#define MCU_FAULT_MEMMAN_MLSPERR        MCU_FAULT_MAKE_FAULT(MCU_FAULT_MEMMAN, 5)   // Lazy state preservation error flag

#define MCU_FAULT_BUS                   (5)
#define MCU_FAULT_BUS_PERCISERR         MCU_FAULT_MAKE_FAULT(MCU_FAULT_BUS, 1)      // Precise Error
#define MCU_FAULT_BUS_IBUSERR           MCU_FAULT_MAKE_FAULT(MCU_FAULT_BUS, 2)      // Instruction prefetch
#define MCU_FAULT_BUS_IMPERCISERR       MCU_FAULT_MAKE_FAULT(MCU_FAULT_BUS, 3)      // Imprecise data access
#define MCU_FAULT_BUS_UNSTKERR          MCU_FAULT_MAKE_FAULT(MCU_FAULT_BUS, 4)      // Exception return unstacking
#define MCU_FAULT_BUS_STKERR            MCU_FAULT_MAKE_FAULT(MCU_FAULT_BUS, 5)      // Exception entry stacking
#define MCU_FAULT_BUS_LSPERR            MCU_FAULT_MAKE_FAULT(MCU_FAULT_BUS, 6)      // FP lazy state preservation

#define MCU_FAULT_USAGE                 (6)
#define MCU_FAULT_USAGE_UNDEFINESTR     MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 1)    // Undefined instruction
#define MCU_FAULT_USAGE_INVSTATE        MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 2)    // EPSR, invalid
#define MCU_FAULT_USAGE_INVPC           MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 3)    // Integrity check error of loading EXC_RETURN to PC
#define MCU_FAULT_USAGE_NOCP            MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 4)    // Coprocessor disabled or not exists
#define MCU_FAULT_USAGE_STKOF           MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 5)    // Jeff Atwood and Joel Spolsky
#define MCU_FAULT_USAGE_UNALIGNED       MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 6)    // Unaligned mem access
#define MCU_FAULT_USAGE_DIVBYZERO       MCU_FAULT_MAKE_FAULT(MCU_FAULT_USAGE, 7)    // Div by zero

#define MCU_FAULT_SECURE                (7)
#define MCU_FAULT_SECURE_INVEP          MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 1)   // NS call to S without SG, or SG is not in NSC region
#define MCU_FAULT_SECURE_INVIS          MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 2)   // Integrity signature invalid when unstacking in Exception
#define MCU_FAULT_SECURE_INVER          MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 3)   // Invalid EXC_RETURN
#define MCU_FAULT_SECURE_AUVIOL         MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 4)   // NS-Req to Secure AU
#define MCU_FAULT_SECURE_INVTRAN        MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 5)   // Direct Access NS from S
#define MCU_FAULT_SECURE_LSPERR         MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 6)   // SAU or IDAU violation occurred during the lazy preservation of floating-point state
#define MCU_FAULT_SECURE_LSERR          MCU_FAULT_MAKE_FAULT(MCU_FAULT_SECURE, 7)   // Lazy state activation or deactivation error

typedef int32_t (*fault_handler)(fault_info_t *info);

int32_t install_fault_handler(fault_handler hdl);

#endif //CONFIG_MCU_DRV_FAULT
#ifdef __cplusplus
}
#endif
#endif // _MCU_DRV_FAULT_H_