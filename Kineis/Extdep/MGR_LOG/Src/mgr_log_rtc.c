/**
 ******************************************************************************
 * @file    mgr_log_rtc.c
 * @brief   This file contains log routine concerning RTC. There is a dependency on HAL RTC.
 *
 * @date     2020/04/21
 * @author   creation : William BEGOC
 * @version  1.0
 * @note
 ******************************************************************************
 */

/**
 ******************************************************************************
 * @addtogroup MGR_LOG
 * @{
 ******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdio.h>
#include "mgr_log.h"
#include "mgr_log_rtc.h"
#include "kns_app_conf.h" // for STM32 HAL include
#include STM32_HAL_H
#include STM32_HAL_RTC_H

/*******************************************************************************
 * EXTERNS
 ******************************************************************************/
extern RTC_HandleTypeDef hrtc;

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/
void MGR_LOG_RtcDateTime(void)
{
	RTC_DateTypeDef sdatestructureget;
	RTC_TimeTypeDef stimestructureget;

	/* Buffer used for displaying Time */
	uint8_t aShowTime[15] = {0};
	uint8_t aShowDate[15] = {0};

	HAL_RTC_WaitForSynchro(&hrtc);
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

	/* Display date Format : mm-dd-yy */
	sprintf((char *)aShowDate,
			"%04d/%02d/%02d",
			2000 + sdatestructureget.Year,
			sdatestructureget.Month,
			sdatestructureget.Date);

	/* Display time Format : hh:mm:ss */
	sprintf((char *)aShowTime,
			"%02d:%02d:%02d",
			stimestructureget.Hours,
			stimestructureget.Minutes,
			stimestructureget.Seconds);

	vMGR_LOG_printf((char *)aShowDate);
	vMGR_LOG_printf(" ");
	vMGR_LOG_printf((char *)aShowTime);
}

/**
 * @}
 */
