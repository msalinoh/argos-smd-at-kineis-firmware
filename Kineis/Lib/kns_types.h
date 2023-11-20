/**
 * @file    KNS_types.h
 * @brief   Kineis types definition
 * @author  Kineis
 * @date    creation 2022/07/01
 * @version 1.0
 * @note
 */

/**
 * @addtogroup KNS_TYPES
 * @brief Kineis generic types used in most of the kineis SW stack functions
 * @{
 */

#ifndef KNS_TYPES_H
#define KNS_TYPES_H

#include <stdint.h>

#pragma GCC visibility push(default)

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Enums --------------------------------------------------------------------------------------- */

/**
 * @enum KNS_status_t
 * @brief Status codes returned by driver. All KNS_xxx functions returns this status type.
 * @todo This status should be common to all driver as much as possible.
 */
enum KNS_status_t {
	KNS_STATUS_OK,       /**< status OK */
	KNS_STATUS_ERROR,    /**< status generic ERROR, in case none below fits the root cause */

	KNS_STATUS_DISABLED, /**< module is disabled (transceiver, bus, ... */
	KNS_STATUS_BUSY,     /**< module is busy (transceiver, bus, ... */
	KNS_STATUS_TIMEOUT,  /**< timeout reached */

	KNS_STATUS_BAD_SETTING, /**< wrong configuration settig for radio */
	KNS_STATUS_BAD_LEN,  /**< TX data frame length error */

	KNS_STATUS_TR_ERR,   /**< Error during transceiver action (TX, RX, SPI/A2S/... error) */

	KNS_STATUS_QFULL,    /**< queue is full */
	KNS_STATUS_QEMPTY,    /**< queue is empty */
	KNS_STATUS_NVM_ACCESS_ERR  /**< for all nvm access issue */
};

/**
 * @enum KNS_serviceFlag_t
 * @brief Kineis frames's service flag used in UL frames, also used as attribute to user data.
 */
enum KNS_serviceFlag_t {
  KNS_SF_NO_SERVICE                = 0b000, /**< No specific service, one-way UL frame */
  KNS_SF_MAIL_REQUEST              = 0b001, /**< UL frame asking for any DL beacon request? */
  KNS_SF_BC_UNICAST_ACK            = 0b010, /**< UL frame ACKnowledging a unicast beacon cmd */
  KNS_SF_BC_MULTICAST_ACK          = 0b011, /**< UL frame ACKnowledging a multicast beacon cmd */
  KNS_SF_PACK_NORMAL_REQUEST       = 0b100, /**< UL frame requesting a DL ACKnowledgment */
  KNS_SF_PACK_EMERGENCY_REQUEST    = 0b101  /**< UL frame requesting a DL emergency ACK */
};

/**
 * @enum KNS_satServiceFlag_t
 * @brief Kineis satellite service flag used in DL frames.
 */
enum KNS_satServiceFlag_t {
  KNS_SAT_SF_SYS_BC         = 0b00, /**< DL system beacon command */
  KNS_SAT_SF_USER_BC        = 0b01, /**< DL user beacon command */
  KNS_SAT_SF_POSITIVE_ACK   = 0b10, /**< DL frame ACKnowledging an UL frame */
  KNS_SAT_SF_SPARE          = 0b11, /**< Spare : Can be used for allcast messages */
};

/**
 * @enum KNS_tx_mod_t
 * @brief Transmission modulation
 *
 * @todo need to put modulation back into kns_tx.h in KNS_TX_cfg_t
 */
enum KNS_tx_mod_t {
	KNS_TX_MOD_A2,    /**< modulation A2 */
	KNS_TX_MOD_A2L,    /**< modulation A2 Legacy*/
	KNS_TX_MOD_VLDA4, /**< modulation VLDA4 */
	KNS_TX_MOD_LDK,    /**< modulation LDK */
};

/**
 * @struct KNS_rf_tx_cfg_t
 * @brief TX configuration structure
 *
 * All parameters requested to define radio tx configuration.
 */
struct KNS_tx_rf_cfg_t {
	uint32_t center_freq;         /**< Carrier uplink center frequency to transmit */
	enum KNS_tx_mod_t modulation; /**< Modulation to use for tx uplink msg */
	int8_t power;
};

#pragma GCC visibility pop

#endif /* end of KNS_TYPES_H */

/**
 * @}
 */
