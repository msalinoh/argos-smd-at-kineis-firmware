/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    user_data.h
 * @author  Kinéis
 * @brief   Library to handle user data to be be received or transmitted over the air (e.g.. on
 *          Kinéis network).
 */

/**
 * \page user_data_page User Data library
 *
 * This library is a kind of very high layer dedicated to handle user data messages for
 * transmission and/or reception.
 *
 * So far, this lib is only containing a reference (data buffer, status, attributes) for 1 uplink
 * message. By the end, this lib can turn to handling a FIFO of message with services such as push,
 * pop, isEmpty, isfull.
 */

/**
 * @addtogroup USER_DATA
 * @brief  User data Library. (refer to \ref user_data_page page for general description).
 * @{
 */

#ifndef USER_DATA_H
#define USER_DATA_H

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include "kineis_sw_conf.h"

#pragma GCC visibility push(default)

/* Defines -------------------------------------------------------------------*/

/**
 * Maximum size, in bytes, of a TX-FIFO data:
 *
 * It depends on maximum allowed number of bit in KINEIS protocol:
 *  * for A2/A3 protocols:
 *  * for A4 VLD protocols: 3
 *  * for HDA4 protocol:
 *  * for LDK protocol:
 */
#ifndef USERDATA_TX_PAYLOAD_MAX_SIZE
#define USERDATA_TX_PAYLOAD_MAX_SIZE	25
#endif

#if (USERDATA_TX_PAYLOAD_MAX_SIZE < 25)
#error "USERDATA TX FIFO MAX SIZE cannot support maximum payload size for USER data payload"
#endif

/* Types ---------------------------------------------------------------------*/

/**
 * @brief Enum defining service flag field of the user data attribute
 */
enum eUserDataAttrService {
	ATTR_NONE           = 0, /**< basic type of message, only send UL data */
	ATTR_MAIL_REQUEST   = 1, /**< user expects to request if some donwlink message is for him */
	ATTR_UNICAST_ACK    = 2, /**< reserved, internal usage */
	ATTR_MULTICAST_ACK  = 3, /**< reserved, internal usage */
	ATTR_PACK           = 4, /**< user request some ACKnowledgment for this msg */
	ATTR_PACK_EMERGENCY = 5  /**< user request some ACKnowledgment for this emergency msg */
};

/**
 * @brief structure containing USERDATA attribute
 *
 * This attribute is set by user, telling if message is important or not, and which type of message
 * it is. It may impact lower layer strategies to transmit the message. So far, attribute is
 * defined as:
 * * bit0-2: service (000 no, 001 mail request, 010 ACK, ... reserved)
 * * bit3  : add back/front (0 back, 1 front)
 * * bit4-7: reserved
 *
 */
union sUserDataAttribute_t {
	struct {
		/* FW status */
		enum eUserDataAttrService sf:3;
		uint8_t backFront:1;
		uint8_t reserved:4;
	};
	uint8_t u8_raw;
};

/**
 * @brief structure defining one element of the TX fifo
 */
struct sUserDataTxFifoElt_t {
	uint16_t u16DataBitLen;
	/** @note As user data buf can contain ASCII-HEX data string:
	 * * multiply length by 2
	 * * add 1 extra for end-of-string ('\0')
	 */
	uint8_t u8DataBuf[(USERDATA_TX_PAYLOAD_MAX_SIZE * 2) + 1];
	bool bIsToBeTransmit;	/**< used as status variable to tell buffer contains valid data or
				  * not, meaning data needs to be transmitted over the air.
				  */
	union sUserDataAttribute_t u8Attr;
};

/* Variables -----------------------------------------------------------------*/

extern struct sUserDataTxFifoElt_t sUserDataTx;

#pragma GCC visibility pop

#endif /* __USER_DATA_H */

/**
 * @}
 */
