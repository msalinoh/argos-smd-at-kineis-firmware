/* SPDX-License-Identifier: no SPDX license */
/**
 ******************************************************************************
 * @file           : mgr_log_rtc.h
 * @author         : William BEGOC
 * @brief          : Header for mgr_log_rtc.c file. This file contains log routine concerning RTC.
 *                   Thus there is a dependency on HAL RTC.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 KINEIS.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
/**
 *\page mgr_log_rtc Log Manager
 *
 * This page is presenting utilities and handler for a general UART logs
 *
 *
 * Log Manager is write in console (via UART) some user notification/message
 * or AT command response.
 *
 * There are two types of logs :
 *
 * * Write formatted string into a buffer with variable number of parameters with
 *   a specified maximum number of chars to write
 * * Write formatted string with RTC Date and time
 *
 */
/**
 * @addtogroup MGR_LOG
 * @brief Log manager module (refer to \ref mgr_log_rtc page for general description).
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef MGR_LOG_RTC_H_
#define MGR_LOG_RTC_H_

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/**
 * @brief : Log RTC Date and time
 */
void MGR_LOG_RtcDateTime(void);

#endif /* MGR_LOG_RTC_H_ */

/**
 * @}
 */
