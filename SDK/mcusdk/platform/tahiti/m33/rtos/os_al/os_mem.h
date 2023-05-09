/**
 * \file os_mem.h
 * \name OS Memory APIs
 * \defgroup OS_MEM_API System Memory API
 * \ingroup OS_ABSTRACTION_API
 * @{
 *
 *
 * os_mem.h
 * 
 * \brief System memory API
 * 
 * 
 *
 */
 /** @} */

/**
* \file os_mem.h
* \brief Those APIs can be used for malloc and free memory.
*/
#ifndef OS_MEM_H_
#define OS_MEM_H_

#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \fn os_MallocAlign(unsigned long MemSize, unsigned long Alignment)
 * \brief Allocate a block of memory based on alignment specified by user.
 * \param MemSize Size of memory block in bytes.
 * \param Alignment Alignment of memory block.
 * \return void *: Pointer to allocated memory block or NULL if failed.
 */
void*	os_MallocAlign(unsigned long MemSize, unsigned long Alignment);

/**
 * \fn os_Malloc(unsigned long MemSize)
 * \brief Allocate a block of memory.
 * \param MemSize Size of memory block in bytes.
 * \return void *: Pointer to allocated memory block or NULL if failed.
 */
void*	os_Malloc(unsigned long MemSize);

/**
 * \fn os_FreeAlign(void *pMemStart)
 * \brief Free a block of memory with specific alignment.
 * \param *pMemStart Pointer to the start of allocated memory block.
 * \return status int representing the status of the function.\n
 *               = 0 if successful\n
 *               =-1 if error
 */
int		os_FreeAlign(void *pMemStart);

/**
 * \fn os_Free(void *pMemStart)
 * \brief Free a block of memory back to the pool specified by the caller.
 * \param *pMemStart Pointer to the start of allocated memory block.
 * \return status int representing the status of the function.\n
 *               = 0 if successful\n
 *               =-1 if error
 */
int		os_Free(void *pMemStart);

/************************************************* 
	This section is only legacy porting from CAPE
 ************************************************/

/* ---------------------------------------------------------------------------------------------------------------- */
/* Below APIs are used only for porting CAPE code to Anacapa. Later all the sos_* MACROS should be changed to os_* */
/* Therefore, all new code should 'NOT' use sos_* MACROS, but instead use os_* MACROS/Functions */
/* ---------------------------------------------------------------------------------------------------------------- */
#define	sos_xalloc	os_MallocAlign
#define	sos_xfree	os_FreeAlign
/* ---------------------------------------------------------------------------------------------------------------- */

#define	MEM_32A			0
#define	MEM_ALIGN_16	32 // aligned on 16 words which is 32 bytes for M0

#ifdef __cplusplus
}
#endif

#endif // #ifndef OS_MEM_H_