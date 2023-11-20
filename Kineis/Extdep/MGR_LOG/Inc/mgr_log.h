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
 * @addtogroup MGR_LOG
 * @{
 */

#ifndef MGR_LOG_H
#define MGR_LOG_H

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include "mgr_log_conf.h"

#define VERBOSE 1

#ifndef USE_LOCAL_PRINTF
#include <stdio.h>
#endif // end USE_LOCAL_PRINTF

/* Defines -------------------------------------------------------------------*/

#ifdef DEBUG

/* log with RTC timestamp before */
#define MGR_LOG_DEBUG(...)		do {\
						/* E.G. Add RTC logging here */\
						vMGR_LOG_printf(__VA_ARGS__);\
					} while (0)
/* direct logging */
#define MGR_LOG_DEBUG_RAW(...)		vMGR_LOG_printf(__VA_ARGS__)

#ifdef VERBOSE
/* log with RTC timestamp before */
#define MGR_LOG_VERBOSE(...)		do {\
						/* E.G. Add RTC logging here */\
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
