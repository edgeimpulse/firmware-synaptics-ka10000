/**
 * \file os_sem.h
 * \name OS Semiphore APIs
 * \defgroup OS_QUEUE_API  System Semiphore API
 * \ingroup OS_ABSTRACTION_API
 * @{
 *
 *
 * os_queue.h
 * 
 * \brief System semiphore API
 * 
 * 
 *
 */
 /** @} */

#ifndef OS_SEM_H_
#define OS_SEM_H_

#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SEM_BINARY,
    SEM_COUNTING,
    SEM_MUTEX        // Mutual exclusion
} eSemType;

typedef struct os_SemStruct_t
{
	unsigned long	  uSemType;
	unsigned long	  uInitialCount;
	unsigned long	  uMaxCount;
	SemaphoreHandle_t xSemaphore;
} os_SemStruct;

typedef os_SemStruct*	os_SemHandle;

typedef struct os_SemInit_t
{
	unsigned long uSemType;
	unsigned long uInitialCount;
	unsigned long uMaxCount;
} os_SemInit;

#define OS_SEM_WAIT_FOREVER		portMAX_DELAY 
#define OS_SEM_NO_WAIT			0

/**********************************************************************************
 os_SemCreate()
 Create binary or counting semaphore or mutex
 Parameters:
    os_SemInit* pSemInit
 Returns: os_SemHandle: OS specific semaphore handle
**********************************************************************************/
/**
 * \fn os_SemHandle	os_SemCreate(os_SemInit* pSemInit)
 * \brief Create binary or counting semaphore or mutex
 * \param pSemInit: pointer to semiphore object
 * \return os_SemHandle: OS specific semaphore handle
 */
os_SemHandle	os_SemCreate(os_SemInit* pSemInit);

/**
 * \fn int				os_SemDelete(os_SemHandle SemHandle)
 * \brief Delete previously created semaphore or mutex
 * \param os_SemHandle SemHandle: OS specific semaphore handle
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_SemDelete(os_SemHandle SemHandle);

/**
 * \fn int				os_SemTake(os_SemHandle SemHandle, unsigned long TimeOutms)
 * \brief Obtain an instance of the specified semaphore or mutex
 * \param SemHandle   : OS specific semaphore handle
 * \param TimeOutms  : Suspend time of calling task to wait for available semaphore
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_SemTake(os_SemHandle SemHandle, unsigned long TimeOutms);

/**
 * \fn int				os_SemGive(os_SemHandle SemHandle)
 * \brief Release an instance of the specified semaphore.
 * \param SemHandle   : OS specific semaphore handle
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int				os_SemGive(os_SemHandle SemHandle);

#ifdef __cplusplus
}
#endif

#endif // #ifndef OS_SEM_H_