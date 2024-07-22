/* SPDX-License-Identifier: no SPDX license */
/**
 * @file   user_data.h
 * @brief  Library to handle user data to be be received or transmitted over the air through
 *         several networks (e.g.. Kineis, LORA, ...). this is mainly implementing a FIFO.
 * @author Kinéis
 */

/**
 * @page user_data_page User data Library
 *
 * This page is presenting the USER DATA Library.
 *
 * This software library handles user data. It is a central component in charge to:
 * * get free memory and store user data coming from high layers (AT commands, sensors, ...)
 * * provide it to several lower RF layer which will transmit it over the air (e.g. ARGOS, LoRa,
 * NB-IOT, ...).
 *
 * Typically, the higher layer can be an AT command manager and the lower layer can be some network
 * manager (NW MGR) in charge to transmit and receive data over the air. In case of hybridation,
 * the lower layer could be a network manager which will decide on which network it is worth to
 * transmit first.
 *
 * @section user_data_design_apis APIs
 *
 * The APIs proposed by this library are about to:
 * * add a new message at the beginning or at the end of the FIFO
 * * remove an element
 * * count the number of message already located in the FIFO
 * * count the remaining free place in FIFO
 * * get next free memory element
 * * get first element of the FIFO
 * * flush the entire FIFO
 * * Retrieve an element of the FIFO
 *
 * When an element is transmitted, the lower layer needs to notify the upper layers.
 * For each message, the upper layer has to register a notification callback, so that the lower
 * layer can call it. So far, it is assumed there is only one notification callback.
 *
 * On an other hand, USERDATA can notify the lower layers when som events occurs on the fifo, e.g.:
 * * an element is added in FIFO
 * * an element is removed from fifo
 * * the fifo is flushed
 * This allows lower layers to maintain their internal control code and status code.
 *
 * @section user_data_design_points Design/usage highlights
 *
 * @subsection user_data_design_point_cl Chained list
 *
 * Depending on the radio link quality, some message added first in FIFO may not be received first
 * on satellite, at first transmission. Some message will need several retransmission, some other
 * will not. Thus, some later-added messages may be removed first from the FIFO.
 *
 * For that purpose, the fifo is implemented as a chained list. A pointer to first element is
 * available. Each element contains a pointer to next element. The last element of the fifo
 * is point is pointing to NULL.
 *
 * As each message is independent, the FIFO is internally designed as a chained list.
 *
 * @subsection user_data_design_point_cs Critical sections
 *
 * As this library is accessed by several software entities with different priorities, some
 * critical sections may be needed in this library for data protection against dual access.
 * Nevertheless, it is possible to avoid too much critical sections by clearly identify and
 * separate variables in a way to get only one writer-reader path per variable.
 *
 * Here is some examples:
 * * needs_transmit:
 *     * write 0->1 by APP only
 *     * write 1->0 by APP only, after NW MGR sent notification to APP
 * * tx_rat: write by NW MGR only
 * * nb_transmit:
 *     * write 0->x by NW MGR only
 *     * write x->0 by RAT MGR only
 *
 * @subsection user_data_design_point_es Element size
 *
 * Depending on transmission protocol (KINEIS, NB-IOT, ...), the user data length can be different.
 * Optimizing the memory resource with dynamic user data size in a chained list is difficult.
 *
 * At first step, the data buffer will be an array where each element as the maximum size allowed
 * among all protocols.
 *
 * In next design iterations, two or three data size may be considered (e.g. bigger buffer for HDA4
 * data). This will lead the higher layer to reserve place in a buffer fitting its user-data length.
 */

/**
 * @addtogroup USER_DATA
 * @brief  User data Library. (refer to \ref user_data page for general description).
 * @{
 */

#ifndef USER_DATA_H
#define USER_DATA_H

/* Includes ------------------------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>

#pragma GCC visibility push(default)

/* Defines ------------------------------------------------------------------------------------- */
/**
 * @brief Maximum number of bytes of user data field
 *
 * On Kineis RAT, the maximum size is 196 bits which is 24.5 bytes. Round it to 25.
 *
 * @note So far MGR_AT_CMD makes the FIFO to contain the USER data as ASCII-HEX string. Thus:
 * * multiply length by 2
 * * add 1 extra for end-of-string ('\0')
 */
