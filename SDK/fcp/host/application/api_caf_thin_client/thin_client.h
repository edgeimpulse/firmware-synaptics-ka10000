/*------------------------------------------------------------------------------
  Copyright (C) 2010-2018 Conexant Systems Inc.
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

  File Name:  thin_client.h

  Description:  Macro/Define for Thin client
------------------------------------------------------------------------------*/

#ifndef THIN_CLIENT_H_
#define THIN_CLIENT_H_

#include <stdint.h>

#define DEFAULT_BUFFER_LEN           4096 
#define MAX_AMIXER_NAME_LEN	     50

#define SENDCMD_TAG                  "sendcmd "
#define SENDCMD_TAG_LEN              (sizeof(SENDCMD_TAG)-1)
#define MAX_RETURN_BUF               (MAX_COMMAND_SIZE *9 +3)

#endif  // THIN_CLIENT_H_


