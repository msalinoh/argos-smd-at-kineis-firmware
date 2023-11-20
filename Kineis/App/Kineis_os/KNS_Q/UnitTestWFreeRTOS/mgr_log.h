/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    mgr_log.h
 * @brief   manager the output log using the LPUART communication driver
 * @author  Ahmed JEDIDI
 * @date    creation 03/02/2020
 * @version 1.0
 * @note
 */

/**
 * @addtogroup MGR_LOG
 * @{
 */

#ifndef MGR_LOG_H
#define MGR_LOG_H

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdbool.h>
//#include "mgr_log_rtc.h"
//						MGR_LOG_RtcDateTime();\

#ifndef USE_LOCAL_PRINTF
#include <stdio.h>
#else
#include "mgr_log_conf.h"
#endif // end USE_LOCAL_PRINTF

/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#ifdef DEBUG

/* log with RTC timestamp before */
#define MGR_LOG_DEBUG(...)		do {\
						vMGR_LOG_printf(" ");\
						vMGR_LOG_printf(__VA_ARGS__);\
					} while (0)
/* direct logging */
#define MGR_LOG_DEBUG_RAW(...)		vMGR_LOG_printf(__VA_ARGS__)

#ifdef VERBOSE
/* log with RTC timestamp before */
#define MGR_LOG_VERBOSE(...)		do {\
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

/*******************************************************************************
 * EXTERNAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/
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
