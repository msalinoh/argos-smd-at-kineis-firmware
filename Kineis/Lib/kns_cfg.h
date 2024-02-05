/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_cfg.h
 * @brief   Device communication driver for Kineis radio frequence
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
 * @page kns_cfg_page KNS_CFG: Service handling general kineis stack configuration parameters
 *
 * This page is presenting the Kineis configuration service.
 *
 * Its purpose is to:
 * * Provide information such as ID, radio config
 */

/**
 * @addtogroup KNS_SRVC
 * @brief Kineis RF Transceiver module. (refer to @ref kns_cfg_page for general description).
 * @{
 */

#ifndef KNS_CFG_H
#define KNS_CFG_H

/* Includes ------------------------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>
#include "kns_types.h"

#pragma GCC visibility push(default)

/* Global defines -------------------------------------------------------------------------------*/
#define DEVICE_ADDR_LENGTH        4
#define DEVICE_SN_LENGTH          14

/* Structures ---------------------------------------------------------------------------------- */

/**
 * @struct KNS_CFG_radio_t
 * @brief radio configuration parameters of Kineis services, may depend on platform needs
 */
struct KNS_CFG_radio_t {
	uint32_t min_frequency;
	uint32_t max_frequency;
	int8_t rf_level;
	enum KNS_tx_mod_t modulation;
};

/* Function prototypes ------------------------------------------------------------------------- */

/**
 * @brief Get the value of the Message Counter
 *
 * @param[out] mc_ptr : pointer where the Message Counter value is copied
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_getMC(uint16_t *mc_ptr);

/**
 * @brief Set the value of the Message Counter
 *
 * @param[in] mcTmp : value to store in non-volatile memory
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_setMC(uint16_t mcTmp);

/**
 * @brief Get the Kineis radio configuration
 *
 * @param[out] radio_cfg : pointer to a radio configuration
 * The radio configuration is an encrypted memory bloc containing data
 * about radio settings of the device. This data bloc is provided by Kineis.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_getRadioInfo(struct KNS_CFG_radio_t *radio_cfg);

/**
 * @brief Set the Kineis radio configuration
 *
 * @param[in] radio_cfg : pointer to a radio configuration
 * The radio configuration is an encrypted memory bloc containing data
 * about radio settings of the device. This data bloc is provided by Kineis.
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_setRadioInfo(void *radio_cfg);

/**
 * @brief Get the Kineis device identifier
 *
 * @param[out] dev_id : device identifier
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_getId(uint32_t *dev_id);

/**
 * @brief Get the Kineis device address
 *
 * @param[out] dev_add : device address (4-bytes binary array)
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_getAddr(uint8_t dev_add[]);

/**
 * @brief Get the device serial number
 *
 * @param[out] sn : serial number (14-bytes null terminated ASCII string)
 *
 * @return Status @ref KNS_status_t
 */
enum KNS_status_t KNS_CFG_getSN(uint8_t sn[]);

#pragma GCC visibility pop

#endif /* KNS_CFG_H */
/**
 * @}
 */
