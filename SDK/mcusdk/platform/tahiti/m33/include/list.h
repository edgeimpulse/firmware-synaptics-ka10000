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

#ifndef _MCU_LIST_H_INCLUDED_
#define _MCU_LIST_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
//
//  Double linked list.
//
struct list_s {
  struct list_s *prev;
  struct list_s *next;
};

int __STATIC_INLINE _list_is_empty(struct list_s * e) {
  return e->next==e;
}

void __STATIC_INLINE _list_initialize(struct list_s * e) {
  e->prev=e->next=e;
}

void __STATIC_INLINE _list_add_before(struct list_s * e,struct list_s * l) {
  e->prev=l->prev;
  e->next=l;
  l->prev->next=e;
  l->prev=e;
}

void __STATIC_INLINE _list_add_after(struct list_s * e,struct list_s * l) {
  e->next=l->next;
  e->prev=l;
  l->next->prev=e;
  l->next=e;
}

void __STATIC_INLINE _list_del(struct list_s * e) {
  e->next->prev=e->prev;
  e->prev->next=e->next;
  _list_initialize(e);
}

#ifdef __cplusplus
}
#endif

#endif 	// _MCU_LIST_H_INCLUDED_
