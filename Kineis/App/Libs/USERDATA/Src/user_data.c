// SPDX-License-Identifier: no SPDX license
/**
 * @file    user_data.c
 * @author  Kinéis
 * @brief   Library to handle user data to be be received or transmitted over the air (e.g.. on
 *          Kinéis network)
 */

/**
 * @addtogroup USER_DATA
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "user_data.h"

/* Variables -----------------------------------------------------------------*/

/** @brief default values of TX fifo elements (all cleared) */
const struct sUserDataTxFifoElt_t sUserDataTxEltDflt =  {
		.u16DataBitLen = 0,
		.u8DataBuf = {0},
		.bIsToBeTransmit = false,
		.u8Attr.u8_raw = 0x00
};

struct sUserDataTxFifoElt_t sUserDataTx = sUserDataTxEltDflt;

/**
 * @}
 */
