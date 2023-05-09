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

#ifndef _MCU_DRV_HEAP_H_
#define _MCU_DRV_HEAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(__DOMAIN_NS) && defined(CONFIG_MCU_DRV_HEAP_NS)) || !defined(__DOMAIN_NS)

#if !defined(__DOMAIN_NS)
void *pvPortMalloc( size_t xSize );
void vPortFree( void *pv );
void vPortInitialiseBlocks( void );
size_t xPortGetFreeHeapSize( void );
size_t xPortGetMinimumEverFreeHeapSize( void );
#endif // __DOMAIN_NS

#define HEAP_ALLOCATOR_IOCTRL_CMD_INIT          (0U)
#define HEAP_ALLOCATOR_IOCTRL_CMD_MALLOC        (1U)
#define HEAP_ALLOCATOR_IOCTRL_CMD_FREE          (2U)
#define HEAP_ALLOCATOR_IOCTRL_CMD_REMAINING     (3U)
#define HEAP_ALLOCATOR_IOCTRL_CMD_PRIVATE_CTRL  (4U)

#define HEAP_FLAGS_ALGIN_SHIFT                  (0U)
#define HEAP_FLAGS_ALGIN_MASK                   (0xFFU)
#define HEAP_FLAGS_ALIGN_BYTES_2                (2U)
#define HEAP_FLAGS_ALIGN_BYTES_4                (4U)
#define HEAP_FLAGS_ALIGN_BYTES_8                (8U)
#define HEAP_FLAGS_ALIGN_BYTES_16               (16U)
#define HEAP_FLAGS_ALIGN_BYTES_32               (32U)

//Todo: Support bellow flags to support base size of heap_malloc
#define HEAP_FLAGS_INT_SIZE_SHIFT               (8U)
#define HEAP_FLAGS_INT_SIZE_MASK                (0xFFU)
#define HEAP_FLAGS_INT_SIZE___8                 (1U)
#define HEAP_FLAGS_INT_SIZE__16                 (2U)
#define HEAP_FLAGS_INT_SIZE__32                 (4U)
#define HEAP_FLAGS_INT_SIZE__64                 (8U)
#define HEAP_FLAGS_INT_SIZE_128                 (16U)

#define HEAP_FLAGS_ALLOCATOR_SHIFT              (16U)
#define HEAP_FLAGS_ALLOCATOR_MASK               (0xFU)
#define HEAP_FLAGS_ALLOCATOR_1                  (0U)
#define HEAP_FLAGS_ALLOCATOR_2                  (1U)

typedef union _heap_flags_u {
    uint32_t u32;
    struct
    {
        uint32_t align      : 8;
        uint32_t int_size   : 8;
        uint32_t allocator  : 4;
        uint32_t _RESERVED  : 12;
    } b;
} heap_flags;

typedef struct _heap_ops_s {
    int32_t (*ioctrl)(void* privat, uint32_t cmd, void *param);
} heap_ops_t;

typedef struct _heap_info_s {
    uint32_t owner_id;
    uint32_t addr_start;
    size_t size_b;
    heap_flags flags;
    void * heap_private;
} heap_info_t;

typedef void * PHEAP_HANDLE;

/**
 * @brief create heap
 * 
 * @param [in] owner_id    four CC id
 * @param [in] start_addr  start memory addr used by heap
 * @param [in] size_b      size of the memory section
 * @param [in] flags       uesed to create flags of heap_flags
 * @return    none null value succeed, null value fail
 */
PHEAP_HANDLE heap_create(uint32_t owner_id, uint32_t start_addr, size_t size_b, uint32_t flags);

/**
 * @brief malloc memory
 *
 * @param [in] handle     heap handle created by heap_create
 * @param [in] size       wanted memory size
 * @return    none null value succeed, null value fail
 */
void * heap_malloc(PHEAP_HANDLE handle, size_t size);

/**
 * @brief malloc memory by required alignment
 * 
 * @param [in] handle     heap handle created by heap_create
 * @param [in] size       wanted memory size
 * @param [in] align      memory alignment
 * @return    none null value succeed, null value fail
 */
void * heap_malloc_align(PHEAP_HANDLE handle, size_t size, uint32_t align);

/**
 * @brief free memory allocated by heap_malloc
 * 
 * @param [in] handle     heap handle created by heap_create
 * @param [in] pointer    memory pointer
 * @return    0 succeed, negative fail
 */
int32_t heap_free(PHEAP_HANDLE handle, void * pointer);

/**
 * @brief free memory allocated by heap_malloc_align
 * 
 * @param [in] handle     heap handle created by heap_create
 * @param [in] pointer    memory pointer
 * @return    0 succeed, negative fail
 */
int32_t heap_free_align(PHEAP_HANDLE handle, void * pointer);

/**
 * @brief heap private control, different heap algorithm may have
 *        its own control command
 * 
 * @param [in] handle     heap handle created by heap_create
 * @param [in] param      memory private control
 * @return    0 succeed, negative fail
 */
int32_t heap_private_ctrl(PHEAP_HANDLE handle, void * param);

#endif // (__DOMAIN_NS && CONFIG_MCU_DRV_HEAP_NS) || !__DOMAIN_NS
#ifdef __cplusplus
}
#endif
#endif // _MCU_DRV_HEAP_H_
