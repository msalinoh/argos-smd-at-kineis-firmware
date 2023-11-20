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
#include "kineis_sw_conf.h"
#ifdef USE_LOCAL_PRINTF
#include STM32_HAL_H
#else
#include <stdio.h>
#endif
#ifdef DEBUG
#ifndef UNIT_TEST
#include "mgr_log_rtc.h"
#endif
#endif
#ifdef FLASHLOG_ENABLED
#include "mgr_nvm_mem.h"
#endif

/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#ifdef DEBUG

#ifdef UNIT_TEST

//#if defined(UNIT_TEST_HW) // unit test on STM32 HW
#ifdef UNIT_TEST_HW

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
/* direct logging */
#define MGR_LOG_VERBOSE_RAW(...)	vMGR_LOG_printf(__VA_ARGS__)
#else
#define MGR_LOG_VERBOSE(...)		do {} while (0)
#define MGR_LOG_VERBOSE_RAW(...)	do {} while (0)
#endif // end VERBOSE

/* log AT cmd responses */
#define MGR_LOG_AT(...)		vMGR_LOG_printf(__VA_ARGS__)
/* MGR_LOG_FLASH logs AT cmd responses. Also, it save the response in NVM when FLASHLOG_ENABLED is
 * defined and when flash logging is activated (using AT+FLOG command).
 */
#ifdef FLASHLOG_ENABLED
#define MGR_LOG_FLASH(...)	do {\
					if (bMGR_LOG_isFlashLoggingEnabled())\
						vMgrNvm_AddlogtoFlash(__VA_ARGS__);\
						vMGR_LOG_printf(__VA_ARGS__);\
				} while (0)
#else // FLASHLOG_ENABLED undefined
#define MGR_LOG_FLASH(...)		vMGR_LOG_printf(__VA_ARGS__)
#endif // end FLASHLOG_ENABLED
#else // not UNIT_TEST && UNIT_TEST_HW

#define MGR_LOG_DEBUG			printf
#define MGR_LOG_DEBUG_RAW		printf
#define MGR_LOG_VERBOSE			printf
#define MGR_LOG_VERBOSE_RAW		printf
#define MGR_LOG_AT(...)			vMGR_LOG_printf(__VA_ARGS__)
#define MGR_LOG_FLASH(...)		vMGR_LOG_printf(__VA_ARGS__)
#endif // end UNIT_TEST_HW

#else // not UNIT_TEST

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
/* direct logging */
#define MGR_LOG_VERBOSE_RAW(...)	vMGR_LOG_printf(__VA_ARGS__)
#else
#define MGR_LOG_VERBOSE(...)		do {} while (0)
#define MGR_LOG_VERBOSE_RAW(...)	do {} while (0)
#endif // end VERBOSE

/* log AT cmd responses */
#define MGR_LOG_AT(...)		vMGR_LOG_printf(__VA_ARGS__)
/* MGR_LOG_FLASH logs AT cmd responses. Also, it save the response in NVM when FLASHLOG_ENABLED is
 * defined and when flash logging is activated (using AT+FLOG command).
 */
#ifdef FLASHLOG_ENABLED
#define MGR_LOG_FLASH(...)	do {\
					if (bMGR_LOG_isFlashLoggingEnabled())\
						vMgrNvm_AddlogtoFlash(__VA_ARGS__);\
						vMGR_LOG_printf(__VA_ARGS__);\
				} while (0)
#else // FLASHLOG_ENABLED undefined
#define MGR_LOG_FLASH(...)		vMGR_LOG_printf(__VA_ARGS__)
#endif // end FLASHLOG_ENABLED

#endif /* end of UNIT_TEST */


#else // not DEBUG

#define MGR_LOG_DEBUG(...)		do {} while (0)
#define MGR_LOG_DEBUG_RAW(...)		do {} while (0)
#define MGR_LOG_VERBOSE(...)		do {} while (0)
#define MGR_LOG_VERBOSE_RAW(...)	do {} while (0)
#define MGR_LOG_AT(...)			vMGR_LOG_printf(__VA_ARGS__)
#define MGR_LOG_FLASH(...)		vMGR_LOG_printf(__VA_ARGS__)

#endif // end DEBUG

#ifdef USE_LOCAL_PRINTF

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
void vMGR_LOG_printf(const char *format, ...);

#else // not USE_LOCAL_PRINTF

#define vMGR_LOG_printf			printf

#endif // end USE_LOCAL_PRINTF

bool bMGR_LOG_AT_dataBuf(uint8_t *pu8_inDataBuff, uint16_t u16_dataLenBit);

#ifdef FLASHLOG_ENABLED
/**
 * @brief return FLASH logging status
 * @return "1" if FLASH logging enabled, "0" otherwise
 */
bool bMGR_LOG_isFlashLoggingEnabled(void);

/**
 * @brief Enable/Disable FLASH logging
 * @param[in] b_isEnable: 0/1 to enable or disable FLASH logging
 */
void vMGR_LOG_enableFlashLogging(bool b_isEnable);
#endif // end FLASHLOG_ENABLED

/**
 * @brief Write content of data buffer as AT cmd response
 *
 * @param[in] pu8_inDataBuff: pointer to data buffer
 * @param[in] u16_dataLenBit: length of data buffer in bit
 *
 * @return none
 */
void vMGR_LOG_AT_dataBuf(uint8_t *pu8_inDataBuff, uint16_t u16_dataLenBit);

#endif // end MGR_LOG_H

/**
 * @}
 */
