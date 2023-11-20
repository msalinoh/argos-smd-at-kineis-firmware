/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_mac.h
 * @brief   Main header file for MAC layer of Kineis stack
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
 * @addtogroup KNS_MAC
 * @{
 */

#ifndef KNS_MAC_H
#define KNS_MAC_H

/* Includes ------------------------------------------------------------------------------------ */

#include "kns_mac_evt.h"

#pragma GCC visibility push(default)


/* Enums --------------------------------------------------------------------------------------- */

/**
 * @enum KNS_MAC_prflId_t
 * @brief MAC profile list
 */
enum KNS_MAC_prflId_t {
	KNS_MAC_PRFL_BASIC,  /**< basic profile */
	KNS_MAC_PRFL_MAX   /**< in case of MAIL REQ frm, transmit done, now waiting for DL_BC */
};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief Initialize and start MAC profile
 *
 * @param[in] prflId: profile identification
 * @param[in] prflCtxt: configuiration context, NULL if no contexte needed
 *
 * @return KNS_STATUS_OK if data is correctly processed, any other status if error
 */
enum KNS_status_t KNS_MAC_init(enum KNS_MAC_prflId_t prflId, void* prflCtxt);

/**
 * @brief MAC layer main task to be called from OS
 *
 * This task is the main loop of Kineis stack. It is going to handle all TX/RX/TRX UL/DL events
 * of the kineis modem.
 */
void KNS_MAC_task(void);

#pragma GCC visibility pop

#endif /* KNS_MAC_H */
/**
 * @}
 */
