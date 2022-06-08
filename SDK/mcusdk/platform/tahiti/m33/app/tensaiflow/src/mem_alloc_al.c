#define PRINT_LEVEL_SET ASSERT_LEVEL_ERR
#include "mem_alloc_al.h"
#include "driver.h"
#include "shmem.h"
#include "autoconf.h"
#include "mcu.h"
#include "rpc.h"
#include "preAlloc.h"
#include "workQ_common.h"
#include "executor_public.h"

static uint16_t shmAllocCount = 0;
static uint16_t sramAllocCount = 0;

typedef struct __PRE_ALLOC_BUFF_RANGE_
{
    unsigned int start;
    unsigned int end;
}preAllocBufRange_t;

// TODO : Change to assert after the code is stable
#define ERR_PRE_ALLOC()   Print_ERR("ERROR : preAlloc");
#define PRE_ALLOC_ERR     (-1)

static uint16_t preAllocNextFreeIdx = 0;
static uint16_t preAllocFreeCount = 0;

static int32_t popPreAlloc(uint32_t size);
static void releasePreAlloc(void* ptr);

#ifdef __GNUC__
tSysSharedMemory volatile __SYSBSS__ shmemM3Dsp = {0};
// TBD: use GCC directive to allocate this memory @ the sharedmem area specified in linker file.
#endif

#define MEMORY_PATTERN 0XDEAD

int initSharedMemHeap(void)
{
  shmemM3Dsp.topMemoryBoundaryPattern &= 0xFFFF;
  shmemM3Dsp.topMemoryBoundaryPattern |= (MEMORY_PATTERN << 16) ;
//   shmemM3Dsp.bottomMemoryBoundaryPattern = shmemM3Dsp.topMemoryBoundaryPattern;
  return SharedMemInit(shmemM3Dsp.byteArray, SHARED_MEMORY_ALLOCATOR_SIZE);
}

int16_t getMemAllocCount(MEM_TYPE_t mem)
{
    if(mem == SRAM)
        return sramAllocCount;
    else if(mem == SHM)
        return shmAllocCount;
    else
        return 0;
}

void* allocMem(MEM_TYPE_t mem, uint32_t size)
{
    void* ptr = NULL;

    if(size == 0)
        return NULL;

    switch(mem)
    {
        case SRAM:
            ptr = os_Malloc(size);
            sramAllocCount++;
            Print_DBG("SRAM alloc, ptr : 0x%x, size : 0x%x\r\n", ptr,size);
            break;
        case SHM:
            ptr = SharedMemAlloc(size);
            shmAllocCount++;
            Print_DBG("SHM alloc, ptr : 0x%x, size : 0x%x\r\n", ptr,size);
            break;
        default:
            Print_ERR("ERROR : Unknown mem type");
            break;
    }

    return ptr;
}

void freeMem(void* ptr)
{
    if(ptr != NULL)
    {
        if(ADDR_IS_SHARED_MEM(ptr))
        {
            Print_DBG("SHM free, ptr : 0x%x\r\n", ptr);
            SharedMemFree(ptr);
            shmAllocCount--;
        }
        else
        {
            Print_DBG("SRAM free, ptr : 0x%x\r\n", ptr);
            os_Free(ptr);
            sramAllocCount--;
        }
    }
}

void* copyMem(void * s1, const void * s2, size_t n)
{
    char* ll = (char*)s1;
    char* rr = (char*)s2;
    for (size_t i = 0; i < n; i++) ll[i] = rr[i];
    return s1;
}

void* preAllocMem(MEM_TYPE_t mem, uint32_t size)
{
    void* ptr = NULL;
    void *ptrBase = ((uint32_t)&shmemM3Dsp.preAllocArray + 7) & ~0x7;
    uint32_t allocOffset;

    if(size == 0)
        return NULL;

    switch(mem)
    {
        case SHM:
            allocOffset = popPreAlloc(size);
            if(allocOffset == PRE_ALLOC_ERR)
                ptr = NULL;
            else
                ptr = (void*)((uint32_t)ptrBase + allocOffset);
            Print_DBG("SHM pre alloc, ptr : 0x%x, size : %d\r\n", ptr,size);
            break;
        default:
            Print_ERR("ERROR : Unknown mem type");
            break;
    }
    // printPreAllocActiveList();
    return ptr;
}

void preAllocFreeMem(void* bufAddr)
{
    Print_DBG("SHM pre alloc free ptr : 0x%x\r\n", bufAddr);
    releasePreAlloc(bufAddr);
    // printPreAllocActiveList();
}

static void resetPreAlloc(void)
{
    preAllocNextFreeIdx = 0;
    preAllocFreeCount = 0;
}

static int32_t popPreAlloc(uint32_t size)
{
    Print_DBG("prealloc idx : %d\n\r",preAllocNextFreeIdx);
    if(preAllocNextFreeIdx == PRE_ALLOC_MAX_ALLOCS_PER_INFER)
    {
        Print_DBG("Rollover happened\n\r");
        if(preAllocFreeCount == PRE_ALLOC_MAX_ALLOCS_PER_INFER)
        {
            resetPreAlloc();
        }
        else
        {
            Print_ERR("ERROR : Free count not mathcing with alloc count, possible memory leak\n\r");
            ERR_PRE_ALLOC();
            return PRE_ALLOC_ERR;
        }
    }

    size = (size + 7) & ~0x07;

    if(preallocArray[preAllocNextFreeIdx][0] == size)
    {
        return preallocArray[preAllocNextFreeIdx++][1];
    }
    else
    {
        Print_ERR("ERROR : prealloc size mismatch\n\r");
        return PRE_ALLOC_ERR;
    }
}

static void releasePreAlloc(void* ptr)
{
    void *ptrBase = &shmemM3Dsp.preAllocArray;
    uint32_t offset = (uint32_t)ptr - (uint32_t)ptrBase;

    if(preAllocFreeCount < preAllocNextFreeIdx)
    {
        preAllocFreeCount++;
    }
    else
    {
        ERR_PRE_ALLOC();
    }
}
