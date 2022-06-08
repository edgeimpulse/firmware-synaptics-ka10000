#ifndef	_EXECUTOR_PRIV_H_
#define	_EXECUTOR_PRIV_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "executor_common.h"
#define BUF_STATE_NO_MEM			0
#define BUF_STATE_MEM_ACTIVE		1
#define BUF_STATE_MEM_IDLE			2

#define BUF_TYPE_NONE				0
#define BUF_TYPE_IN				  1
#define BUF_TYPE_OUT				2
#define BUF_TYPE_INOUT			3
#define BUF_TYPE_TRANSITION     4

#define WORK_ID_VALID				1
#define WORK_ID_INVALID				0

#define NORMAL_CONV_2D          0
#define DEPTHWISE_CONV_2D   1

struct privateInfo
{
	uint8_t bufState;
	uint8_t bufType;
	uint8_t usageCount;
	uint8_t tobeFreed;
	void *bufAddr;
};

// Represents already scheduled work on various Hw executors.
typedef struct ExecWork
{
  uint8_t execHwId;
  uint16_t kID;
  void *kPtr;
  void *prefun;
  void *postfun;
  uint8_t valid:1;
  uint8_t wait4Completion:1;
  uint8_t variant :2;
  uint8_t dbgTimeDump : 1;
  uint8_t reserved:4;
  ExecOperand_t *inbufs[CONFIG_EXEC_MAX_INPUTS] ;
  ExecOperand_t *scratchbufs[CONFIG_EXEC_MAX_SCRATCH];
  ExecOperand_t *outbufs[CONFIG_EXEC_MAX_OUTPUTS];
  ExecOperand_t *wts[CONFIG_EXEC_MAX_WEIGHTS];
  ExecOperand_t *bias[CONFIG_EXEC_MAX_BIAS];
  void *opt;
  uint8_t input_count;
  uint8_t scratch_count;
  uint8_t output_count;
  uint8_t weight_count;
  uint8_t bias_count;
  uint16_t dbgIdx;
  uint32_t cycle_start;
  uint32_t cycle_end;
// #ifdef CONFIG_ENABLE_EXEC_DBG
  uint16_t layerId;
// #endif
  void *args;
  uint8_t argc;
#ifdef CONFIG_BIN_SEM
  /* keep this as last member */
  SemaphoreHandle_t waitSem;
#endif
} ExecWork_t ;

uint8_t CheckAndScheduleWork (ExecWork_t * work, uint8_t inoutExist) ;
void SubmitM3Work (uint8_t execWorkID) ;
void SubmitDSPWork (uint8_t execWorkID) ;




#endif /*_EXECUTOR_PRIV_H_*/
