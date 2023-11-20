/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    mcu_aes.h
 * @brief   MCU wrapper for encryption/decryption wrappers utilities
 * @author  Kineis
 * @date    Creation 2022/10/17
 */

/**
 * @page mcu_aes_page MCU wrappers concerning Encryption
 *
 * Encryption is used by the Kinéis SW library for protocol.
 *
 * @note The DSK provided by Kinéis operator (part of credentials: ID, address, DSK) must be used in
 * this AES wrapper.
 *
 * @attention Here the integrator can implement his own version of this wrapper, depending on his
 * expected security strategy (from software aes up to secure element or crypto MCU).
 */

/**
 * @addtogroup MCU_WRAPPERS
 * @brief MCU wrapper used for encryption/decryption features
 * @{
 */

#ifndef MCU_AES_H
#define MCU_AES_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "kns_types.h"

/* Defines -------------------------------------------------------------------*/
#define DSK_BYTE_LENGTH 16
#define AES_BYTE_LENGTH 16
#define AES_BIT_LENGTH (AES_BYTE_LENGTH * 8)

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Intialize AES module
 *
 * @attention Once this function returns KNS_STATUS_OK, the Kinéis stack assumes AES module is
 * correctly initialized with the DSK (provided by Kinéis operator) and any call to
 * \ref MCU_AES_128_encrypt API will encrypt data with this DSK.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_AES_128_init(void);

/**
 * @brief Encrypt a 16-bytes-long data
 *
 * @note This API should internally call some AES CBC algorithm
 *
 * @param[in]  in:  pointer to input data
 * @param[out] out: pointer to ouput encrypted data containing the result of the encryption using
 *                  the DSK loaded in the MCU_AES_128_init
 * @param[in]  iv:  init vector of AES
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_AES_128_encrypt(const uint8_t *in, uint8_t *out, uint8_t *iv);

#endif // end MCU_AES_H

/**
 * @}
 */
