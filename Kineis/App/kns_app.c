// SPDX-License-Identifier: no SPDX license
/**
 * @file    kns_app.h
 * @brief   provide main application softwares using kineis sw satck
 * @author  Kinéis
 */

/**
 * @addtogroup KNS_APP
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "kns_q.h"
#include "kns_mac.h"
#include "mgr_at_cmd.h"
#include "mgr_log.h"

#ifdef USE_TX_LED // Light on a GPIO when TX occurs
#include "main.h"
#endif

/* Private macro -------------------------------------------------------------*/

/** Comment below to avoid 'TEST' status primitives to be logged
 */
#define PRINT_TEST_ASSERT

#ifdef PRINT_TEST_ASSERT
#define TEST_ASSERT(expr) ((expr) ? (void)0U : KNS_APP_assert_failed((uint8_t *)__FILE__, __LINE__))
#define TEST_PASS(...) MGR_LOG_DEBUG("==== TEST PASS ====\r\n")
#define TEST_FAIL(...) MGR_LOG_DEBUG("==== TEST FAIL ====\r\n")
#else
#define TEST_ASSERT(expr) ((expr) ? (void)0U : (void)0U)
#define TEST_PASS(...)
#define TEST_FAIL(...)
#endif

// defines used to check compilation flag settings KNS_TX_MOD (numerical values are dummy)
#define LDA2 0x22
#define LDK 0x33
#define VLDA4 0x44
#define LDA2L 0x55

/* Private variables ---------------------------------------------------------*/

static bool isCbReached;
static enum KNS_status_t cbStatus = KNS_STATUS_ERROR;

/* Private functions ----------------------------------------------------------*/

/** @brief  Set/clear a GPIO aroung transmission
 *
 * @note It is assumed a GPIO named LED1 is defined. Compile with USE_TX_LED to call STM32 HAL APIs
 *
 * @param[in] state: 1 set gpio, 0 clear GPIO
 */
#ifdef USE_TX_LED // Light on a GPIO when TX occurs
static void Set_TX_LED(uint8_t state)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, state);
}
#else
static void Set_TX_LED(__attribute__((unused)) uint8_t state)
{
}
#endif

#ifdef PRINT_TEST_ASSERT
/** @brief  Reports the name of the source file and the source line number where assert occured
 *
 * @param[in] file: pointer to the source file name
 * @param[in] line: assert_param error line source number
 */
static void KNS_APP_assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 * ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	 *
	 * So far, default logger used bythe kineis sw stack is used
	 */
	MGR_LOG_DEBUG("ASSERT FAIL: %lu %s\r\n", line, file);
	TEST_FAIL();
}
#endif

/** @brief Function used to process kineis expected kineis stack event instandalone APP.
 *
 * @retval KNS_STATUS_OK if TX DONE or KNS_STATUS_TIMEOUT if timeout reached, else KNS_STATUS_ERROR
 */
static enum KNS_status_t KNS_APP_macEvtProcess(void)
{
	enum KNS_status_t status;
	struct KNS_MAC_srvcEvt_t srvcEvt;

	status = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt);

	if (status == KNS_STATUS_OK) {
		switch (srvcEvt.id) {
		case (KNS_MAC_TX_DONE):
			MGR_LOG_DEBUG("[%s] TX done\r\n", __func__);
			Set_TX_LED(0);
			status = KNS_STATUS_OK;
			break;
		case (KNS_MAC_TX_TIMEOUT):
			MGR_LOG_DEBUG("[%s] TX timeout\r\n", __func__);
			status = KNS_STATUS_TIMEOUT;
			break;
		case (KNS_MAC_OK):
			MGR_LOG_DEBUG("[%s] OK from MAC layer\r\n", __func__);
			status = KNS_STATUS_ERROR;
			break;
		case (KNS_MAC_ERROR):
			MGR_LOG_DEBUG("[%s] ERROR from MAC layer\r\n", __func__);
			status = KNS_STATUS_ERROR;
			break;
		default:
			/* Any other event than above should not occur in current standalone app
			 *
			 */
			status = KNS_STATUS_ERROR;
			break;
		}
		cbStatus = status;
		isCbReached = true;
	}

	return status;
}

/** @brief At end of standalone APP test, generate TEST status
 *
 * @return true if data is correctly processed, false otherwise
 */

/* Public functions ----------------------------------------------------------*/

void KNS_APP_stdalone(void)
{
	uint8_t idx;
	struct KNS_MAC_appEvt_t appEvt;

	/** static variables for state machine */
	static uint8_t state;
	static uint8_t one_sec_delay_idx;

#if KNS_TX_MOD == LDA2
	//const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33 };
	const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
					0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33,
					0x44, 0x55, 0x66, 0x77};
