//////////////////////////////////////////////////////////////////////////
// The confidential and proprietary information contained in this file may
// only be used by a person authorized under and to the extent permitted
// by a subsisting licensing agreement from Eta Compute Inc.
//
//        (C) Copyright 2019 Eta Compute Inc
//                All Rights Reserved
//
// This entire notice must be reproduced on all copies of this file
// and copies of this file may only be made by a person if such person is
// permitted to do so under the terms of a subsisting license agreement
// from Eta Compute Inc.
//////////////////////////////////////////////////////////////////////////

#ifndef H_WORKQ_COMMON_
#define H_WORKQ_COMMON_
#include "config.h"
#include "ipc_drv.h"
#include "preAlloc.h"
//TBD: Needs to go to config.h
#define MAX_WORK_QUEUE_SIZE 2
/* reserved 32 for tM33DSPSharedMemory */

#define SHARED_MEMORY_ALLOCATOR_SIZE (CONFIG_SHM_LENGTH - 32)
#define NPU_WQ_INDEX 0
#define CAPE2_WQ_INDEX 1
#define DSP_WQ_COUNT 2

#define CPU_TO_WQ_INDEX(x) ((x) == IPC_TARGET_CAPEA ? 1: 0)

typedef struct workDescriptor_t {
  uint16_t  moduleId; //8bit moduleId   sensor, executor
  uint16_t  operation; //example sensor's open, ioctl, read etc.   FFT,CONVOLUTION
  uint32_t argumentPointerOffset; //memory offset where additional arguments for commands are located.
}tWorkDescriptor;

typedef struct workQueue_t {
    uint16_t readIndex;
    uint16_t writeIndex;
    tWorkDescriptor workArray[MAX_WORK_QUEUE_SIZE];
} tWorkQueue;

typedef struct sharedMemory_t {
    uint32_t   topMemoryBoundaryPattern;  //For debugging purpose, it will be filled with DEADBEAF
    tWorkQueue workQueue[DSP_WQ_COUNT];
    // uint32_t bottomMemoryBoundaryPattern;  //For debugging purpose, it will be filled with DEADBEAF
    uint8_t byteArray[SHARED_MEMORY_ALLOCATOR_SIZE];
    uint8_t preAllocArray[PRE_ALLOC_TOTAL_HEAP_MEMORY_SIZE];
} tSysSharedMemory;

int WorkQueueInit(tWorkQueue* queue);   //Shared queue between m3 and dsp will be initialized by M33
int WorkQueueAdd(tWorkQueue* queue, tWorkDescriptor* work);  //New workTask will be added by M33 into queue

#ifdef __GNUC__
int WorkQueueRemove(tWorkQueue * queue, tWorkDescriptor* work); //DSP will takeout task from queue and process.
#else
int WorkQueueRemove(volatile tWorkQueue * queue, tWorkDescriptor* work); //DSP will takeout task from queue and process.
#endif

uint8_t IsWorkQueueEmpty(volatile tWorkQueue * queue);


#endif//# H_WORKQ_COMMON_