#ifndef USERDATA_TX_DATAFIELD_SIZE
#define USERDATA_TX_DATAFIELD_SIZE 25
#endif

#ifndef USERDATA_TX_PAYLOAD_MAX_SIZE
#define USERDATA_TX_PAYLOAD_MAX_SIZE ((USERDATA_TX_DATAFIELD_SIZE * 2) + 1)
#endif

#if (USERDATA_TX_DATAFIELD_SIZE < 25)
#error "USERDATA TX FIFO MAX SIZE does not support maximum payload size for KINEIS network"
#endif

#ifndef USERDATA_RX_PAYLOAD_MAX_SIZE
#define USERDATA_RX_PAYLOAD_MAX_SIZE	100
#endif
#ifndef USERDATA_DFT_POS_ON_RX_PAYLOAD
#define USERDATA_DFT_POS_ON_RX_PAYLOAD	0
#endif

/**< Max TX fifo size  */
#ifndef USERDATA_TX_FIFO_SIZE
#define USERDATA_TX_FIFO_SIZE		4
#endif

/* Enums --------------------------------------------------------------------------------------- */

/**
 * @brief enum defining service field of the user data attribute
 */
enum eUserDataAttrService {
	ATTR_NONE           = 0,
	ATTR_MAIL_REQUEST   = 1, /**< user expects to request if some message is for him */
	ATTR_UNICAST_ACK    = 2, /**< reserved, internal usage */
	ATTR_MULTICAST_ACK  = 3, /**< reserved, internal usage */
	ATTR_PACK           = 4, /**< user request some ACKnowledgment for this msg*/
	ATTR_PACK_EMERGENCY = 5  /**< user request some ACKnowledgment for this emergency msg*/
};

/**
 * @brief structure containing USERDATA attribute
 *
 * This attribute is set by user, such message is important or not. It may impact lower layer
 * strategies to transmit the message. So far, attribute is defined as:
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
 * @brief enum listing RAT using the USERDATA fifo
 *
 * @note This type is set as weak, so that it can be overwritten outside the library (e.g. in
 * application code). The content here is just for example.
 */
__attribute((__weak__))
enum USERDATA_RAT_T {
	/* \todo add hybrid RAT enum here (e.g. LoRa, NB_IOT, ) */
#ifdef USE_USERDATA_KINEIS
	USERDATA_RAT_KINEIS,
#endif
#ifdef USE_USERDATA_LORA
	USERDATA_RAT_LORA,
#endif
	USERDATA_RAT_MAX
};

/* Struct--------------------------------------------------------------------------------------- */

/**
 * @brief struct for general control/status struct of USERDATA clients
 *
 * The content of this structure is for USERDATA's RAT clients to maintain some info regarding fifo
 * elements. Any RAT (e.g. ARGOS, Kineis, LoRa, Sigfox, NB-IOT) may have its own control code struct
 * added inside this struct.
 *
 * @note This type is set as weak, so that it can be overwritten outside the library (e.g. in
 * application code). The content here is just for example.
 */
__attribute((__weak__))
struct sUserDataTxFifoRatCtrl_t {
#ifdef USE_USERDATA_KINEIS
	struct kineisRat_t kineisRat;
#endif
#ifdef USE_USERDATA_LORA
	struct loraRat_t loraRat;
#endif
	/* \todo add hybrid RAT ctrl structures here (e.g. LoRa, NB_IOT, ) */
};

/**
 * @brief structure defining one element of the TX fifo
 */
struct sUserDataTxFifoElt_t {
	uint8_t u8DataBuf[USERDATA_TX_PAYLOAD_MAX_SIZE];
	bool bIsToBeTransmit;  /** when true, element is reserved, ready to be added in fifo when
				* correctly filled-up
				*/
	union sUserDataAttribute_t u8Attr;
	uint16_t u16DataBitLen;
	struct sUserDataTxFifoRatCtrl_t sRatCtrl; /**< struct w/ ctrl info from RAT managers */
	struct sUserDataTxFifoElt_t *spNext; /**< pointer to next element of the chained list */
};

