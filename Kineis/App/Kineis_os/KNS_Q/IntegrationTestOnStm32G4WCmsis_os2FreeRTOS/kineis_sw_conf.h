/**
 * @file        kineis_sw_conf.h
 * @brief       kineis sw configuration concerns
 * @note
 *
 */

/**
 * @addtogroup KINEIS_SW_CONF
 * @brief Kineis SW configuration. This module can be adapted by external user depending on project
 * @{
 */

#ifndef KINEIS_SW_CONF_H
#define KINEIS_SW_CONF_H


/* Includes ------------------------------------------------------------------------------------ */

/**
 * @brief Platform dependent includes for basic types such uint8_t, uint32_t, ...
 */
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Include containing APIs used to handle enabling/disabling of interrupts:
 * * __disable_irq
 * * __enable_irq
 * * __get_PRIMASK
 *
 * @note Those APIs are stubbed in case of Unit test as:
 * * CMOCK/CUNIT framework is not able to mock it by itslef
 * * There is no interrupt to block during unit tests, most of the time.
 */
#ifdef UNIT_TEST
#ifndef UNIT_TEST_HW
#define __disable_irq(...) {}
#define __enable_irq(...)  {}
#define __get_PRIMASK(...) 0 // return always 0 as Priority Mask
#endif
#else
#include "cmsis_compiler.h"
#endif

/* Defines ------------------------------------------------------------------------------------- */

#define STM32_HAL_H		"stm32g4xx_hal.h"
#define STM32_HAL_CONF_H	"stm32g4xx_hal_conf.h"
#define STM32_HAL_RTC_H		"stm32g4xx_hal_rtc.h"
#define STM32_HAL_LPTIM_H	"stm32g4xx_hal_lptim.h"
#define STM32_HAL_UART_H	"stm32g4xx_hal_uart.h"
#define STM32_HAL_SPI_H		"stm32g4xx_hal_spi.h"
#define STM32_IT_H		"stm32g4xx_it.h"


//! Unit tests
#define MOCKSTM32_HAL_H			"Mockstm32g4xx_hal.h"
#define MOCKSTM32_HAL_GPIO_H		"Mockstm32g4xx_hal_gpio.h"
#define MOCKSTM32_HAL_RTC_H		"Mockstm32g4xx_hal_rtc.h"
#define MOCKSTM32_HAL_CORTEX_H		"Mockstm32g4xx_hal_cortex.h"
#define MOCKSTM32_HAL_UART_H		"Mockstm32g4xx_hal_uart.h"
#define MOCKSTM32_HAL_SPI_H		"Mockstm32g4xx_hal_spi.h"

/* MGR_LOG uncomment below if MGR_LOG is used */
#include "mgr_log_conf.h"

#endif // end KINEIS_SW_CONF_H

/**
 * @}
 */
