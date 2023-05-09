/*------------------------------------------------------------------------------
  Copyright (C) 2018 Synaptics Inc.
  All rights reserved.

  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Synaptics Inc.
  ("Synaptics")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Synaptics Inc.

  Except as otherwise specifically provided through an express agreement
  with Synaptics that governs the confidentiality, possession, use
  and distribution of the information contained in this file, SYNAPTICS
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: syna_types.h

------------------------------------------------------------------------------*/
#ifndef SYNA_TYPES_H
#define SYNA_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <stdint.h>
#include <stdarg.h>

#define AUDIOSMART_DEBUG_PRINT(...)

#ifndef NULL
#define NULL 0
#endif

#ifdef SYNA_HOST_EXPORT_DLL
#define SYNA_EXPORT_DLL __declspec(dllexport)
#else
#define SYNA_EXPORT_DLL
#endif


#ifdef __cplusplus
}
#endif


#endif
