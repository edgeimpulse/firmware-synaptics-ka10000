/*----------------------------------------------------------------------------
 * CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY
 *
 * The information contained in this source code file
 * is strictly confidential and proprietary to Conexant Systems, Inc.
 * ("Conexant")
 *
 * No part of this file may be possessed, reproduced or distributed, in
 * any form or by any means for any purpose, without the express written
 * permission of Conexant Systems Inc.
 *
 * Except as otherwise specifically provided through an express agreement
 * with Conexant that governs the confidentiality, possession, use
 * and distribution of the information contained in this file, CONEXANT
 * PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
 * AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
 * ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
 *
 * Copyright (C) 2009-2013 Conexant Systems Inc.
 * All rights reserved.
 *
 *----------------------------------------------------------------------------
 *
 * File: CxProgress.h
 *
 * Description: Character based Progress Bar (for tools)
 *
 *----------------------------------------------------------------------------
 */

void  InitShowProgress(const int MaxPos);
unsigned __stdcall ShowWaitingCursor( void* pArguments );
void  ShowProgress(int curPos, int bForceRedraw, int eState,const int MaxPos);
void set_txt_color(int color);
void SetSimpleConsoleProgress(const int enableSimpleConsolePrint);