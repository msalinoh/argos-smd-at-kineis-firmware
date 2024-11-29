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
 * @page kns_mac_page MAC task (Kineis Stack entry point)
 *
 * This page is presenting the task holding the Kineis stack (\ref KNS_MAC) and how to use it
 *
 * The \ref KNS_MAC_task can be started by the OS as a classical task.
 * Once started, the task will internally go through each queue waiting for any new event coming
 * from the outside world (APP task, KNS_RF events, INFRA interrupts).
 *
 * The first thing to really start the kineis stack is to init the profile with \ref KNS_MAC_init
 * direct API.
 *
 * Once initialized, the stack is able to receive envents from the external worlds (APP task,
 * KNS_RF, INFRA ISR).
 *
 * * Top APIs APP-to-MAC      : cf events in \ref KNS_MAC_appEvtId_t
 * * Top APIs MAC-to-APP      : cf events in \ref KNS_MAC_srvcEvtId_t
 * * Bottom APIs MAC-to-KNS_RF: cf direct APIs \ref kns_rf.h
 * * Bottom APIs KNS_RF-to-MAC: cf events in \ref KNS_RF_eventId_t reported to MAC through callbacks
 *
 * @section kns_app_subpages Sub-pages
 *
 * * @subpage mcu_aes_page
 * * @subpage mcu_nvm_page
 * * @subpage mcu_tim_page
 * * @subpage kns_rf_page
 */

/**
 * @addtogroup KNS_MAC
 * @{
 */

#ifndef KNS_MAC_H
#define KNS_MAC_H

/* Includes ------------------------------------------------------------------------------------ */

#include "kns_mac_evt.h"
#include "kns_mac_prfl_cfg.h"

#pragma GCC visibility push(default)

/* Types --------------------------------------------------------------------------------------- */

/** @brief Bitmap listing ressources currently used by MAC protocol (timers, RTC, RAM, ...)
 *
 * By comparing this list of active peripherals and the corresponding HW modules (cf corresponding
 * MCU wrappers), it is possible to deduce what can be the deepest LPM without impacting the MAC
 * protocol.
 */
union KNS_MAC_rsrcStatus_t {
	struct {
		uint32_t keepKnsCtxt:1;      // __attribute__((__section__(".knsCtxtData")))
		uint32_t modemOn:1;          // kineis modem (SUBGHZ perpih on KRD)
		uint32_t satDetTimeout:1;    // MCU_tim_...(MCU_TIM_HDLR_SATDET_TIMEOUT,
		uint32_t txTimeout:1;        // MCU_tim_...(MCU_TIM_HDLR_TX_TIMEOUT…)
		uint32_t rxTimeout:1;        // MCU_tim_...(MCU_TIM_HDLR_RX_TIMEOUT…)
		uint32_t txPeriod:1;         // MCU_tim_...(MCU_TIM_HDLR_TX_PERIOD,
		uint32_t satPassStart:1;     // [TBD] MCU_rtc
		uint32_t satPassEnd:1;       // [TBD] MCU_rtc
	};
	uint32_t raw;
};

/* Public functions ----------------------------------------------------------*/

/**
 * @brief MAC layer main task to be called from OS
 *
 * This task is the main loop of Kineis stack. It is going to handle all TX/RX/TRX UL/DL events
 * of the kineis modem.
 */
void KNS_MAC_task(void);

/**
 * @brief Get current MAC profile info
 *
 * @param[out] prflInfo pointer to the current MAC configiration
 *
 * @return KNS_MAC_rsrcStatus_t bitmap listing the peripherals
 */
enum KNS_status_t KNS_MAC_getPrflInfo(struct KNS_MAC_prflInfo_t *prflInfo);

/**
 * @brief Get the status of the ressource currently used by MAC protocol
 *
 * @return KNS_MAC_rsrcStatus_t bitmap listing the peripherals
 */
union KNS_MAC_rsrcStatus_t KNS_MAC_getRsrcStatus(void);

/**
 * @brief Callback notifying MAC which ressources will remain active when entering into the incoming
 * low power mode
 *
 * @param[in] rsrc bitmap of ressources which will remain alive during LPM
 *
 * @return KNS_STATUS_OK status if success, ERROR otherwise
 */
enum KNS_status_t KNS_MAC_lpmEnter(union KNS_MAC_rsrcStatus_t rsrc);

/**
 * @brief Callback notifying the low power mode has just exit
 *
 * As a reminder, the input parameter lists which ressources were active during the just-exited low
 * power mode
 *
 * @param[in] rsrc bitmap of ressources which were alive during LPM (no need to restore them)
 *
 * @return KNS_STATUS_OK status if success, ERROR otherwise
 */
enum KNS_status_t KNS_MAC_lpmExit(union KNS_MAC_rsrcStatus_t rsrc);

#pragma GCC visibility pop

#endif /* KNS_MAC_H */
/**
 * @}
 */
