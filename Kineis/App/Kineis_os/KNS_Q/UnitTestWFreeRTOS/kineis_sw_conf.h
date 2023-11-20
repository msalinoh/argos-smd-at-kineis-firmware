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
#define __disable_irq(...) {}
#define __enable_irq(...)  {}
#define __get_PRIMASK(...) 0 // return always 0 as Priority Mask
#else
#include "cmsis_compiler.h"
#endif

#endif // end KINEIS_SW_CONF_H

/**
 * @}
 */
