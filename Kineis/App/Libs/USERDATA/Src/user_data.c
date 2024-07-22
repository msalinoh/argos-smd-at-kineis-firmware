// SPDX-License-Identifier: no SPDX license
/**
 * @file    user_data.c
 * @brief   Library to handle user data to be be received or transmitted over the air
 * @note
 */

/**
 * @addtogroup USER_DATA
 * @{
 */

/* Includes ------------------------------------------------------------------------------------ */

#include <stdbool.h>
#include <stddef.h>
#include "kns_cs.h"
#include "user_data.h"
#include "kineis_sw_conf.h"  // for assert include below
#include KINEIS_SW_ASSERT_H

//#undef VERBOSE // FIFO verbose log disabled by default as too verbose.
#include "mgr_log.h" /* @note This log is for debug, can be deleted */

/* Struct -------------------------------------------------------------------------------------- */

struct sUserDataTxFifo_t {
	struct sUserDataTxFifoElt_t *spFirst; /**< fifo start pointer */
};

struct sUserDataRx_t {
	uint8_t *const pu8UserDataRx; /**< user data rx pointer */
	uint16_t u16UserDataRxBytelen; /**< user data rx length in Bytes */
};

/* Variables ----------------------------------------------------------------------------------- */

#ifdef USE_USERDATA_TX
/* default values of TX fifo elements */
const struct sUserDataTxFifoElt_t sUserDataTxEltDflt =  {
		.u8DataBuf = {0},
		.bIsToBeTransmit = false,
		.u8Attr.u8_raw = 0x00,
		.u16DataBitLen = 0,
		//.sRatCtrl = {0}, //.sRatCtrl will be initialized by calling client's callbacks
		.spNext = NULL
};

static
__attribute__((__section__(".retentionRamData")))
struct sUserDataTxFifoElt_t sUserDataTxFifoBuf[USERDATA_TX_FIFO_SIZE] = {sUserDataTxEltDflt};

static
__attribute__((__section__(".retentionRamData")))
struct sUserDataTxFifo_t sUserDataTxFifo = {
		.spFirst  = NULL,
};
#endif /* USE_USERDATA_TX */

#ifdef USE_USERDATA_RX
static uint8_t au8_rxpayload[USERDATA_RX_PAYLOAD_MAX_SIZE] = {0};

static struct sUserDataRx_t s_userdatarx = {
		.pu8UserDataRx = &(au8_rxpayload[USERDATA_DFT_POS_ON_RX_PAYLOAD]),
		.u16UserDataRxBytelen = 0
};
#endif /* USE_USERDATA_RX */

/* Local functions ----------------------------------------------------------------------------- */

static bool USERDATA_txFifoIsInBuf(struct sUserDataTxFifoElt_t *spElt);

/* Public functions ---------------------------------------------------------------------------- */

#ifdef USE_USERDATA_TX

    /* ---- TX functions ---- */

/**
 * @brief Log content of the TX fifo
 *
 * It will print address of each element for top to bottom.
 */
static void USERDATA_txFifoLog(void)
{
	struct sUserDataTxFifoElt_t *spTxFifoElt = sUserDataTxFifo.spFirst;

	/* make fifo log atomic, by putting all code under critical section */
	KNS_CS_enter();

	MGR_LOG_VERBOSE("USERDATA: TX FIFO:");
	/* From top of the fifo, log address of each element */
	for (spTxFifoElt = sUserDataTxFifo.spFirst;
	     spTxFifoElt != NULL;
	     spTxFifoElt = spTxFifoElt->spNext){
		MGR_LOG_VERBOSE_RAW(" 0x%x", spTxFifoElt);
	}
	MGR_LOG_VERBOSE_RAW(":\r\n");

	/* Enable interrupts back only if they were enabled before we disabled it in this fct */
	KNS_CS_exit();
}

