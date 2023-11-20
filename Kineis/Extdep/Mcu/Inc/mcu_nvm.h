/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    mcu_nvm.h
 * @brief   Implementation of miscellaneous features for a specific design
 * @author  Kineis
 * @date    Creation 2023/07/06
 */

/**
 * @page mcu_nvm_page MCU wrappers concerning non-volatile settings used by Kinéis stack
 *
 * This is about storing ID and address into some Non-Volatile Memory (NVM).
 * Those settings are part of the credentials (ID, address, DSK) provided by Kinéis operator.
 *
 * @note DSK will be managed by the used by the AES wrapper (mcu_aes.h)
 *
 * @attention It is up to you to manage the storing stategy of those values during the entire life
 * of your device.
 */

/**
 * @addtogroup MCU_WRAPPERS
 * @brief MCU wrapper used for Non volatile memory management.
 * @{
 */

#ifndef MCU_NVM_H_
#define MCU_NVM_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "kineis_sw_conf.h"
#include  STM32_HAL_H
#include "kns_types.h"

/* Function declaration -------------------------------------------------------------*/

/**
 * @brief get a 16-bit-long non-volatile value
 *
 * This is a read-only operation.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_NVM_getMC(int16_t *mc_ptr);

/**
 * @brief set a 16-bit-long non-volatile value
 *
 * The value may be set for each new user message sent to the Kinéis stack.
 *
 * @attention Depending the expected lifetime of your device, ensure your non-volatile memory
 * can support enough write/erase cycles.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_NVM_setMC(int16_t mcTmp);

/**
 * @brief get the Kinéis identifier (32-bits long)
 *
 * This is a read-only operation.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_NVM_getID(uint32_t *id);

/**
 * @brief get the Kinéis address (4-bytes long)
 *
 * This is a read-only operation.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t MCU_NVM_getAddr(uint8_t addr[]);

#endif /* MCU_NVM_H_ */

/**
 * @}
 */
