/**
 * \file os_task.h
 * \name OS Task APIs
 * \defgroup OS_TASK_API System Task API
 * \ingroup OS_ABSTRACTION_API
 * @{
 *
 *
 * os_task.h
 * 
 * \brief System task management APIS (creation, deletion, notifications, etc...)
 * 
 * 
 *
 */
 /** @} */

#ifndef OS_TASK_H_
#define OS_TASK_H_

#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	OS_TASK_LOWEST_PRIORITY		0
#define	OS_TASK_LOW_PRIORITY		(OS_TASK_LOWEST_PRIORITY+1)
#define OS_TASK_MID_PRIORITY		(configMAX_PRIORITIES/2)
#define OS_TASK_HIGH_PRIORITY		(configMAX_PRIORITIES-2)
#define	OS_TASK_HIGHEST_PRIORITY	(configMAX_PRIORITIES-1)

#define os_TaskFunc			TaskFunction_t		// typedef void (*TaskFunction_t)( void * );
#define os_Priority			UBaseType_t			// typedef unsigned long UBaseType_t;
#define os_TaskHookFunc		TaskHookFunction_t	// BaseType_t (*TaskHookFunction_t)( void * )

typedef struct os_TaskStruct_t
{
	unsigned long	ContextID;
	TaskHandle_t	xTaskHandle;
	os_SemHandle	TaskSem;
	os_QHandle		TaskQueue;
	char          *TaskName;
#if defined(CONFIG_MCU_WAIT_TASK_RUNNING)
  int           running_state;
#endif
} os_TaskStruct;

typedef os_TaskStruct*	os_TaskHandle;

typedef struct os_TaskInit_t
{
	unsigned long	ContextID;
  os_TaskFunc		TaskFunc;
	os_TaskHookFunc	TaskHookFunc;
	char*			TaskName;
  unsigned short	TaskStackSize;
  os_Priority		TaskPriority;
#if (__DOMAIN_NS == 1U)
  unsigned int  TaskTZModule;
#endif // __DOMAIN_NS
} os_TaskInit;

typedef struct os_TaskHookMsg_t
{
	int msg;
	void* parm;
} os_TaskHookMsg;

/**
 * \fn os_TaskHandle	os_TaskGetCurrentHandle(void)
 * \brief Return the currently active task handle
 * \return Returns: os_TaskHandle: OS specific task handle
 */
os_TaskHandle	os_TaskGetCurrentHandle(void);

/**
 * \fn unsigned long	os_TaskGetCurrentID(void)
 * \brief  Return the currently active task ID (ID is different than handle)
 * \return Returns: unsigned long Task ID
 */
unsigned long	os_TaskGetCurrentID(void);

/**
 * \fn os_TaskHandle   os_FindTaskFromID(unsigned long id)
 * \brief  Return the corresponding task handle of the requested task ID (ID is different than handle)
 * \param id : Task ID
 * \return os_TaskHandle: OS specific task handle
 */
os_TaskHandle   os_FindTaskFromID(unsigned long id);

/**********************************************************************************
 os_TaskCreate()
 Create, initilize, and start a user specified task
 Parameters:
   os_TaskInit *pTaskInit : Pointer to OS specific task initialization structure
 Returns: os_TaskHandle: OS specific task handle
**********************************************************************************/
/**
 * \fn os_TaskHandle	os_TaskCreate(os_TaskInit* pTaskInit)
 * \brief  Create, initilize, and start a user specified task
 * \param pTaskInit : Pointer to OS specific task initialization structure
 * \return os_TaskHandle: OS specific task handle
 */
os_TaskHandle	os_TaskCreate(os_TaskInit* pTaskInit);