/**
 * @brief check the element is pointing on a valid address, i.e. one from sUserDataTxFifoBuf buffer
 *
 * @param[in] spElt pointer to the element
 *
 * @return true if element is in the buff, false otherwise
 */
static bool USERDATA_txFifoIsInBuf(struct sUserDataTxFifoElt_t *spElt)
{
	uint8_t eltIdx;

	/* search element */
	for (eltIdx = 0;
	     eltIdx < USERDATA_TX_FIFO_SIZE &&
	     &sUserDataTxFifoBuf[eltIdx] != spElt;
	     eltIdx++)
		/* empty for loop */
		;

	if (eltIdx == USERDATA_TX_FIFO_SIZE)
		return false;
	else
		return true;
}

struct sUserDataTxFifoElt_t *USERDATA_txFifoReserveElt(void)
{
	uint8_t eltIdx;
	struct sUserDataTxFifoElt_t *spFreeElt = NULL;

	/* search some free element */
	for (eltIdx = 0;
	     eltIdx < USERDATA_TX_FIFO_SIZE &&
	     sUserDataTxFifoBuf[eltIdx].bIsToBeTransmit == true;
	     eltIdx++)
		/* empty for loop */
		;

	if (eltIdx < USERDATA_TX_FIFO_SIZE) {
		spFreeElt = &sUserDataTxFifoBuf[eltIdx];

		/* check elt is not already part of the fifo */
		kns_assert(!USERDATA_txFifoIsEltInFifo(spFreeElt));

		/* tag as reserved, caller will have to add it in fifo after fill-up */
		*spFreeElt = sUserDataTxEltDflt;
		spFreeElt->bIsToBeTransmit = true;
	}

	MGR_LOG_VERBOSE("USERDATA: TX FIFO: reserve 0x%x\r\n", spFreeElt);

	return spFreeElt;
}


#pragma GCC push_options
#pragma GCC optimize("O0")
bool USERDATA_txFifoAddElt(struct sUserDataTxFifoElt_t *spEltToAdd, bool bIsAddBack)
{
	uint8_t u8IdxClient;
	struct sUserDataTxFifoElt_t *spTxFifoElt = sUserDataTxFifo.spFirst;

	if (USERDATA_txFifoIsInBuf(spEltToAdd) == false)
		return false;

	/* check elt is not already part of the fifo */
	if (USERDATA_txFifoIsEltInFifo(spEltToAdd) == true)
		return false;

	/* \note "u8IdxClient + 1 <= USERDATA_RAT_MAX" condition is used instead of
	 *       "u8IdxClient < USERDATA_RAT_MAX" to avoid comparison-is-always-false compilation
	 *       error when there is no clients at all
	 *
	 */
	for (u8IdxClient = 0; u8IdxClient + 1 <= USERDATA_RAT_MAX; u8IdxClient++)
		if (sUserDataClientCb[u8IdxClient].USERDATA_txFifoAddEltCb != NULL)
			sUserDataClientCb[u8IdxClient].USERDATA_txFifoAddEltCb(spEltToAdd);

	/* if no element in FIFO  or add in front */
	if (spTxFifoElt == NULL || !bIsAddBack) {
		spEltToAdd->spNext = sUserDataTxFifo.spFirst;
		sUserDataTxFifo.spFirst = spEltToAdd;
	} else {
		/* add in back of the TX Fifo, go to last element */
		for (;
		     spTxFifoElt->spNext != NULL;
		     spTxFifoElt = spTxFifoElt->spNext)
			/* empty for loop */
			;

		/* \note coding like below will only execute second instruction because of compiler
		 * optimization:
		 *     spTxFifoElt->spNext = spEltToAdd;
		 *     spTxFifoElt->spNext = NULL;
		 * Need to code like below then
		 */
		 spEltToAdd->spNext = NULL;
		 spTxFifoElt->spNext = spEltToAdd;
	}

	USERDATA_txFifoLog();

	return true;
}
#pragma GCC pop_options

