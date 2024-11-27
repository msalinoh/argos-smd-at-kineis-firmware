// SPDX-License-Identifier: no SPDX license
/**
 * @file    kns_app.h
 * @brief   provide main application softwares using kineis sw satck
 * @author  Kineis
 */

/**
 * @addtogroup KNS_APP
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdlib.h>
#include "kns_q.h"
#include "kns_mac.h"
#include "kns_cfg.h"
#include "mgr_at_cmd.h"
#include "kineis_sw_conf.h"
#include "mcu_nvm.h"

#include KINEIS_SW_ASSERT_H
#include "mgr_log.h"

#ifdef USE_TX_LED // Light on a GPIO when TX occurs
#include "main.h"
#endif

/* Private macro -------------------------------------------------------------*/

/** Uncomment below depending on Kineis MAC protocol to be used by Kineis stack. */
#if !defined(USE_MAC_PRFL_BASIC) && !defined(USE_MAC_PRFL_BLIND)
//#define USE_MAC_PRFL_BASIC // immediate TX
#define USE_MAC_PRFL_BLIND // period TX for few repetitions
#endif

#if defined(USE_MAC_PRFL_BASIC) && defined(USE_MAC_PRFL_BLIND)
#error "USE_MAC_PRFL_BASIC/USE_MAC_PRFL_BLIND: Cannot build an APP which select two MAC profile at the same time, select only one."
#endif

/** Comment below to avoid 'TEST' status primitives to be logged */
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

/* Private variables ---------------------------------------------------------*/

#ifdef USE_MAC_PRFL_BLIND
/**
 * @attention Configuration below shall be discussed with Kineis according to the SAT constellation
 * deployment
 */
struct KNS_MAC_BLIND_usrCfg_t prflBlindUserCfg = {
	.retx_nb = 4,
	.retx_period_s = 60,	/** Repetition period per message
				 * @attention ensure retx_period_s =  60 * nb_parrallel_msg
				 */
	.nb_parrallel_msg = 1	/** User-Message FIFO size @attention do not exceed 4*/
};
#endif

/* Private functions ----------------------------------------------------------*/

#ifdef PRINT_TEST_ASSERT
/** @brief  Reports the name of the source file and the source line number where assert occured
 *
 * @param[in] file: pointer to the source file name
 * @param[in] line: kns_assert error line source number
 */
static void KNS_APP_assert_failed( __attribute__((unused)) uint8_t *file,
	__attribute__((unused)) uint32_t line)
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

__attribute__((unused))
/** @brief  Log array of uint8_t
 * @param[in] data pointer to table
 * @param[in] len number of bytes to log from the table
 */
static void MGR_LOG_array(__attribute__((unused)) uint8_t *data, uint16_t len)
{
	uint16_t i;

	for (i = 0; i < len; i++)
		MGR_LOG_DEBUG_RAW("%02X", data[i]);
	MGR_LOG_DEBUG_RAW("\r\n");
}
/** @brief At end of standalone APP test, generate TEST status
 *
 * @return true if data is correctly processed, false otherwise
 */

/* Public functions ----------------------------------------------------------*/

void KNS_APP_stdln_init(__attribute__((unused)) void *context)
{
	enum KNS_status_t status;

	kns_assert(context == NULL);

	/** Initialize Kineis MAC profile */
#ifdef USE_MAC_PRFL_BASIC
	status = KNS_MAC_init(KNS_MAC_PRFL_BASIC, NULL);
	if (status != KNS_STATUS_OK) {
		MGR_LOG_DEBUG("[ERROR] Cannot initialize MAC BASIC protocol: Error code 0x%x\r\n",
			status);
		MGR_LOG_DEBUG("[ERROR] Check protocol capabilities of the build\r\n");
		kns_assert(0);
	}
#endif
#ifdef USE_MAC_PRFL_BLIND
	status = KNS_MAC_init(KNS_MAC_PRFL_BLIND, (void *)&prflBlindUserCfg);
	if (status != KNS_STATUS_OK) {
		MGR_LOG_DEBUG("[ERROR] Cannot initialize MAC BLIND protocol: Error code 0x%x\r\n",
			status);
		MGR_LOG_DEBUG("[ERROR] Check protocol capabilities of the build\r\n");
		kns_assert(0);
	}
#endif
}

