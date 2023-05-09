/**
 * \file os_queue.h
 * \name OS Queue APIs
 * \defgroup OS_QUEUE_API  System Queue API
 * \ingroup OS_ABSTRACTION_API
 * @{
 *
 *
 * os_queue.h
 * 
 * \brief System Queue API
 * 
 * 
 *
 */
 /** @} */
 
#ifndef OS_QUEUE_H_
#define OS_QUEUE_H_

#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    Q_TO_BACK,
    Q_TO_FRONT,
	Q_OVERWRITE,
} eQMethod;

typedef QueueHandle_t				os_QHandle;

#define OS_QUEUE_WAIT_FOREVER		portMAX_DELAY 
#define OS_QUEUE_NO_WAIT			0


/**
 * \fn os_QueueCreate(unsigned long QLength, unsigned long QItemSize)
 * \brief Create queue
 * \param QLength Maximum numer of items queue can contain
 * \param QItemSize Size of each item in the queue in bytes
 * \return QHandle:	OS specific queue handle
 */
os_QHandle	os_QueueCreate(unsigned long QLength, unsigned long QItemSize);

/**
 * \fn os_QueueDelete(os_QHandle QHandle)
 * \brief Delete queue
 * \param os_QHandle OS specific queue handle
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int			os_QueueDelete(os_QHandle QHandle);

/**
 * \fn int			os_QueuePostMsg(os_QHandle QHandle, unsigned long* pQItem, eQMethod Option, unsigned long TimeOutms)
 * \brief Post an item to the queue
 * \param QHandle OS specific queue handle
 * \param pQItem:		Pointer to item to be queued
 * \param eQMethod Option:			Post to the front, back, or overwrite
 * \param unsigned long TimeOutms:	Time to wait for space to become available in queue
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int			os_QueuePostMsg(os_QHandle QHandle, unsigned long* pQItem, eQMethod Option, unsigned long TimeOutms);

/**
 * \fn int			os_QueueWaitMsg(os_QHandle QHandle, unsigned long *pMsgBuffer, unsigned long TimeOutms)
 * \brief Check for incoming message posted to queue
 * \param QHandle OS specific queue handle
 * \param 	unsigned long *pMsgBuffer:	Pointer to buffer into which received item will be copied
 * \param 	unsigned long TimeOutms:	Amount of time should block waiting for an item to be received.
 * \return int (0 for SUCCESS or -1 for FAILURE)
 */
int			os_QueueWaitMsg(os_QHandle QHandle, unsigned long *pMsgBuffer, unsigned long TimeOutms);

/**
 * \fn int			os_QueueMsgsWaiting(os_QHandle QHandle)
 * \brief Check the number of messages available in the queue.
 * \param QHandle OS specific queue handle
 * \return int (>= 0 for SUCCESS or -1 for FAILURE)
 */
int			os_QueueMsgsWaiting(os_QHandle QHandle);

#ifdef __cplusplus
}
#endif

#endif // #ifndef OS_QUEUE_H_