bool USERDATA_txFifoRemoveElt(struct sUserDataTxFifoElt_t *spEltToRemove)
{
	uint8_t u8IdxClient;
	struct sUserDataTxFifoElt_t *spTxFifoElt = sUserDataTxFifo.spFirst;

	if (USERDATA_txFifoIsInBuf(spEltToRemove) == false)
		return false;

	/* \note "u8IdxClient + 1 <= USERDATA_RAT_MAX" condition is used instead of
	 *       "u8IdxClient < USERDATA_RAT_MAX" to avoid comparison-is-always-false compilation
	 *       error when there is no clients at all
	 *
	 */
	for (u8IdxClient = 0; u8IdxClient + 1 <= USERDATA_RAT_MAX; u8IdxClient++)
		if (sUserDataClientCb[u8IdxClient].USERDATA_txFifoTxCompleteCb != NULL)
			kns_assert(sUserDataClientCb[u8IdxClient].USERDATA_txFifoTxCompleteCb(
				spEltToRemove));

	/* Free element from the memory buffer */
	spEltToRemove->bIsToBeTransmit = false;

	/* Remove element from TX fifo if present: */
	/* - if no element in FIFO, exit */
	if (spTxFifoElt == NULL)
		return false;
	/* - check element is the first one or not */
	if (spTxFifoElt == spEltToRemove) {
		sUserDataTxFifo.spFirst = spEltToRemove->spNext;
		MGR_LOG_VERBOSE("USERDATA: TX FIFO: remove 0x%x\r\n", spEltToRemove);
		USERDATA_txFifoLog();
		return true;
	}
	/* - From top of the fifo, search the element to be removed */
	for (;
	     spTxFifoElt->spNext != NULL && spTxFifoElt->spNext != spEltToRemove;
	     spTxFifoElt = spTxFifoElt->spNext)
		/* empty for loop */
		;
	/* - if element found in list */
	if (spTxFifoElt->spNext == spEltToRemove) {
		spTxFifoElt->spNext = spEltToRemove->spNext;
		MGR_LOG_VERBOSE("USERDATA: TX FIFO: remove 0x%x\r\n", spEltToRemove);
		USERDATA_txFifoLog();
		return true;
	}

	return false;
}

bool USERDATA_txFifoIsEltInFifo(struct sUserDataTxFifoElt_t *spEltToFind)
{
	struct sUserDataTxFifoElt_t *spTxFifoElt = sUserDataTxFifo.spFirst;

	if (USERDATA_txFifoIsInBuf(spEltToFind) == false)
		return false;

	/* - if no element in FIFO, exit */
	if (spTxFifoElt == NULL)
		return false;
	/* - check element is the first one or not */
	if (spTxFifoElt == spEltToFind)
		return true;
	/* - From top of the fifo, search the element to be removed */
	for (;
	     spTxFifoElt->spNext != NULL && spTxFifoElt->spNext != spEltToFind;
	     spTxFifoElt = spTxFifoElt->spNext)
		/* empty for loop */
		;
	/* - if element found in list */
	if (spTxFifoElt->spNext == spEltToFind)
		return true;

	return false;
}

uint8_t USERDATA_txFifoGetCount(void)
{
	uint8_t u8EltCnt;
	struct sUserDataTxFifoElt_t *spTxFifoElt = sUserDataTxFifo.spFirst;

	/* From top of the fifo, search the element pointing on NULL */
	for (u8EltCnt = 0;
	     spTxFifoElt != NULL;
	     spTxFifoElt = spTxFifoElt->spNext, u8EltCnt++)
		/* empty for loop */
		;

	MGR_LOG_VERBOSE("USERDATA: TX FIFO contains %d elt\r\n", u8EltCnt);

	kns_assert(u8EltCnt <= USERDATA_TX_FIFO_SIZE);

	return u8EltCnt;
}