void KNS_APP_stdln_loop(void)
{
	/** Static variables for state machine.
	 * @note The implementation as a state machine is mainly usefull with kineis baremetal OS.
	 * The standalone application should be much more simple with a real OS
	 */
	static
	__attribute__((__section__(".retentionRamData")))
	uint8_t state;

	/** Static variable used with rand_r() function to generate random USER DATA in messages
	 * sent to Kineis stack.
	 * This will work also with STANDBY LPM as the seed tis mapped in SRAM2 memory here
	 * @note using rand() instead of rand_r() may lead to generate several messages with the
	 * same USER DATA payload
	 */
	static
	__attribute__((__section__(".retentionRamData")))
	uint32_t seed;

	uint8_t idx;
	struct KNS_MAC_appEvt_t appEvt;
	uint8_t buffer_tx[KNS_MAC_USRDATA_MAXLEN];

	switch (state) {
	case 0: { /** Send data event */
		struct KNS_CFG_radio_t device_radio_cfg;

		/** Initialize buffer with random data */
		for (idx = 0; idx < sizeof(buffer_tx); idx++)
			buffer_tx[idx] = rand_r(((unsigned int *)&seed));

		/** ---- OPTIONAL BLOCK ---- START -- needed if radio conf not hardcoded -----   */
		kns_assert(KNS_CFG_getRadioInfo(&device_radio_cfg) == KNS_STATUS_OK);

		appEvt.id =  KNS_MAC_SEND_DATA;
		for (idx = 0; idx < sizeof(buffer_tx); idx++)
			appEvt.data_ctxt.usrdata[idx] = buffer_tx[idx];
		appEvt.data_ctxt.sf = KNS_SF_NO_SERVICE;

		switch (device_radio_cfg.modulation) {
		case (KNS_TX_MOD_LDA2):
			/** max payload size in this  example, cf spec for smaller packets */
			appEvt.data_ctxt.usrdata_bitlen = 192;
			break;
		case (KNS_TX_MOD_LDA2L):
			appEvt.data_ctxt.usrdata_bitlen = 196;
			break;
		case (KNS_TX_MOD_VLDA4):
			appEvt.data_ctxt.usrdata_bitlen = 24;
			break;
		case (KNS_TX_MOD_LDK):
			appEvt.data_ctxt.usrdata_bitlen = 152;
			break;
		default:
			kns_assert(0); // wrong modulation flashed into device
			break;
		}
		/** ---- OPTIONAL BLOCK ---- END ---------------------------------------------   */

		MGR_LOG_DEBUG("[%s] request to send 0x", __func__);
		MGR_LOG_array(appEvt.data_ctxt.usrdata, (appEvt.data_ctxt.usrdata_bitlen+7)>>3);

		TEST_ASSERT(KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt) == KNS_STATUS_OK);

		state++; /* go to next state, and return to let higher task to process event */
		return;
	}
	case 1: { /** Wait for Kineis stack replying:
	 	   *  * OK to send frame
	 	   *  * TX done for previous transmit
	 	   */
		enum KNS_status_t status = KNS_STATUS_OK;
		struct KNS_MAC_srvcEvt_t srvcEvt;

		for (status = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt);
		     status != KNS_STATUS_QEMPTY;
		     status = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt)) {
			if (status == KNS_STATUS_OK) {
				switch (srvcEvt.id) {
				case (KNS_MAC_OK):
					if (srvcEvt.app_evt == KNS_MAC_SEND_DATA) {
						MGR_LOG_DEBUG("[%s] OK to send 0x", __func__);
						MGR_LOG_array(srvcEvt.tx_ctxt.data,
//							(srvcEvt.tx_ctxt.data_bitlen+7)>>3);
							4); // limit to 4 bytes for real-time
					}
					break;
				case (KNS_MAC_TX_DONE):
					MGR_LOG_DEBUG("[%s] TX done for 0x", __func__);
					MGR_LOG_array(srvcEvt.tx_ctxt.data,
						(srvcEvt.tx_ctxt.data_bitlen+7)>>3);
					TEST_PASS();
					state++;
					break;
				case (KNS_MAC_TX_TIMEOUT):
					MGR_LOG_DEBUG("[%s] TX timeout for 0x", __func__);
					MGR_LOG_array(srvcEvt.tx_ctxt.data,
						(srvcEvt.tx_ctxt.data_bitlen+7)>>3);
					TEST_FAIL();
					state++;
					break;
				case (KNS_MAC_ERROR):
					/** MAC ERROR can occure when FIFO is full, as expected per
					 * this application
					 */
					if (srvcEvt.app_evt == KNS_MAC_SEND_DATA) {
						MGR_LOG_DEBUG("[%s] cannot send 0x", __func__);
						MGR_LOG_array(srvcEvt.tx_ctxt.data,
//							(srvcEvt.tx_ctxt.data_bitlen+7)>>3);
							4); // limit to 4 bytes for real-time
					}
					TEST_FAIL();
					state++;
					break;
				default:
					TEST_FAIL();
					break;
				}
			}
		}
		return;
	}
	case 2: { /** Wait for Kineis stack replying:*/
		break;
	}
	default:
		kns_assert(0);
		break;
	}
}

void KNS_APP_gui_init(void *context)
{
	enum KNS_status_t status;

	kns_assert(context != NULL); // context should contain pointer to UART handle

	/** Initialize AT command manager */
	MGR_AT_CMD_start(context);

	/** Initialize Kineis MAC profile
	 * @todo to be moved in "AT+KMAC=<PRFL>,<prfl context>" AT command once implemented
	 * */
#ifdef USE_MAC_PRFL_BASIC
	status = KNS_MAC_init(KNS_MAC_PRFL_BASIC, NULL);
	if (status != KNS_STATUS_OK) {
		MGR_LOG_DEBUG("[ERROR] Cannot initialize MAC BASIC protocol: Error code 0x%x\r\n",
			status);
		MGR_LOG_DEBUG("[ERROR] Check protocol capabilities of the build\r\n");
		kns_assert(0);
	}
#endif
#ifdef USE_MAC_PRFL_BLIND
	status = KNS_MAC_init(KNS_MAC_PRFL_BLIND, (void *)&prflBlindUserCfg);
	if (status != KNS_STATUS_OK) {
		MGR_LOG_DEBUG("[ERROR] Cannot initialize MAC BLIND protocol: Error code 0x%x\r\n",
			status);
		MGR_LOG_DEBUG("[ERROR] Check protocol capabilities of the build\r\n");
		kns_assert(0);
	}
#endif
}

void KNS_APP_gui_loop(void)
{
	uint8_t *pu8_atcmd = NULL;

	/** ---- Look for AT cmds ---- */
	pu8_atcmd = MGR_AT_CMD_popNextAt();
	if (pu8_atcmd != NULL)
		MGR_AT_CMD_decodeAt(pu8_atcmd);  // @todo: return code is not used ?
	MGR_AT_CMD_macEvtProcess();
}

/**
 * @}
 */
