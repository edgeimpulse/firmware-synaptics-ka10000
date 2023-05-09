/*------------------------------------------------------------------------------
  Copyright (C) 2010-2019 Conexant Systems Inc.
  All rights reserved.

  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Conexant Systems, Inc.
  ("Conexant")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Conexant Systems Inc.

  Except as otherwise specifically provided through an express agreement
  with Conexant that governs the confidentiality, possession, use
  and distribution of the information contained in this file, CONEXANT
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
--------------------------------------------------------------------------------

  File Name: sim_io.h

  Description: Simulator input and output. A single sim I/O object provides 
               audio content for a single signal stream. 
------------------------------------------------------------------------------*/
#ifndef AUDIODSP_TEST_SIMMULATOR_IO_STRUCT_H_
#define AUDIODSP_TEST_SIMMULATOR_IO_STRUCT_H_



#include "autoconf.h"
#include "signal_stream.h"






#ifdef __cplusplus
extern "C"
{
#endif


  typedef struct IOInterfaceConfigTag {

    uint32_t       io_id;
    uint32_t       interface_type;
    SignalType     signal_type;
    uint32_t       buffer_size;
    uint32_t       flags;
    uint32_t       irq_interval; 
    uint32_t       trigger;

  }IOInterfaceConfig;



#ifdef __cplusplus
}
#endif

#endif  // AUDIODSP_TEST_SIMMULATOR_IO_STRUCT_H_