bool USERDATA_txFifoFlush(void)
{
	uint8_t u8IdxClient;
	struct sUserDataTxFifoElt_t *spTxFifoElt;

	/* make fifo flush atomic, by putting all code under critical section */
	KNS_CS_enter();

	/* \note "u8IdxClient + 1 <= USERDATA_RAT_MAX" condition is used instead of
	 *       "u8IdxClient < USERDATA_RAT_MAX" to avoid comparison-is-always-false compilation
	 *       error when there is no clients at all
	 *
	 */
	for (u8IdxClient = 0; u8IdxClient + 1 <= USERDATA_RAT_MAX; u8IdxClient++)
		if (sUserDataClientCb[u8IdxClient].USERDATA_txFifoFlushCb != NULL)
			kns_assert(sUserDataClientCb[u8IdxClient].USERDATA_txFifoFlushCb());

	spTxFifoElt = sUserDataTxFifo.spFirst;
	sUserDataTxFifo.spFirst = NULL;

	/* From top of the fifo, free each element */
	for (; spTxFifoElt != NULL; spTxFifoElt = spTxFifoElt->spNext)
		spTxFifoElt->bIsToBeTransmit = false;

	/* Enable interrupts back only if they were enabled before we disabled it in this fct */
	KNS_CS_exit();

	USERDATA_txFifoLog();

	kns_assert(USERDATA_txFifoGetCount() == 0);

	return true;
}

struct sUserDataTxFifoElt_t *USERDATA_txFifoGetFirst()
{
	return sUserDataTxFifo.spFirst;
}

struct sUserDataTxFifoElt_t *USERDATA_txFifoFindPayload(uint8_t *data, uint16_t bitlen)
{
	struct sUserDataTxFifoElt_t *spTxFifoElt;
	uint16_t dataIdx;
	uint8_t mask;

	/* - From top of the fifo, search the element to be removed */
	for (spTxFifoElt = sUserDataTxFifo.spFirst;
	     spTxFifoElt != NULL;
	     spTxFifoElt = spTxFifoElt->spNext) {
		if (spTxFifoElt->u16DataBitLen != bitlen)
			continue;
		if (spTxFifoElt->bIsToBeTransmit == false)
			continue;
		for (dataIdx = 0;
			(dataIdx < (bitlen/8)) &&
			(spTxFifoElt->u8DataBuf[dataIdx] == data[dataIdx]) ;
			dataIdx++)
			; // empty loop
		if (dataIdx != (bitlen/8))
			continue;
		/* check remaining bits of last byte if any */
		if (bitlen % 8) {
			mask = (1 << (bitlen % 8)) - 1;    // 7: 0b01111111, 1: 0b00000001
			mask = mask << (8 - (bitlen % 8)); // 7: 0b11111110, 1: 0b10000000
			if ((spTxFifoElt->u8DataBuf[bitlen % 8] & mask) !=
			    (data[bitlen % 8] & mask))
				continue;
		}
		return spTxFifoElt;
	}
	return NULL;
}

#endif /* USE_USERDATA_TX */

#ifdef USE_USERDATA_RX

    /* ---- RX functions ---- */

uint8_t *USERDATA_GetUserDataRxPtr(void)
{
	return s_userdatarx.pu8UserDataRx;
}

uint16_t USERDATA_GetUserDataRxByteLen(void)
{
	return s_userdatarx.u16UserDataRxBytelen;
}

void USERDATA_SetUserDataRxByteLen(uint16_t u8_userdatarx_bytelen)
{
	/* Probably not as per design to set a byte length which is larger than supported size.
	 *
	 * \todo As a recovery procedure, may limit size to buffer size instead of assert.
	 */
	kns_assert(u8_userdatarx_bytelen <= sizeof(au8_rxpayload));
	s_userdatarx.u16UserDataRxBytelen = u8_userdatarx_bytelen;
}

#endif /* USE_USERDATA_RX */

/**
 * @}
 */
