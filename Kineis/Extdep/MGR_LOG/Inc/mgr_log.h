/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_log.h
 * @author Kinéis
 * @brief logger main header file
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Kineis. All rights reserved.</center></h2>
 *
 * This software component is licensed by Kineis under Ultimate Liberty license, the "License";
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 * * kineis.com
 *
 */

/**
 * @page mgr_log_page LOGGER
 *
 * This page is presenting the logging strategy for the kineis stack (\ref MGR_LOG).
 *
 * so far, two levels of logging are available: DEBUG, VERBOSE. The logging routines are defined
 * through static macros.
 *
 * Depending on the enabled compilation flags, the source code of the kineis stack will invoke
 * empty macros (DEBUG=0, VERBOSE=0) or calls to a routine (\ref vMGR_LOG_printf if DEBUG=1 or
 * VERBOSE=1). This strategy allow to limit the footprint an dimpact on real-time at maximum
 * when logging is disabled.
 *
 * Then, a third static compilation is impacting \ref vMGR_LOG_printf routine:
 * * if USE_LOCAL_PRINTF is defined, vMGR_LOG_printf is a real function coded in \ref mgr_log.c
 * * if USE_LOCAL_PRINTF is NOT  defined, vMGR_LOG_printf is actually replace by legacy printf
 * function.
 *
 * By defult, in most of the packages delivered by Kineis, this Log Manager is actually outpûting
 * the log to a dedicated console (via UART).
 *
 */

/**
 * @addtogroup MGR_LOG
 * @{
 */

#ifndef MGR_LOG_H
#define MGR_LOG_H

/* Includes ------------------------------------------------------------------*/

#ifdef DEBUG
#include "mgr_log_rtc.h"
#endif

/* Defines -------------------------------------------------------------------*/

#ifdef DEBUG

/* log with RTC timestamp before */
#define MGR_LOG_DEBUG(...)		do {\
						MGR_LOG_RtcDateTime();\
						vMGR_LOG_printf(" ");\
						vMGR_LOG_printf(__VA_ARGS__);\
					} while (0)
/* direct logging */
#define MGR_LOG_DEBUG_RAW(...)		vMGR_LOG_printf(__VA_ARGS__)

#ifdef VERBOSE
/* log with RTC timestamp before */
#define MGR_LOG_VERBOSE(...)		do {\
						MGR_LOG_RtcDateTime();\
						vMGR_LOG_printf(" ");\
						vMGR_LOG_printf(__VA_ARGS__);\
					} while (0)
/* direct logging without timestamp */
#define MGR_LOG_VERBOSE_RAW(...)	vMGR_LOG_printf(__VA_ARGS__)
#else  // else VERBOSE
#define MGR_LOG_VERBOSE(...)		do {} while (0)
#define MGR_LOG_VERBOSE_RAW(...)	do {} while (0)
#endif // end VERBOSE


#else // not DEBUG

#define MGR_LOG_DEBUG(...)		do {} while (0)
#define MGR_LOG_DEBUG_RAW(...)		do {} while (0)
#define MGR_LOG_VERBOSE(...)		do {} while (0)
#define MGR_LOG_VERBOSE_RAW(...)	do {} while (0)

#endif // end not DEBUG

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Write formatted string into a buffer with variable number of
 * parameters with a specified maximum number of chars to write.
 *
 * @param[in] format: format of the string (same syntax as printf)
 *
 * @return none
 */

#ifdef USE_LOCAL_PRINTF
void vMGR_LOG_printf(const char *format, ...);

#else // not USE_LOCAL_PRINTF

#define vMGR_LOG_printf			printf

#endif // end USE_LOCAL_PRINTF

#endif // end MGR_LOG_H

/**
 * @}
 */