#elif KNS_TX_MOD == LDA2L
	//const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33 };
	const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
					0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33,
					0x44, 0x55, 0x66, 0x77, 0x88};
#elif KNS_TX_MOD == LDK
	const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
					0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, 0x33 };
#elif KNS_TX_MOD == VLDA4
	const uint8_t buffer_tx[] = { 0xC0, 0xFF, 0xE5 };
#else
#error configure KNS_TX_MOD compilation flag with LDA2, LDA2L, LDK, VLDA4
#endif


	switch (state) {
	case 0: /** Send data event */
		appEvt.id =  KNS_MAC_SEND_DATA;
		for (idx = 0; idx < sizeof(buffer_tx); idx++)
			appEvt.data_ctxt.usrdata[idx] = buffer_tx[idx];
		appEvt.data_ctxt.usrdata_bitlen = sizeof(buffer_tx) * 8;
#if KNS_TX_MOD == LDA2L
		appEvt.data_ctxt.usrdata_bitlen -= 4; //> A2 Legacy = 196 bits = 24.5 bytes
#endif
		appEvt.data_ctxt.sf = KNS_SF_NO_SERVICE;

		TEST_ASSERT(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt) == KNS_STATUS_OK);
		Set_TX_LED(1);

		state++; /* go to next state, and return to let higher task to process event */
		return;
	case 1: { /** Wait for Kineis stack replying OK to send frame */
		enum KNS_status_t status;
		struct KNS_MAC_srvcEvt_t srvcEvt;

		status = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt);

		if (status == KNS_STATUS_OK) {
			switch (srvcEvt.id) {
			case (KNS_MAC_OK):
				MGR_LOG_DEBUG("[%s] TX OK for sending\r\n", __func__);
				state++; /* go to next state, and let higher task process event */
				return;
			default:
				TEST_FAIL();
				state++; /* go to next state, and let higher task process event */
				return;
			}
		}
		return;
	}
	case 2: { /** Wait for TX-done event from Kineis stack */
		enum KNS_status_t status;
		struct KNS_MAC_srvcEvt_t srvcEvt;

		status = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt);

		if (status == KNS_STATUS_OK) {
			switch (srvcEvt.id) {
			case (KNS_MAC_TX_DONE):
				MGR_LOG_DEBUG("[%s] TX done\r\n", __func__);
				Set_TX_LED(0);
				TEST_PASS();
				one_sec_delay_idx = 0;
				state++; /* go to next state, and let higher task process event */
				return;
			case (KNS_MAC_TX_TIMEOUT):
				MGR_LOG_DEBUG("[%s] TX timeout\r\n", __func__);
				TEST_FAIL();
				one_sec_delay_idx = 0;
				state++; /* go to next state, and let higher task process event */
				return;
			case (KNS_MAC_ERROR):
				MGR_LOG_DEBUG("[%s] ERROR from MAC layer\r\n", __func__);
				TEST_FAIL();
				one_sec_delay_idx = 0;
				state++; /* go to next state, and let higher task process event */
				return;
			default:
				TEST_FAIL();
				one_sec_delay_idx = 0;
				state++; /* go to next state, and let higher task process event */
				return;
			}
		}
		return;
	}
	case 3: /** active wait during almost 60s */
		for (; one_sec_delay_idx < 58; one_sec_delay_idx++) {
			HAL_Delay(1000);
			if (KNS_Q_isEvtInHigherPrioQ(KNS_Q_UL_MAC2APP))
				return;
		}
		state = 0;
		one_sec_delay_idx = 0;
		return;
	default:
		break;
		assert_param(0);
	}
}

void KNS_APP_stdalone_stressTest(void)
{
	uint8_t idx;
	struct KNS_MAC_appEvt_t appEvt;
	struct KNS_MAC_srvcEvt_t appEvtDummy;

	/** static variables for state machine */
	static uint8_t state;
	static uint8_t one_sec_delay_idx;

#if KNS_TX_MOD == LDA2
	//const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33 };
	const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
					0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33,
					0x44, 0x55, 0x66, 0x77};
#elif KNS_TX_MOD == LDA2L
	//const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33 };
	const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
					0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33,
					0x44, 0x55, 0x66, 0x77, 0x88};
#elif KNS_TX_MOD == LDK
	const uint8_t buffer_tx[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
					0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, 0x33 };
#elif KNS_TX_MOD == VLDA4
	const uint8_t buffer_tx[] = { 0xC0, 0xFF, 0xE5 };
