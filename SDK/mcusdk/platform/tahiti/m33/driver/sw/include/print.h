////////////////////////////////////////////////////////////////////////////////
//
//  SYNAPTICS INC. CONFIDENTIAL AND PROPRIETARY
//
//  The information contained in this source code file
//  is strictly confidential and proprietary to Synaptics, Inc.
//  ("Synaptics")
//
//  No part of this file may be possessed, reproduced or distributed, in
//  any form or by any means for any purpose, without the express written
//  permission of Synaptics Inc.
//
//  Except as otherwise specifically provided through an express agreement
//  with Synaptics that governs the confidentiality, possession, use
//  and distribution of the information contained in this file, SYNAPTICS
//  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
//  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
//  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
//  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
//  AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
//  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
//
//  Copyright (C) 2019 Synaptics Inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
/**
 * \file print.h
 * \brief APIs to print debug information to UART port
 * \defgroup Programming_API_LOG Debug Print
 * \ingroup COMMON_SYSTEM_API
 * @{
 *
 *
 * print.h
 * 
 * \section Introduction
 * Debug print API for different kinds of data types.
 * 5 levels of debug messages selectively enabled per source file.
 * 
 * \section Usage
 * \b Step 1: 
 * Define Macro PRINT_LEVEL_SET as the \b first line in \*.c file. \n
 * Valid options are:
 * <pre>
 ASSERT_DISABLE                 // value=0, use this for disabling all log
 ASSERT_LEVEL_ERR               // value=1, Enable ERR Prints Only 
 ASSERT_LEVEL_DBG               // value=2, Enable DGB and ERR Prints
 ASSERT_LEVEL_WARN              // value=3, Enable WARN, DBG and ERR Prints
 ASSERT_LEVEL_INFO              // value=4, Enable INFO, WARN, DGB and ERR Prints
 ASSERT_LEVEL_ALL               // value=5, Enable all prints, including enter and exit of functions
 * </pre>
 * 
 * \b Step 2: 
 * Call different log level macros to print log in your code.
 * 
 * <pre>
 Print_ERR                       // Will be printed when PRINT_LEVEL_SET >= ASSERT_LEVEL_ERR
 Print_DBG                       // Will be printed when PRINT_LEVEL_SET >= ASSERT_LEVEL_DBG
 Print_WARN                      // Will be printed when PRINT_LEVEL_SET >= ASSERT_LEVEL_WARN  
 Print_INFO                      // Will be printed when PRINT_LEVEL_SET >= ASSERT_LEVEL_INFO
 Print_ALL                       // Will be printed when PRINT_LEVEL_SET = ASSERT_LEVEL_ALL
 * </pre>
 *
 * 
 * @note 
 * 1.Calling these macros from ISR functions will impact overall system response time. \n
 * 2.Supported data formats are: 
 * <code>
 * \%c, \%s, \%d, \%u, \%x
 * </code>
 * 
 *
 * \section Example
 * 
 * @code
 // ---------------------------------------------------------------------------------------------
 //    Below Macro 'PRINT_LEVEL_SET', Should be first line in this .c file, before any includes. 
 //        Valid options are:      
 //            > ASSERT_DISABLE    
 //            > ASSERT_LEVEL_ERR  
 //            > ASSERT_LEVEL_WARN 
 //            > ASSERT_LEVEL_INFO 
 //            > ASSERT_LEVEL_ALL  
 #define PRINT_LEVEL_SET ASSERT_LEVEL_ERR
 #include "print.h"

 {
   ...
   Print_ERR("MCU main ready\n");
   ...
 } 
 * @endcode
 *
 */
 /** @} */ 
#ifndef _MCU_PRINT_H_
#define _MCU_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ASSERT_DISABLE          0    ///< Disable all prints
#define ASSERT_LEVEL_ERR        1    ///< Enable ERR Prints Only
#define ASSERT_LEVEL_DBG        2    ///< Enable DGB and ERR Prints
#define ASSERT_LEVEL_WARN       3    ///< Enable WARN, DBG and ERR Prints
#define ASSERT_LEVEL_INFO       4    ///< Enable INFO, WARN, DGB and ERR Prints
#define ASSERT_LEVEL_ALL        5    ///< Enable all prints, including enter and exit of functions

