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

#include "kns_types.h"
#include "aes.h"
#include "mcu_aes.h"

/* Variables ---------------------------------------------------------*/
static aes_context ctx;

/** For security reason, the device secret key cannot appear clearly inside the
 * device memory. This key should be hidden as much as possible and be hardly
 * accessible from the outside. It is recommended to use an encryption mechanism
 * (AES, etc.) to store it.
 *
 * It is up to the integrator to store it as per its own security strategy.
 *
 * @attention Credentials are delivered by Kineis after certificication of your device.
 *
 * @attention Once credentials are delivered by Kineis, it becomes your responsability.
 */
static const uint8_t device_secret_key[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
					0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

/* Functions -------------------------------------------------------------*/

enum KNS_status_t MCU_AES_128_init(uint8_t key[])
{
	if (key == NULL) {
		/* Set the AES key with the Device Secret Key */
		if (aes_set_key(device_secret_key, DSK_BYTE_LENGTH, &ctx) != 0)
			return KNS_STATUS_ERROR;
	} else
		/* Set the AES key with the given key */
		if (aes_set_key(key, DSK_BYTE_LENGTH, &ctx) != 0)
			return KNS_STATUS_ERROR;

	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_AES_128_cbc_encrypt(const uint8_t *in, uint8_t *out, int32_t nb_block, uint8_t *iv)
{
	/** Call the AES CBC encrypt */
	if (aes_cbc_encrypt(in, out, nb_block, iv, &ctx) != EXIT_SUCCESS)
		return KNS_STATUS_ERROR;

	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_AES_128_cbc_decrypt(const uint8_t *in, uint8_t *out, int32_t nb_block, uint8_t *iv)
{
	/** Call the AES CBC decrypt */
	if (aes_cbc_decrypt(in, out, nb_block, iv, &ctx) != EXIT_SUCCESS)
		return KNS_STATUS_ERROR;

	return KNS_STATUS_OK;
}

/**
 * @}
 */
