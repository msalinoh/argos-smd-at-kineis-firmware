/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_assert.h
 * @brief   ASSERT handling for kineis stack. Handle way to reset device if needed
 * @author  Kineis
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
 * @addtogroup KINEIS_SW_CONF
 * @brief Kineis SW configuration regarding asserts
 * @{
 */

#ifndef KNS_ASSERT_H
#define KNS_ASSERT_H

/* Includes ------------------------------------------------------------------------------------ */

/**
 * @brief Platform dependent includes for basic types such uint8_t, uint32_t, ...
 */
#include <stdint.h>

/* Defines ------------------------------------------------------------------------------------- */
/**
  * @brief  This macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
#define kns_assert(expr) ((expr) ? (void)0U : kns_assert_failed((uint8_t *)__FILE__, __LINE__))

/* Exported functions ------------------------------------------------------- */
void kns_assert_failed(uint8_t *file, uint32_t line);

#endif // end KNS_ASSERT_H

/**
 * @}
 */
