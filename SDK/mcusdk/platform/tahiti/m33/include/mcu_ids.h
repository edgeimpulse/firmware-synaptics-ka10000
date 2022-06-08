////////////////////////////////////////////////////////////////////////////////
//
//  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Conexant Systems, Inc.
//  ("Conexant")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Conexant Systems Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Conexant that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, CONEXANT
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2009-2013 Conexant Systems Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ID_H_INCLUDED_
#define _ID_H_INCLUDED_

#define _ID(a,b,c,d)  ((uint32_t)((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26)))

#define hex2id0(id)  ((id>>8 )&0x3f)+0x20
#define hex2id1(id)  ((id>>14)&0x3f)+0x20
#define hex2id2(id)  ((id>>20)&0x3f)+0x20
#define hex2id3(id)  ((id>>26)&0x3f)+0x20
#define hex2id(id)   (((hex2id3(id))<<24) |((hex2id2(id))<<16) |((hex2id1(id))<<8) |(hex2id0(id)))


#define ID_MASK		    0xFFFFFF00
#define ID_BROADCAST    0xFFFFF00
#define ID_INIT		    _ID('I','N','I','T')

#define ID_MCU          _ID('M','C','U',' ')
#define ID_MM           _ID('<','M','M','>')
#define ID_CTRL         _ID('C','T','R','L')
#define ID_STRM         _ID('S','T','R','M')
#define ID_LEDS         _ID('L','E','D','S')
#define ID_PRNT         _ID('P','R','N','T')
#define ID_PRNS         _ID('P','R','N','S')
#define ID_CLII         _ID('C','L','I','I')
#define ID_TEST         _ID('T','E','S','T')
#define ID_ELKM         _ID('E','L','K','M')
#define ID_ELKS         _ID('E','L','K','S')
#define ID_KEYS         _ID('K','E','Y','S')

#define ID_INDX_FILE    _ID('I','N','D','X')
#define ID_EOF          _ID('E','O','F','0')

#ifdef CONFIG_MCU_CTRL_ELK_TUNNELING
#define ID_FWCT_LOC(x)  ((x==ELK_MST_MODE) ? _ID('F','W','C','T') : _ID('F','W','X','T')) //CORE-A 
#define ID_FWBT_LOC(x)  ((x==ELK_MST_MODE) ? _ID('F','W','B','T') : _ID('F','W','Y','T')) //CORE-B 
#define ID_FWCT         ID_FWCT_LOC(TAHITI_MODE)
#define ID_FWBT         ID_FWBT_LOC(TAHITI_MODE)
#define ID_FWXT         _ID('F','W','X','T') //CORE-A 
#define ID_FWYT         _ID('F','W','Y','T') // for mcu forwarding sys commands to CAPE-B 
#else
#define ID_FWCT         _ID('F','W','C','T') //CORE-A 
#define ID_FWBT         _ID('F','W','B','T') // for mcu forwarding sys commands to CAPE-B 
#endif

#define ID_MEM_SYS      _ID('M','S','Y','S') // SYS Memory Heap ID
#define ID_PMNG         _ID('P','M','N','G')
#define ID_UADT         _ID('U','A','D','T')
#define ID_IIPC         _ID('I','I','P','C')
#define ID_SFUG         _ID('S','F','U','G')

#endif	// _ID_H_INCLUDED_
