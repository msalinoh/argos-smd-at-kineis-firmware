/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    mgr_log_conf.h
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

#ifndef MGR_LOG_CONF_H
#define MGR_LOG_CONF_H

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#ifdef USE_LOCAL_PRINTF
#include STM32_HAL_H

#define log_uart			huart2

/*******************************************************************************
 * EXTERNAL VARIABLES
 ******************************************************************************/
extern UART_HandleTypeDef huart2;

#endif // end USE_LOCAL_PRINTF

#endif // end MGR_LOG_H

/**
 * @}
 */
