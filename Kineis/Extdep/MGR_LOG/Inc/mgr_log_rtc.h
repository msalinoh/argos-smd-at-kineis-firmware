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
