/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_log_conf.h
 * @author Kinéis
 * @brief template configuration for logging manager of kineis_sw libraries
 *
 * This file contains configuration example for logging manager.
 * Its content depends on target platform.
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

#ifndef MGR_LOG_CONF_H
#define MGR_LOG_CONF_H

/* Includes ------------------------------------------------------------------*/

#ifdef USE_LOCAL_PRINTF
#include "stm32wlxx_hal.h" // used for UART handler and assert_param

/* Defines -------------------------------------------------------------------*/

#define log_uart	huart2

/* Variables -----------------------------------------------------------------*/

extern UART_HandleTypeDef huart2;

#endif // end USE_LOCAL_PRINTF

#endif // end MGR_LOG_CONF_H

/**
 * @}
 */
