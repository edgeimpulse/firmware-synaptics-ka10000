#ifndef	_EXECUTOR_PUBLIC_H_
#define	_EXECUTOR_PUBLIC_H_
#include <stdint.h>
#include <stdbool.h>
#include "executor_types.h"
#include "executor_op.h"

#define ALIGN_TO(X)  __attribute__((aligned (X)))

#define NAME(X)      #X

#define EXEC_DBG_PRINT_NONE   0
#define EXEC_DBG_PRINT_NORMAL 1
#define EXEC_DBG_PRINT_CSV    2

#define EXEC_DUMP_TO_SERIAL 0
// #define EXEC_DUMP_TO_FILE   1  // TBD
#ifdef CONFIG_ENABLE_EXEC_DBG
#define EXEC_BUFFER_DUMP(DUMP_MODE,X)  {ExecBufferDump(DUMP_MODE,NAME(X),&X);}
#define EXEC_DBG_INIT()             {ExecBufferInit();}
#define EXEC_DBG_FLUSH(X)            {ExecBufferFlush((X));}
#else
#define EXEC_BUFFER_DUMP(DUMP_MODE,X)
#define EXEC_DBG_INIT()
#define EXEC_DBG_FLUSH(X)
#endif

#define EXEC_MAP_ROM(X) ExecAllocMem(&X,&incbin_ ## X ## _start,0) // do this for all weights and biases'
#define EXEC_UNMAP_ROM(X) ExecFreeMem(&X)
#define EXEC_ALLOC_MEM(X) ExecAllocMem(&X, 0, 0 )
#define EXEC_MAP_TO_IOMEM(X,IOMEM)  do { X.origin= OPD_ORIG_IO;  ExecAllocMem(&X, IOMEM, 0 ); } while(0);
#define EXEC_REMAP_TO_IOMEM(X,IOMEM)  do { X.origin= OPD_ORIG_IO;  ExecReMapMem(&X, IOMEM, 0 ); } while(0);
#define EXEC_FREE_MEM(X)  ExecFreeMem(&X)
#define ARRAY_PTR(X)  (int8_t*)&incbin_ ## X ## _start
#define GET_BUFF_ADDR(X)  ExecGetBufAddr(&X)
#define NUM_ELEMENTS(X) (X.numElements)
#define NUM_BYTES(X)  ( (X.numElements) * (X.basetypeSize +1))

#define DECL_BUF_M33_SCRATCH( X,N)  ExecOperand_t X = {.origin= OPD_ORIG_CPU, .accessType = OPD_ACCESS_TYPE_RW,\
                                        .memType=OPD_MEM_TYPE_M33_LOCAL,.basetypeSize=OPD_BASE_SIZE_BYTES_1,\
                                        .memScope=OPD_MEM_SCOPE_OPERATION,.numElements=N};
#define DECL_BUF_M33_PERSISTENT( X,N) ExecOperand_t X = {.origin= OPD_ORIG_CPU, .accessType = OPD_ACCESS_TYPE_RW,\
                                      .memType=OPD_MEM_TYPE_M33_LOCAL,.basetypeSize=OPD_BASE_SIZE_BYTES_1,\
                                      .memScope=OPD_MEM_SCOPE_GLOBAL,.numElements=N};
#define DECL_CONST_INT_ROM( X,N)  ExecOperand_t X = {.origin= OPD_ORIG_INT_PERSISTENT_MEM, .accessType = OPD_ACCESS_TYPE_RO,\
                                      .memType=OPD_MEM_TYPE_M33_LOCAL,.basetypeSize=OPD_BASE_SIZE_BYTES_1,\
                                      .memScope=OPD_MEM_SCOPE_GLOBAL,.numElements=N};INCBIN_ETA(X);

#define DECL_BUF_M33_SHM( X,N) ExecOperand_t X = {.origin= OPD_ORIG_CPU, .accessType = OPD_ACCESS_TYPE_RW,\
                                .memType=OPD_MEM_TYPE_SHARED,.basetypeSize=OPD_BASE_SIZE_BYTES_1,\
                                .memScope=OPD_MEM_SCOPE_GLOBAL,.numElements=N};

#define EXEC_HW_ID_M33   0
#define EXEC_HW_ID_NPU  1
#define EXEC_HW_ID_CAPE2 2
#define NO_INPUT_ALLOC
#define NO_OUTPUT_ALLOC
#define WAIT_4_COMPLETION() ExecWaitForCompletion()

ExecStatus ExecInit (void);

typedef struct conv2d_relu_avgpool_opt_st
{
  uint16_t in_rows;     // input tensor dimension height
  uint16_t in_cols;     // input tensor dimension width
  uint16_t in_depth;    // number of input tensor channels
  uint16_t num_filt;    // number of filters
  uint16_t bias_lshift; // amount of left-shift for bias
  uint16_t out_rshift;  // amount of right-shift for output
  uint16_t act_min;     // the minimum value of the output range
  uint16_t act_max;     // the maximum value of the output range
} conv2d_relu_avgpool_opt;

typedef void (*kopf)(void *ptr, ...);

typedef struct
{
  uint8_t input_count;
  uint8_t scratch_count;
  uint8_t output_count;
  uint8_t weight_count;
  uint8_t bias_count;
  bool hasopt;
  // bool dbgTimeDump;
// #ifdef CONFIG_ENABLE_EXEC_DBG
  uint16_t layerId;
// #endif
  void *prefun;
  void *postfun;
} KDesc_t;

ExecStatus Execfunc(uint8_t execHwId, uint16_t  opId, void *opPtr, KDesc_t *desc, int pcnt, ...);

#define M33_FUNC(kernel) (kernel)
#define CAPE2_FUNC(kernel) ((void *)(CAPE2_##kernel))
#define NPU_FUNC(kernel) ((void *)(NPU_##kernel))
#define LLE_FUNC(kernel) ((void *)(LLE_##kernel))

#define EXEC_M33_FUNC(kernel, desc, ...) \
  Execfunc(EXEC_HW_ID_M33, M33_##kernel, kernel, desc, NARGS(__VA_ARGS__), __VA_ARGS__)

#define EXEC_NPU_FUNC(kernel, desc, ...) \
  Execfunc(EXEC_HW_ID_NPU, NPU_##kernel,NULL, desc, NARGS(__VA_ARGS__), __VA_ARGS__)

#define EXEC_CAPE2_FUNC(kernel, desc, ...) \
  Execfunc(EXEC_HW_ID_CAPE2, CAPE2_##kernel, NULL, desc, NARGS(__VA_ARGS__), __VA_ARGS__)

#endif /*_EXECUTOR_PUBLIC_H_*/
