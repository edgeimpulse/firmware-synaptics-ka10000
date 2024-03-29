/*
 * Copyright (c) 2022 EdgeImpulse Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an "AS
 * IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _EI_CLASSIFIER_PORTING_H_
#define _EI_CLASSIFIER_PORTING_H_

#include <stdint.h>
#include <stdlib.h>
#include "edge-impulse-sdk/dsp/returntypes.h"

#if defined(__cplusplus) && EI_C_LINKAGE == 1
extern "C" {
#endif // defined(__cplusplus)

/**
 * Cancelable sleep, can be triggered with signal from other thread
 */
EI_IMPULSE_ERROR ei_sleep(int32_t time_ms);

/**
 * Check if the sampler thread was canceled, use this in conjunction with
 * the same signaling mechanism as ei_sleep
 */
EI_IMPULSE_ERROR ei_run_impulse_check_canceled();

/**
 * Read the millisecond timer
 */
uint64_t ei_read_timer_ms();

/**
 * Read the microsecond timer
 */
uint64_t ei_read_timer_us();

/**
 * Set Serial baudrate
 */
void ei_serial_set_baudrate(int baudrate);

/**
 * @brief      Connect to putchar of target
 *
 * @param[in]  c The chararater
 */
void ei_putchar(char c);

/**
 * @brief       Connect to getchar of target
 * @return      character from serial
*/
char ei_getchar(void);

/**
 * Print wrapper around printf()
 * This is used internally to print debug information.
 */
__attribute__ ((format (printf, 1, 2)))
void ei_printf(const char *format, ...);

/**
 * Override this function if your target cannot properly print floating points
 * If not overriden, this will be sent through `ei_printf()`.
 */
void ei_printf_float(float f);

/**
 * Wrapper around malloc
 */
void *ei_malloc(size_t size);

/**
 * Wrapper around calloc
 */
void *ei_calloc(size_t nitems, size_t size);

/**
 * Wrapper around free
 */
void ei_free(void *ptr);

#if defined(__cplusplus) && EI_C_LINKAGE == 1
}
#endif // defined(__cplusplus) && EI_C_LINKAGE == 1

// Load porting layer depending on target

// First check if any of the general frameworks or operating systems are supported/enabled
#ifndef EI_PORTING_ZEPHYR
#if defined(__ZEPHYR__)
#define EI_PORTING_ZEPHYR      1
#else
#define EI_PORTING_ZEPHYR      0
#endif
#endif

#ifndef EI_PORTING_ARDUINO
#ifdef ARDUINO
#define EI_PORTING_ARDUINO      1
#else
#define EI_PORTING_ARDUINO      0
#endif
#endif

#ifndef EI_PORTING_MBED
#ifdef __MBED__
#define EI_PORTING_MBED      1
#else
#define EI_PORTING_MBED      0
#endif
#endif

// Then check for target spcific build systems

#ifndef EI_PORTING_ESPRESSIF
#if ((defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S3)) && EI_PORTING_ZEPHYR == 0)
#include "esp_idf_version.h"
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif
#define EI_PORTING_ESPRESSIF      1
#define EI_PORTING_ARDUINO        0
#else
#define EI_PORTING_ESPRESSIF     0
#endif
#endif

#ifndef EI_PORTING_POSIX
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#define EI_PORTING_POSIX      1
#else
#define EI_PORTING_POSIX      0
#endif
#endif

#ifndef EI_PORTING_SILABS
#if defined(EFR32MG12P332F1024GL125)
#define EI_PORTING_SILABS      1
#else
#define EI_PORTING_SILABS      0
#endif
#endif

#ifndef EI_PORTING_RASPBERRY
#ifdef PICO_BOARD
#define EI_PORTING_RASPBERRY      1
#else
#define EI_PORTING_RASPBERRY      0
#endif
#endif


#ifndef EI_PORTING_STM32_CUBEAI
#if defined(USE_HAL_DRIVER) && !defined(__MBED__) && EI_PORTING_ZEPHYR == 0
#define EI_PORTING_STM32_CUBEAI      1
#else
#define EI_PORTING_STM32_CUBEAI      0
#endif
#endif

#ifndef EI_PORTING_HIMAX
#ifdef CPU_ARC
#define EI_PORTING_HIMAX        1
#else
#define EI_PORTING_HIMAX        0
#endif
#endif

#ifndef EI_PORTING_MINGW32
#ifdef __MINGW32__
#define EI_PORTING_MINGW32      1
#else
#define EI_PORTING_MINGW32      0
#endif
#endif
// End load porting layer depending on target

// Additional configuration for specific architecture
#if defined(__CORTEX_M)

#if (__CORTEX_M == 55U)
#define EI_MAX_OVERFLOW_BUFFER_COUNT	15
#endif

#if (__CORTEX_M == 85U)
#define EI_MAX_OVERFLOW_BUFFER_COUNT	50
#endif

#endif

#if defined(CONFIG_IDF_TARGET_ESP32S3)
#define EI_MAX_OVERFLOW_BUFFER_COUNT	30
#endif

// End additional configuration

#endif // _EI_CLASSIFIER_PORTING_H_
