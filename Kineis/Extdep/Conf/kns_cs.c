// SPDX-License-Identifier: no SPDX license
/**
 * @file    kns_cs.c
 * @brief   critical section handling for kineis stack.
 * @author  Kinéis
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
 * @brief Kineis SW configuration regarding critical sections
 * @{
 */

/* Includes ------------------------------------------------------------------------------------ */

#include <stdint.h>

#include "main.h"

#pragma GCC visibility push(default)

/* Private variables --------------------------------------------------------------------------- */

static uint32_t prim;

/* Function ------------------------------------------------------------------------------------ */

void KNS_CS_enter()
{

	prim = __get_PRIMASK();
	__disable_irq();
}

void KNS_CS_exit()
{
	if (!prim) {
		__enable_irq();
	}
}

#pragma GCC visibility pop

/**
 * @}
 */