/**
 * @brief structure containing USERDATA client callbacks. It is used to notify client from events
 * occurring at USERDATA layer
 *
 * Each RAT using USERDATA fifo for TX should implement such callback in a way to handle their
 * internal control code regarding FIFO
 *
 * It can be set to NULL if not needed
 */
struct sUserDataClientCb_t {
	 bool (*USERDATA_txFifoAddEltCb)(struct sUserDataTxFifoElt_t *spElt); /**< elt added */
	 bool (*USERDATA_txFifoTxCompleteCb)(struct sUserDataTxFifoElt_t *spElt); /**< TX done */
	 bool (*USERDATA_txFifoFlushCb)(void); /**< fifo flush triggered by someone */
};

/* Externs ------------------------------------------------------------------------------------- */

extern struct sUserDataClientCb_t sUserDataClientCb[];

/* Public functions ---------------------------------------------------------------------------- */

#ifdef USE_USERDATA_TX

    /* ---- TX functions ---- */

/**
 * @brief Get an element from memory pool in a way to push it later
 *
 * \note The found element is internally no more considered free after calling this function.
 * The caller will have to handle it later by adding the element into the fifo.
 * It possible to delete by calling API USERDATA_txFifoRemoveElt once the element is in the fifo.
 *
 * @return pointer on element, NULL if no more free element
 */
struct sUserDataTxFifoElt_t *USERDATA_txFifoReserveElt();

/**
 * @brief Add element in TX fifo
 *
 * @param[in] spEltToAdd pointer to the element to add
 * @param[in] bIsAddBack add elt at the back of the fifo if true, add it at the front if false
 *
 * @return true is success, false else
 */
bool USERDATA_txFifoAddElt(struct sUserDataTxFifoElt_t *spEltToAdd, bool bIsAddBack);

/**
 * @brief Delete an element from the TX fifo
 *
 * @param[in] spEltToRemove pointer to the element to be removed
 *
 * @return true when element is found and removed, false otherwise
 */
bool USERDATA_txFifoRemoveElt(struct sUserDataTxFifoElt_t *spEltToRemove);

/**
 * @brief check element is part of the TX fifo or not
 *
 * @param[in] spEltToFind pointer to the element to find
 *
 * @return true when element is found, false otherwise
 */
bool USERDATA_txFifoIsEltInFifo(struct sUserDataTxFifoElt_t *spEltToFind);

/**
 * @brief count number of elements in TX fifo
 *
 * @return number of elements in the fifo
 */
uint8_t USERDATA_txFifoGetCount(void);

/**
 * @brief flush TX fifo
 *
 * @return true if success, false otherwise
 */
bool USERDATA_txFifoFlush(void);

/**
 * @brief get first element of the TX Fifo
 *
 * @return pointer to the first element
 */
struct sUserDataTxFifoElt_t *USERDATA_txFifoGetFirst(void);

/**
 * @brief get first element which contains the expected payload
 *
 * @param[in] data pointer to dat payload
 * @param[in] bitlen length of the data payload in bits
 *
 * @return pointer to the first element
 */
struct sUserDataTxFifoElt_t *USERDATA_txFifoFindPayload(uint8_t *data, uint16_t bitlen);

#endif /* USE_USERDATA_TX */

#ifdef USE_USERDATA_RX

    /* ---- RX functions ---- */

/**
 * @brief Get user data RX pointer
 *
 * @return the user data Rx Pointer
 */
uint8_t *USERDATA_GetUserDataRxPtr(void);

/**
 * @brief Get current user data RX buffer length in Bytes
 *
 * @return the user data Rx buffer length in Bytes
 */
uint16_t USERDATA_GetUserDataRxByteLen(void);

/**
 * @brief Set user data RX length in Bytes
 *
 * @param[in] u8_userdatarx_bytelen: user data rx length in Bytes
 */
void USERDATA_SetUserDataRxByteLen(uint16_t u8_userdatarx_bytelen);

#endif /* USE_USERDATA_RX */

#pragma GCC visibility pop

#endif /* __USER_DATA_H */

/**
 * @}
 */
