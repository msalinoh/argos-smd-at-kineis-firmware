/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kineis_sw_conf.h
 * @brief   Kineis stack SW configurations depending on platform used
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
 * @brief Kineis SW configuration. This module can be adapted by external user depending on project
 * @{
 */

#ifndef KINEIS_SW_CONF_H
#define KINEIS_SW_CONF_H

/* Defines ------------------------------------------------------------------------------------- */

/**
 * @brief define include file concerninq assert used by Kineis stack
 *
 * @attention DO NOT change this as long as you build with Kinés stack. Must keep it the same way
 * it was when generating the library (libkineis.a).
 */
#define KINEIS_SW_ASSERT_H	"kns_assert.h"

/**
 * @brief define include file concerninq critical sections used by Kineis stack
 *
 * @attention DO NOT change this as long as you build with Kinés stack. Must keep it the same way
 * it was when generating the library (libkineis.a).
 */
#define KINEIS_CS_H		"kns_cs.h"

/* Enums --------------------------------------------------------------------------------------- */

/**
 * @brief Upper error code for KIM device. So far, this is used as return code of AT commands.
 */
enum ERROR_RETURN_T {
	// general errors
	ERROR_NO                        = 0,
	ERROR_UNKNOWN                   = 1,
	ERROR_PARAMETER_FORMAT          = 2,
	ERROR_MISSING_PARAMETERS        = 3,
	ERROR_TOO_MANY_PARAMETERS       = 4,
	ERROR_INCOMPATIBLE_VALUE        = 5,
	ERROR_UNKNOWN_AT_CMD            = 6,
	ERROR_INVALID_ID                = 7,
	ERROR_UNKNOWN_ID                = 8,

	// user data errors
	ERROR_INVALID_USER_DATA_LENGTH  = 20,
	ERROR_DATA_QUEUE_FULL           = 21,
	ERROR_DATA_QUEUE_EMPTY          = 22,

	// protocol errors
	ERROR_RX_TIMEOUT                = 30,

	// trcvr errors
	ERROR_TRCVR                     = 40,
	ERROR_TRCVR_AUTO_RANGING        = 41,
	ERROR_TRCVR_PLL_ERROR           = 42,
	ERROR_TRCVR_XTAL_TIMEOUT        = 43,
	ERROR_TRCVR_RESET               = 44,

	// config errors
	ERROR_PROT_INVALID_TXFREQ_TXMOD = 60
};

#endif // end KINEIS_SW_CONF_H

/**
 * @}
 */
