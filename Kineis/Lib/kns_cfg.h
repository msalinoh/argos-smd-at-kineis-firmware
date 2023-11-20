/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_cfg.h
 * @brief   Device communication driver for Kinéis radio frequence
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

/* Structures ---------------------------------------------------------------------------------- */

/**
 * @struct KNS_CFG_radio_t
 * @brief configuration parameters of Kineis services, may depend on platform needsr
 */
struct KNS_CFG_radio_t {
	uint32_t min_frequency;
	uint32_t max_frequency;
	int8_t rf_level;
	uint8_t downlink_enable;
};

/* Function prototypes ------------------------------------------------------------------------- */

enum KNS_status_t KNS_CFG_GetRadioInfo(struct KNS_CFG_radio_t *radio_cfg);

enum KNS_status_t KNS_CFG_GetIdDec(uint32_t *id_dec);

enum KNS_status_t KNS_CFG_GetAddr(uint8_t addr[]);

#endif /* KNS_CFG_H */
/**
 * @}
 */