#else
#error configure KNS_TX_MOD compilation flag with LDA2, LDA2L, LDK, VLDA4
#endif


	switch (state) {
	case 0: /** Send data event */
		appEvt.id =  KNS_MAC_SEND_DATA;
		for (idx = 0; idx < sizeof(buffer_tx); idx++)
			appEvt.data_ctxt.usrdata[idx] = buffer_tx[idx];
		appEvt.data_ctxt.usrdata_bitlen = sizeof(buffer_tx) * 8;
#if KNS_TX_MOD == LDA2L
		appEvt.data_ctxt.usrdata_bitlen -= 4; //> A2 Legacy = 196 bits = 24.5 bytes
#endif
		appEvt.data_ctxt.sf = KNS_SF_NO_SERVICE;

		TEST_ASSERT(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt) == KNS_STATUS_OK);
		Set_TX_LED(1);

		state++; /* go to next state, and return to let higher task to process event */
		return;
	case 1:
		/** ---- Pop OK event from MAC ---- */
		appEvtDummy.id = KNS_MAC_TX_DONE; /* Initialize with a dummy value */
		TEST_ASSERT(KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&appEvtDummy) == KNS_STATUS_OK);
		TEST_ASSERT(appEvtDummy.id == KNS_MAC_OK);
		state++; /* go to next state, and return to let higher task to process event */
		//return;
	case 2: /** Try sending data again but should lead to Error as transmission already
		  * triggerred in previous state
		  */
		appEvt.id =  KNS_MAC_SEND_DATA;
		for (idx = 0; idx < sizeof(buffer_tx); idx++)
			appEvt.data_ctxt.usrdata[idx] = buffer_tx[idx];
		appEvt.data_ctxt.usrdata_bitlen = sizeof(buffer_tx) * 8;
#if KNS_TX_MOD == LDA2L
		appEvt.data_ctxt.usrdata_bitlen -= 4; //> A2 Legacy = 196 bits = 24.5 bytes
#endif
		appEvt.data_ctxt.sf = KNS_SF_NO_SERVICE;

		TEST_ASSERT(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt) == KNS_STATUS_OK);
		state++; /* go to next state, and return to let higher task to process event */
		return;
	case 3:
		/** ---- Pop error event from MAC ---- */
		appEvtDummy.id = KNS_MAC_TX_DONE; /* Initialize with a dummy value */
		TEST_ASSERT(KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&appEvtDummy) == KNS_STATUS_OK);
		TEST_ASSERT(appEvtDummy.id == KNS_MAC_ERROR);
		state++; /* go to next state, and return to let higher task to process event */
		//return;
	case 4:
	case 5:
	case 6:
		/** Wait after end-of-transmission callback, to raise test status,
		 * return each second to let higher-prio task to execute
		 * Wait 3s at maximum (3 states)
		 */
		HAL_Delay(1000); /* wait callback to be reached while 1s */
		KNS_APP_macEvtProcess();
		state++; /* go to next state, and return to let higher task to process event */
		return;
	case 7:
		/** ---- Free element once again, should report error as already inetrnally free
		 * by lower layer at TX-done event ----
		 */
		appEvt.id =  KNS_MAC_STOP_SEND_DATA;
		TEST_ASSERT(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt) == KNS_STATUS_OK);
		state++; /* go to next state, and return to let higher task to process event */
		return;
	case 8:
		/** ---- Pop error event from MAC ---- */
		appEvtDummy.id = KNS_MAC_TX_DONE; /* Initialize with a dummy value */
		TEST_ASSERT(KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&appEvtDummy) == KNS_STATUS_OK);
		TEST_ASSERT(appEvtDummy.id == KNS_MAC_ERROR);
		state++; /* go to next state, and return to let higher task to process event */
		//return;
	case 9: /** check test passed, meaning TX-done event was received */
		if (!isCbReached || (cbStatus != KNS_STATUS_OK))
			TEST_FAIL();
		else
			TEST_PASS();
		isCbReached = false;
		cbStatus = KNS_STATUS_ERROR;
		state++; /* go to next state, and return to let higher task to process event */
		return;
	default:
		break;
		assert_param(0);
	}

	/** Wait before new transmit: reduce by 3s as KNS_APP_waitEndOfTx includes some.
	 * Check higher-prio-task needs preemption each second.
	 *
	 * wait 57s as we want to transmit each 60s seconds
	 */
	for (; one_sec_delay_idx < 57; one_sec_delay_idx++) {
		HAL_Delay(1000);
		TEST_ASSERT(KNS_APP_macEvtProcess() == KNS_STATUS_QEMPTY);
		if (KNS_Q_isEvtInHigherPrioQ(KNS_Q_UL_MAC2APP))
			return;
	}

	/** Return to start of loop */
	state = 0;
	one_sec_delay_idx = 0;
}

void KNS_APP_gui(void)
{
	uint8_t *pu8_atcmd = NULL;

	/** ---- Look for AT cmds ---- */
	pu8_atcmd = MGR_AT_CMD_getNextAt();
	if (pu8_atcmd != NULL)
		MGR_AT_CMD_decodeAt(pu8_atcmd);
	MGR_AT_CMD_macEvtProcess();
}

/**
 * @}
 */
