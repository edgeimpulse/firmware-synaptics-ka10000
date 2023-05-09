/**
 * \file os_timer.h
 * \name OS Timer APIs
 * \defgroup OS_TIMER_API System Timer API
 * \ingroup OS_ABSTRACTION_API
 * @{
 *
 *
 * os_timer.h
 * 
 * \brief System timer API
 * 
 * 
 *
 */
 /** @} */

#ifndef OS_TIMER_H_
#define OS_TIMER_H_

#include "timers.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TICKS_TO_MS(ticks)	    (ticks * portTICK_RATE_MS)
#define MS_TO_TICKS(ms)		    (ms / portTICK_RATE_MS)

#define os_TimerFunc            TimerCallbackFunction_t     // typedef void (*TimerCallbackFunction_t)( TimerHandle_t xTimer );
#define TIMER_MODE_ONE_SHOT     pdFALSE
#define TIMER_MODE_AUTO_RELOAD  pdTRUE

typedef TimerHandle_t           os_TimerHandle;

typedef struct os_TimerInit_t
{
	char*			TimerName;
    unsigned long   TimerPeriod;
    unsigned int    TimerMode;
    unsigned int    AutoStart;
	void *          UserParm;
    os_TimerFunc    TimerFunc;
} os_TimerInit;

/**
 * \fn unsigned long       os_GetTickCount(void)
 * \brief  Get the current system tick count
 * \return long: current system tick count
 */
unsigned long       os_GetTickCount(void);

/**********************************************************************************
 os_GetElapsedTime_ms()
 Get the elapsed time in miliseconds since the specificed previous system tick count
 Parameters:
	unsigned long: Previous system tick count
 Returns: unsigned long: elapsed time in miliseconds
**********************************************************************************/
/**
 * \fn unsigned long       os_GetElapsedTime_ms(unsigned long PreviousTickCount)
 * \brief  Get the elapsed time in miliseconds since the specificed previous system tick count
 * \param PreviousTickCount: Previous system tick count
 * \return unsigned long: elapsed time in miliseconds
 */
unsigned long       os_GetElapsedTime_ms(unsigned long PreviousTickCount);


/**
 * \fn int os_SysTimerSetup(unsigned long system_freq)
 * \brief  Update RTOS system timer resolution based on system frequency.
 * \param system_freq: Frequency which system timer is running at
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int os_SysTimerSetup(unsigned long system_freq);

/**
 * \fn void*               os_TimerGetUserParm(os_TimerHandle pTimerHandle)
 * \brief  Delete a previously created.
 * \param pTimerHandle: OS specific timer handle
 * \return void*: User Parm assigned to SW timer at create time
 */
void*               os_TimerGetUserParm(os_TimerHandle pTimerHandle);

/**
 * \fn os_TimerHandle      os_TimerCreate(os_TimerInit* pTimerInit, unsigned long timeout)
 * \brief  Create SW timer with callback when timer expires
 * \param pTimerInit: Pointer to OS specific SW timer initialization structure
 * \param timeout: Time in ms that calling task should be blocked waiting for timer creation to complete
 * \return os_TimerHandle: OS specific timer handle
 */
os_TimerHandle      os_TimerCreate(os_TimerInit* pTimerInit, unsigned long timeout);

/**
 * \fn int                 os_TimerDelete(os_TimerHandle pTimerHandle, unsigned long timeout)
 * \brief  Delete a previously created
 * \param pTimerHandle: OS specific timer handle
 * \param timeout: times in ms that calling task should be blocked waiting for timer creation to complete
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int                 os_TimerDelete(os_TimerHandle pTimerHandle, unsigned long timeout);

/**
 * \fn int                 os_TimerStart(os_TimerHandle pTimerHandle, unsigned long NewPeriod, unsigned long timeout)
 * \brief  Start a previously created SW timer. If timer has already been started then this call will reset timer.
 * \param pTimerHandle: OS specific timer handle
 * \param NewPeriod: new time period for previously created SW timer.
 * \param timeout: times in ms that calling task should be blocked waiting for timer start to complete
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int                 os_TimerStart(os_TimerHandle pTimerHandle, unsigned long NewPeriod, unsigned long timeout);


/**
 * \fn int                 os_TimerStop(os_TimerHandle pTimerHandle, unsigned long timeout)
 * \brief  Stop a previously started SW timer.
 * \param pTimerHandle: OS specific timer handle
 * \param timeout: times in ms that calling task should be blocked waiting for timer stop to complete
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int                 os_TimerStop(os_TimerHandle pTimerHandle, unsigned long timeout);

#ifdef __cplusplus
}
#endif

#endif // #ifndef OS_TIMER_H_