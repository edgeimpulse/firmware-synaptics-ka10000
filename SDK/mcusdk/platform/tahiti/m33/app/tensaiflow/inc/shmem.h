#ifndef H_SHMEM_
#define H_SHMEM_
#include <stdint.h>
#include <stdbool.h>

#define ADDR_IS_SHARED_MEM(a)  (((((uint32_t)(a) >= 0x41500000) && ((uint32_t)(a)<=0x415FFFFF))) ? 1:0)

int SharedMemInit(void* startAddress, uint32_t size);
void SharedMemDeinit(void);
void* SharedMemAlloc(uint32_t size);
void  SharedMemFree(void* mem);
bool SharedMemIdentifyAddress(void* address);
uint32_t SharedMemGetOffset(void* mem); // offset in bytes
void* SharedMemGetAddress(uint32_t offset);
#endif//# H_SHMEM_
