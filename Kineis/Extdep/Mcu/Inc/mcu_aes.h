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
 * Encryption is used by the Kineis SW library for protocol.
 *
 * @note The DSK provided by Kineis operator (part of credentials: ID, address, DSK) must be used in
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

/* Defines -------------------------------------------------------------------*/
#define DSK_BYTE_LENGTH 16
#define AES_BYTE_LENGTH 16
#define AES_BIT_LENGTH (AES_BYTE_LENGTH * 8)

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Intialize AES module
 *
 * @attention Once this function returns KNS_STATUS_OK, the Kineis stack assumes that AES module
 * is correctly initialized with the input key. If key is NULL, it is assumed that AES module is
 * initialized with the DSK - Device Secret Key (provided by Kineis operator).
 * Any following call to \ref MCU_AES_128_cbc_encrypt API will encrypt data with the key set here
 * (and respectively \ref MCU_AES_128_cbc_decrypt will decrypt with this key).
 *
 * @param[in] key: key to set for AES (or set key with DSK if NULL)
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_AES_128_init(uint8_t key[]);

/**
 * @brief Encrypt a 16-bytes-long data
 *
 * @note This API should internally call some AES CBC algorithm
 *
 * @param[in]       in: pointer to input data
 * @param[out]     out: pointer to output encrypted data, containing the result of the
 *                      encryption using the key loaded in the \ref MCU_AES_128_init
 * @param[in] nb_block: number of 16-bytes blocks to encrypt
 * @param[in]       iv: init vector of CBC AES algorithm
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_AES_128_cbc_encrypt(const uint8_t *in, uint8_t *out, int32_t nb_block, uint8_t *iv);

/**
 * @brief Decrypt a 16-bytes-long data
 *
 * @note This API should internally call some AES CBC algorithm
 *
 * @param[in]       in: pointer to input data
 * @param[out]     out: pointer to output decrypted data containing the result of the
 *                      decryption using the key loaded in the \ref MCU_AES_128_init
 * @param[in] nb_block: number of 16-bytes blocks to decrypt
 * @param[in]       iv: init vector of CBC AES algorithm
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_AES_128_cbc_decrypt(const uint8_t *in, uint8_t *out, int32_t nb_block, uint8_t *iv);

#endif // end MCU_AES_H

/**
 * @}
 */
