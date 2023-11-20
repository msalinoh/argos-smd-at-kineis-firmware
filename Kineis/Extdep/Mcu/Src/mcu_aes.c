// SPDX-License-Identifier: no SPDX license
/**
 * @file    mcu_aes.c
 * @brief   MCU wrapper for encryption/decryption wrappers utilities
 *
 * @author  Kineis
 * @date    Creation 2022/10/17
 * @version 1.0
 * @note
 */

/**
 * @addtogroup MCU_WRAPPERS
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
#include "mcu_aes.h"
#include "kns_types.h"

#define DSK_LENGTH	16

aes_context ctx;
uint8_t secret_key[DSK_LENGTH];
/* Functions -------------------------------------------------------------*/


enum KNS_status_t MCU_AES_128_init(void)
{
	uint8_t secret_key[DSK_LENGTH] = {0};
	/** Set the DSK coming from Kinéis credentials */
	aes_set_key(secret_key, DSK_LENGTH, &ctx); //> ctx is reinit in this function.
	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_AES_128_encrypt(const uint8_t *in, uint8_t *out, uint8_t *iv)
{
	/** Call the AES CBC */
	aes_cbc_encrypt(in, out, 1, iv, &ctx);
	return KNS_STATUS_OK;
}

/**
 * @}
 */