/**********************************************************************************
 os_TaskDelete()
 Delete the specified task.
 Parameters:
   os_TaskHandle TaskHandle : OS specific task handle
 Returns: int (0 or -1)
**********************************************************************************/
/**
 * \fn int				os_TaskDelete(os_TaskHandle Task)
 * \brief  Delete the specified task.
 * \param  TaskHandle : OS specific task handle
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_TaskDelete(os_TaskHandle Task);

/**
 * \fn int				os_TaskWaitEvent(unsigned long *pNotificationValue)
 * \brief  Task is blocked indefinitely waiting for notify value to return
 * \param  pNotificationValue : pointer to notify variable to be updated
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_TaskWaitEvent(unsigned long *pNotificationValue);

/**
 * \fn int os_TaskWaitEventTimeOut(unsigned long *pNotificationValue, unsigned long timeout_ms)
 * \brief  Task is blocked waiting for notify value to return or specified timeout to occur
 * \param  pNotificationValue : pointer to notify variable to be updated
 * \param  timeout_ms : time in ms waiting for notify value to occurr before timing out
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int os_TaskWaitEventTimeOut(unsigned long *pNotificationValue, unsigned long timeout_ms);

/**
 * \fn int				os_TaskNotifyEvent(os_TaskHandle TasktoNotify, unsigned long Event)
 * \brief  Notify blocked task of change in value/event
 * \param  TasktoNotify : OS specific task handle
 * \param  Event:         Notify value
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_TaskNotifyEvent(os_TaskHandle TasktoNotify, unsigned long Event);

/**
 * \fn int				os_TaskNotify(os_TaskHandle TasktoNotify)
 * \brief  Notify blocked task to wake up
 * \param  TasktoNotify : OS specific task handle
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_TaskNotify(os_TaskHandle TasktoNotify);

/**
 * \fn void			os_TaskYield (void)
 * \brief  Release control of current task
 */
void			os_TaskYield (void);

/**
 * \fn void			os_TaskSleep(unsigned long NumofTicks)
 * \brief  Suspend the calling task for the specified number of timer ticks.
 * \param  NumberTicks : Number of timer ticks
 */
void			os_TaskSleep(unsigned long NumofTicks);

/**
 * \fn void			os_TaskSleepms(unsigned long TimeInms)
 * \brief  Suspend the calling task for the specified number of miliseconds.
 * \param  NumberTicks : Number of miliseconds
 */
void			os_TaskSleepms(unsigned long TimeInms);

/**
 * \fn int				os_TaskSuspend(os_TaskHandle TaskToSuspend)
 * \brief  Suspend specified task. Another task will not to call os_TaskResume
 * \param  TaskToSuspend : task to be suspended
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_TaskSuspend(os_TaskHandle TaskToSuspend);

/**
 * \fn int				os_TaskResume(os_TaskHandle TaskToResume)
 * \brief  Resume suspended task
 * \param  TaskToResume: task to be resumed
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_TaskResume(os_TaskHandle TaskToResume);

/**
 * \fn long 			os_TaskHook(os_TaskHandle TaskToCall, os_TaskHookMsg* pEventMsg)
 * \brief  Execute the hook function associated of the requested task
 * \param  TaskToCall  : Task Handle
 * \param  pEventMsg : Pointer to Event Message to pass to task hook function
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
long 			os_TaskHook(os_TaskHandle TaskToCall, os_TaskHookMsg* pEventMsg);


#ifdef CONFIG_MCU_MEM_USAGE
int os_GetMemInfo(int command, int *data);
#else
/**
 * \fn void os_TaskMemUsage(void)
 * \brief  [DEBUG API] Print memory usage related to each task stack, system heap, and OS stack to UART
 */
void os_TaskMemUsage(void);
#endif
/*************************************************
	This section is only legacy porting from CAPE
 ************************************************/

#if CONFIG_MCU_HAVE_TASK_CPUTIME
unsigned long os_GetIdleTick(void);
unsigned long os_GetIRQTick(void);
unsigned long os_GetIRQPreemptIdleTick(void);
void os_TickCtrl(int enable);
void StartIdleMonitor(void);
void EndIdleMonitor(void);
void StartIRQMonitor();
void EndIRQMonitor();
#endif //CONFIG_MCU_HAVE_TASK_CPUTIME


/* ---------------------------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif // #ifndef OS_TASK_H_