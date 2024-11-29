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

#ifndef KNS_MAC_EVT_H
#define KNS_MAC_EVT_H

/* Includes ------------------------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>
#include "kns_types.h"
#include "kns_mac_prfl_cfg.h"

#pragma GCC visibility push(default)

/* Defines ------------------------------------------------------------------------------------- */

#ifdef USE_HDA4
#define KNS_MAC_USRDATA_MAXLEN 633 // 632.5 bytes long as maximum allowed in HDA4 (5060 bits)
#else
#define KNS_MAC_USRDATA_MAXLEN 25 // 24.5 bytes long as maximum allowed in LDA2L
#endif

#define DL_FRM_BITLEN 384 /* maximum bit length of a DL frame */
#define DL_FRM_SZ  (((DL_FRM_BITLEN-1)/8)+1) /*  max frame len in byte */

/* Enums --------------------------------------------------------------------------------------- */

/**
 * @enum KNS_MAC_appEvtId_t
 * @brief List events which could be reported by service layer
 */
enum KNS_MAC_appEvtId_t {
        KNS_MAC_APP_NONE_EVT,  /**< none, placed as value 0 to detect potential issues in SW */
        KNS_MAC_INIT,          /**< Init MAC profile */
	KNS_MAC_SEND_DATA,     /**< Send data on Kineis UL link */
	KNS_MAC_STOP_SEND_DATA,/**< Abort Send data on Kineis UL link */
	KNS_MAC_RX_START,      /**< Start reception on Kineis DL link (e.g. DL_BC or AOP/CS update) */
	KNS_MAC_RX_STOP        /**< Stop reception on Kineis DL link */
};

/**
 * @enum KNS_MAC_srvcEvtId_t
 * @brief List events which could be reported by service layer
 */
enum KNS_MAC_srvcEvtId_t {
	KNS_MAC_SRVC_NONE_EVT, /**< default value meaning event is not valid */
	KNS_MAC_TX_DONE,       /**< TX completed */
	KNS_MAC_TX_TIMEOUT,    /**< TX failed, timeout reached */
	KNS_MAC_TXACK_DONE,    /**< UL-ACK TX completed */
	KNS_MAC_TXACK_TIMEOUT, /**< UL-ACK TX failed, timeout reached */
	KNS_MAC_RX_ERROR,      /**< error occurred during RX process */
	KNS_MAC_RX_RECEIVED,   /**< RX frame received */
	KNS_MAC_RX_TIMEOUT,    /**< RX window timeout */
	KNS_MAC_DL_BC,         /**< downlink message received: beacon command */
	KNS_MAC_DL_ACK,        /**< downlink message received: acknowledge */
	KNS_MAC_SAT_DETECTED,  /**< SATellite detection complete */
	KNS_MAC_SAT_LOST,      /**< SATellite lost */
	KNS_MAC_RF_ABORTED,    /**< RF action fully aborted */
	KNS_MAC_OK,            /**< generic status telling OK to command */
	KNS_MAC_ERROR          /**< generic error event */
};

/**
 * @enum KNS_MAC_infraEvtId_t
 * @brief List events which could be reported by service layer
 */
enum KNS_MAC_infraEvtId_t {
	KNS_MAC_TRIG_TX        /**< Send data on Kineis UL link */
};

/* Structures ---------------------------------------------------------------------------------- */

	/* ---- APP-to-MAC events ---- */

/**
 * @struct KNS_MAC_send_data_ctxt_t
 * @brief Event context structure containing usefull information for the APPlication to send data
 * to Kineis
 */
struct KNS_MAC_send_data_ctxt_t {
	uint8_t usrdata[KNS_MAC_USRDATA_MAXLEN];
	uint16_t usrdata_bitlen;
	enum  KNS_serviceFlag_t sf;
};

/**
 * @struct KNS_MAC_appEvt_t
 * @brief Generic event structure with ID and context
 *
 * Union describing event contexts
 */
struct KNS_MAC_appEvt_t {
	enum KNS_MAC_appEvtId_t id;
	union {
		struct KNS_MAC_prflInfo_t init_prfl_ctxt;
		struct KNS_MAC_send_data_ctxt_t data_ctxt;
	};
};

	/* ---- SRV-Cto-MAC events ---- */

/**
 * @struct KNS_MAC_TX_cplt_ctxt_t
 * @brief TX-done/TX-timeout event context structure
 *
 * All useful parameters needed when reporting TX is complete
 */
struct KNS_MAC_TX_cplt_ctxt_t {
	uint8_t data[KNS_MAC_USRDATA_MAXLEN];
	uint16_t data_bitlen;
};

/**
 * @struct KNS_MAC_RX_frm_ctxt_t
 * @brief RX-frameè-received event context structure
 *
 * All useful parameters needed when some frame is received
 */
struct KNS_MAC_RX_frm_ctxt_t {
	uint8_t data[DL_FRM_SZ];
	uint16_t data_bitlen;
};

/**
 * @struct KNS_MAC_dl_bc_ctxt_t
 * @brief DL-message-received  event context structure
 *
 * All useful parameters needed when some frame is received
 */
struct KNS_MAC_RX_dl_msg_ctxt_t {
	uint8_t data[DL_FRM_SZ];
	uint16_t data_bitlen;
	uint16_t bc_mc;
};

/**
 * @struct KNS_MAC_srvcEvt_t
 * @brief Generic structure/union describing events coming from SERVICE layer
 *
 * This events are internally used by MAC layer. But @note most of them are converted and forwarded
 * to APP layer as notifications.
 *
 */
struct KNS_MAC_srvcEvt_t {
	enum KNS_MAC_srvcEvtId_t id;
	enum KNS_MAC_appEvtId_t app_evt; /**< original APP event which triggered this.
					  * Correctly set for KNS_MAC_OK and KNS_MAC_ERROR evt
					  */
	union {
		struct KNS_MAC_TX_cplt_ctxt_t tx_ctxt; /**< TX-complete notification context
							 * Also set in case of KNS_MAC_OK and
							 * KNS_MAC_ERROR evt
							 */
		struct KNS_MAC_RX_frm_ctxt_t rx_ctxt; /**< RX-frame-received notification context */
		struct KNS_MAC_RX_dl_msg_ctxt_t msg_ctxt; /**< decoded DL msg context */
	};
};

	/* ---- INFRA-to-MAC events ---- */

/**
 * @struct KNS_MAC_infraEvt_t
 * @brief Generic structure of events coming from INFRA (e.g. timer)
 *
 * Union describing event contexts
 */
struct KNS_MAC_infraEvt_t {
	enum KNS_MAC_infraEvtId_t id;
	union {
		uint32_t trigtx_ctxt; /**< TRIG_TX event context */
	};
};

#pragma GCC visibility pop

#endif /* KNS_MAC_EVT_H */
/**
 * @}
 */
