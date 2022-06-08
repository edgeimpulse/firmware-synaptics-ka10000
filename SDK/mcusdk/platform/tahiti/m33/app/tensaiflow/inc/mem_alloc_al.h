/*

    Interface definitions for mem_alloc_al.c, the memory alloc abstraction layer.

*/
#include "config.h"
#include <stdint.h>
#include "mcu_types.h"

typedef enum MEM_TYPE
{
    SRAM,
    SHM 
}MEM_TYPE_t;

int16_t getMemAllocCount(MEM_TYPE_t mem);
void* allocMem(MEM_TYPE_t mem, uint32_t size);
void freeMem(void* ptr);
void* copyMem(void * __restrict s1, const void * __restrict s2, size_t n);
void* preAllocMem(MEM_TYPE_t mem, uint32_t size);
void preAllocFreeMem(void* bufAddr);
int initSharedMemHeap(void);