#if defined(CONFIG_MCU_DRV_PRINT) && ((!defined(__DOMAIN_NS) && defined(CONFIG_MCU_DRV_UART_SECURE)) || \
    (defined(__DOMAIN_NS) && defined(CONFIG_MCU_DRV_UART_NON_SECURE)))

    #define MCU_MAX_PRINT_QUEUE_ARGS        8U
    #define MCU_MAX_PRINT_CHAR_AT_A_TIME    8U //CONFIG_MCU_DRV_PRINT_NOF_CHAR

    #define MCU_MAX_PRINT_QUEUE_SIZE        CONFIG_MCU_DRV_PRINT_QUEUE_SIZE
    #undef MCU_MAX_PRINT_QUEUE_ARGS
    #define MCU_MAX_PRINT_QUEUE_ARGS        CONFIG_MCU_DRV_PRINT_QUEUE_ARGS

    #ifdef CONFIG_MCU_PRINT_S_2_NS
        #if (defined(CONFIG_MCU_DRV_PRINT_TASK) && defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)) || !defined(__DOMAIN_NS)
            #define MCU_MAX_PRINT_BUF_SIZE     (1024U)
            typedef struct mcu_print_s_2_ns_s {
                volatile short  rd_idx;
                volatile short  wr_idx;
                char            buf[MCU_MAX_PRINT_BUF_SIZE];
            } mcu_print_s_2_ns_t;
        #endif // CONFIG_MCU_DRV_PRINT_TASK
    #endif // CONFIG_MCU_PRINT_S_2_NS

    #if defined(CONFIG_MCU_PRINT_S_2_NS) && defined(__DOMAIN_NS) && defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(CONFIG_MCU_DRV_PRINT_TASK)
        mcu_print_s_2_ns_t * mcu_print_get_s_2_ns_buf_ns(void);
    #endif // __DOMAIN_NS && CONFIG_MCU_NON_SECURE_MODE_RTOS && CONFIG_MCU_DRV_PRINT_TASK

    void print_enable(int en);
    int mcu_print(char *fmt,...);
    int mcu_print_queue(char *fmt,...);
    #define PRINTF              mcu_print
    #define PRINTF_QUEUE        mcu_print_queue

    #define assert_WriteLine(_Str_...)              do{  PRINTF( _Str_ );}while(0)
    #define assert_WriteLine_Queue(_Str_...)        do{  PRINTF_QUEUE( _Str_ );  }while(0)

    #if (PRINT_LEVEL_SET==ASSERT_LEVEL_ALL)
        #define Print_ERR(X...)  assert_WriteLine(X)
        #define Print_DBG(X...)  assert_WriteLine(X)
        #if (defined(CONFIG_MCU_DRV_PRINT_TASK) && defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)) || !defined(__DOMAIN_NS)
            #define Print_WARN(X...) assert_WriteLine_Queue(X)
            #define Print_INFO(X...) assert_WriteLine_Queue(X)
            #define Print_ALL(X...)  assert_WriteLine_Queue(X)
        #else
            #define Print_WARN(X...) assert_WriteLine(X)
            #define Print_INFO(X...) assert_WriteLine(X)
            #define Print_ALL(X...)  assert_WriteLine(X)
        #endif
    #elif (PRINT_LEVEL_SET ==  ASSERT_LEVEL_INFO)
        #define Print_ERR(X...)  assert_WriteLine(X)
        #define Print_DBG(X...)  assert_WriteLine(X)
        #if (defined(CONFIG_MCU_DRV_PRINT_TASK) && defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)) || !defined(__DOMAIN_NS)
            #define Print_WARN(X...) assert_WriteLine_Queue(X)
            #define Print_INFO(X...) assert_WriteLine_Queue(X)
        #else
            #define Print_WARN(X...) assert_WriteLine(X)
            #define Print_INFO(X...) assert_WriteLine(X)
        #endif
        #define Print_ALL(X...)  do{ }while(0)
    #elif (PRINT_LEVEL_SET ==  ASSERT_LEVEL_WARN)
        #define Print_ERR(X...)  assert_WriteLine(X)
        #define Print_DBG(X...)  assert_WriteLine(X)
        #if (defined(CONFIG_MCU_DRV_PRINT_TASK) && defined(CONFIG_MCU_NON_SECURE_MODE_RTOS) && defined(__DOMAIN_NS)) || !defined(__DOMAIN_NS)
            #define Print_WARN(X...) assert_WriteLine_Queue(X)
        #else
            #define Print_WARN(X...) assert_WriteLine(X)
        #endif
        #define Print_INFO(X...) do{ }while(0)
        #define Print_ALL(X...)  do{ }while(0)
    #elif (PRINT_LEVEL_SET ==  ASSERT_LEVEL_DBG)
        #define Print_ERR(X...)  assert_WriteLine(X)
        #define Print_DBG(X...)  assert_WriteLine(X)
        #define Print_WARN(X...) do{ }while(0)
        #define Print_INFO(X...) do{ }while(0)
        #define Print_ALL(X...)  do{ }while(0)
    #elif (PRINT_LEVEL_SET ==  ASSERT_LEVEL_ERR)
        #define Print_ERR(X...)  assert_WriteLine(X)
        #define Print_DBG(X...)  do{ }while(0)
        #define Print_WARN(X...) do{ }while(0)
        #define Print_INFO(X...) do{ }while(0)
        #define Print_ALL(X...)  do{ }while(0)
    #else
        #define Print_ERR(X...)  do{ }while(0)
        #define Print_DBG(X...)  do{ }while(0)
        #define Print_WARN(X...) do{ }while(0)
        #define Print_INFO(X...) do{ }while(0)
        #define Print_ALL(X...)  do{ }while(0)
    #endif

    #define FUNC_ENTER() Print_ALL("Func_Enter(): %s\r\n",__FUNCTION__)
    #define FUNC_EXIT()  Print_ALL("Exit():  %s\r\n",__FUNCTION__)

    extern void MCU_Print_Init(void);
    extern void MCU_Print_Newline(void);

#else

    #define assert_WriteLine(_Level_,_Str_...); { }
    #define Print_ERR(X...)  do{ }while(0)
    #define Print_DBG(X...)  do{ }while(0)
    #define Print_WARN(X...) do{ }while(0)
    #define Print_INFO(X...) do{ }while(0)
    #define Print_ALL(X...)  do{ }while(0)
    #define FUNC_ENTER()
    #define FUNC_EXIT()

#endif // #ifdef CONFIG_MCU_DRV_PRINT

#ifdef __cplusplus
}
#endif

#endif /* _MCU_PRINT_H_ */
