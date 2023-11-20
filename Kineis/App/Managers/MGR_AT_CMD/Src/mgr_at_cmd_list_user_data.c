// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_list_user_data.c
 * @author Kinéis
 * @brief subset of AT commands concerning user data manipulation such as TX
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "kns_types.h"
#include "user_data.h"
#include "mgr_at_cmd_common.h"
#include "mgr_at_cmd_list_user_data.h"
#include "kns_q.h"
#include "kns_mac.h"

#ifdef USE_TX_LED // Light on a GPIO when TX occurs
#include "main.h"
#endif

/* Private macro -------------------------------------------------------------*/

// defines used to check compilation flag settings of KNS_TX_MOD (numerical values are dummy)
#define LDA2 0x22
#define LDK 0x33
#define VLDA4 0x44
#define LDA2L 0x55

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

/** @brief  Log array of uint8_t
 * @param[in] data: pointer to table
 * @param[in] len: number of bytes to log from the table
 */
static void MGR_LOG_array(uint8_t *data, uint16_t len)
{
	uint16_t i;

	for (i = 0; i < len; i++)
		MGR_LOG_DEBUG_RAW("%02X", data[i]);
	MGR_LOG_DEBUG_RAW("\r\n");
}

/** @brief : convert the input form ASCII to binary.
 *
 * Output data (data after conversion) will be stored in the same buffer.
 *
 * The size of the buffer will be reduced to the half. So 0 will be set onto unused Bytes.
 *
 * Example:
 *           ______________________________________________________________
 * buffer   |  'A' |  'B' |  'C' |  'D' |  'E' |  'F' |  '0' |  '1' |  'F' |
 * before   ---------------------------------------------------------------
 * buffer    ______________________________________________________________
 * after    | 0xAB | 0xCD | 0xEF | 0x01 | 0xF0 | 0x00 | 0x00 | 0x00 | 0x00 |
 *
 * * In this example: u16_charNb is "9 characters" and the return value is "36 bits".
 *
 * @param[in,out] *pu8InputBuffer: pointer to the buffer that contains ASCII data. binary data
 * will be stored into the same buffer.
 * @param[in] u16_charNb: input data buffer length in ASCII char
 *
 * @return data length in bits if everything is OK, 0 otherwise.
 */
static uint16_t u16MGR_AT_CMD_convertAsciiBinary(uint8_t *pu8InputBuffer, uint16_t u16_charNb)
{
	uint16_t u16_index = 0;
	uint8_t u8_high, u8_low;

	for (u16_index = 0; u16_index < (u16_charNb / 2); u16_index++) {
		u8_high = u8UTIL_convertCharToHex4bits(pu8InputBuffer[2 * u16_index]);
		u8_low = u8UTIL_convertCharToHex4bits(pu8InputBuffer[2 * u16_index + 1]);
		if ((u8_high != 0xFF) && (u8_low != 0xFF))
			pu8InputBuffer[u16_index] = (u8_high << 4) | u8_low;
		else
			return 0;
	}

	/** Case : characters number is an odd number */
	if (u16_charNb % 2 != 0) {
		u8_high = u8UTIL_convertCharToHex4bits(pu8InputBuffer[u16_charNb - 1]);
		u8_low = 0;
		if (u8_high != 0xFF)
			pu8InputBuffer[u16_index++] = (u8_high << 4) | u8_low;
		else
			return 0;
	}

	/** Set other bytes to zero */
	for (; u16_index < u16_charNb; u16_index++)
		pu8InputBuffer[u16_index] = 0;

	/** Return data length in bits */
	return (((u16_charNb / 2) * 8) + ((u16_charNb % 2) ? 4 : 0));
}

#ifdef KIMX_FW
/** @brief This function converts kineis stack status into AT cmd error code
 *
 * param[in] knsStatus: KNS libraries status
 *
 * @retval ERROR_RETURN_T: KIM error code
 */
__attribute__((unused))
static enum ERROR_RETURN_T convKnsStatusToAtErr(enum KNS_status_t knsStatus)
{
	switch (knsStatus) {
	case KNS_STATUS_OK:
		return ERROR_NO;
	break;
	case KNS_STATUS_ERROR:
	case KNS_STATUS_NVM_ACCESS_ERR: /** @todo create specific Error code for this */
		return ERROR_UNKNOWN;
	break;
	case KNS_STATUS_DISABLED:
	case KNS_STATUS_BUSY:
	case KNS_STATUS_TIMEOUT:
	case KNS_STATUS_TR_ERR:
		return ERROR_TRCVR;
	break;
	case KNS_STATUS_BAD_SETTING:
		return ERROR_INCOMPATIBLE_VALUE;
	break;
	case KNS_STATUS_BAD_LEN:
		return ERROR_INVALID_USER_DATA_LENGTH;
	break;
	case KNS_STATUS_QFULL:
		return ERROR_DATA_QUEUE_FULL;
	break;
	case KNS_STATUS_QEMPTY:
		return ERROR_DATA_QUEUE_EMPTY;
	break;
	default:
		return ERROR_UNKNOWN;
	break;
	}
}
#endif

/** @brief Handle new TX data, this is the core function of AT+TX cmd
 *
 * @attention This fct assumes user data set by user is multiple of 8 bits.
 *
 * This fct is sensible from security point of view, as it is USER entry. It should be robust to
 * overflow.
 *
 * Maximum allowed data payload, in bits, is:
 *  * for A4 VLD protocols: 24 bits
 * All this will be checked in details in lower-layer frame formating.
 *
 * Here, at AT cmd and USERDATA level, it only checks incoming data is not exceeding the USERDATA
 * buffer size.
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] pcAtCmdPattern: AT command pattern to parse
 *
 * @return true if data is correctly processed, else otherwise
 */
static bool bMGR_AT_CMD_handleNewTxData(uint8_t *pu8_cmdParamString, const char *pcAtCmdPattern)
{
	enum KNS_status_t status = KNS_STATUS_OK;
	struct sUserDataTxFifoElt_t *spUserDataMsg = &sUserDataTx;
	union sUserDataAttribute_t u8UserDataAttr;
	uint8_t *pu8UserDataBuf;
	int16_t i16_scan_param_res;
	uint16_t u16UserDataCharNb;
	uint16_t u16UserDataBitlen;
	uint8_t idx;

//	enum KNS_status_t knsStatus;
	struct KNS_MAC_appEvt_t appEvt = {
		.id = KNS_MAC_SEND_DATA,
		.data_ctxt = {
			.usrdata = {0},
			.usrdata_bitlen = 0,      /* to be filled-up later below */
			.sf = KNS_SF_NO_SERVICE,
		}
	};

	/** So far, in case a new command occurs while previous one is not completed, abort it.
	 * Send error response concerning previous frame.
	 *
	 * @todo remove code block below once KIMX_FW correctly supports RX timeout
	 */
	if (spUserDataMsg->bIsToBeTransmit) {
		MGR_LOG_VERBOSE("MGR_AT_CMD: Abort transmission of previous frame\r\n");
		spUserDataMsg->bIsToBeTransmit = false;
		appEvt.id =  KNS_MAC_STOP_SEND_DATA;
		status = KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt);
		assert_param(status == KNS_STATUS_OK);
		bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXNOTOK, (void *)spUserDataMsg);
	}

	if (!spUserDataMsg->bIsToBeTransmit) {

		for (idx = 0; idx < sizeof(spUserDataMsg->u8DataBuf); idx++)
			spUserDataMsg->u8DataBuf[idx] = 0;
		pu8UserDataBuf =  spUserDataMsg->u8DataBuf;

		/** Extract USER DATA from pu8_cmdParamString */
		i16_scan_param_res = sscanf((const char *)pu8_cmdParamString, pcAtCmdPattern,
					    pu8UserDataBuf,
					    &u8UserDataAttr.u8_raw);
		u16UserDataCharNb = strlen((const char *)pu8UserDataBuf);

		/** Assert when the number of characters received from AT command is bigger than
		 * authorized
		 */
		assert_param(u16UserDataCharNb <= (USERDATA_TX_PAYLOAD_MAX_SIZE * 2));

		switch (i16_scan_param_res) {
		case 1:/** Case ARGOS Message with user data only */
			u8UserDataAttr.u8_raw = 0x0; /* default attribute to data, no service */
		case 2:/** Case ARGOS Message with user data + optional attribute:
			 * User data should be converted form string format to Hex format
			 */
			u16UserDataBitlen = u16MGR_AT_CMD_convertAsciiBinary(pu8UserDataBuf,
									     u16UserDataCharNb);
			if (u16UserDataBitlen <= (USERDATA_TX_PAYLOAD_MAX_SIZE * 8)) {
				spUserDataMsg->u16DataBitLen = u16UserDataBitlen;
				spUserDataMsg->u8Attr = u8UserDataAttr;
				spUserDataMsg->bIsToBeTransmit = true;

				for (idx = 0; idx < sizeof(appEvt.data_ctxt.usrdata); idx++)
					appEvt.data_ctxt.usrdata[idx] =
						spUserDataMsg->u8DataBuf[idx];
				appEvt.data_ctxt.usrdata_bitlen = spUserDataMsg->u16DataBitLen;
				appEvt.data_ctxt.sf =
					(enum KNS_serviceFlag_t)(spUserDataMsg->u8Attr.sf);

				/** @note appEvt.send_ctxt already filled-up at declaration */
				appEvt.id = KNS_MAC_SEND_DATA;
				status = KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt);
				assert_param(status == KNS_STATUS_OK);
				return true;
			}
			MGR_LOG_VERBOSE("[ERROR] User data is badly formatted (check length)\r\n");
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_USER_DATA_LENGTH);
		case 0: /* Case ARGOS Message without user data */
		default:
			MGR_LOG_VERBOSE("[ERROR] AT+TX command is badly formatted\r\n");
			return bMGR_AT_CMD_logFailedMsg(ERROR_MISSING_PARAMETERS);
		}
	} else {
		MGR_LOG_VERBOSE("[ERROR] TX FIFO full, cannot get extra data.\r\n");
		return bMGR_AT_CMD_logFailedMsg(ERROR_DATA_QUEUE_FULL);
	}
}

/* Public functions ----------------------------------------------------------*/

bool bMGR_AT_CMD_TX_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode)
{
	/** @attention pattern length below shall not be longer than the length defined by
	 * FRAME_MAX_LEN.
	 *
	 * So far, here in LDA2 example; limit size to 48 chars, i.e. 24 bytes
	 */
	static const char cAtCmdPattern[] = "AT+TX=%49[0-9A-Fa-f],0x%hX";

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		MGR_LOG_VERBOSE("[ERROR] Status mode is unauthorized for this AT cmd\r\n");
		return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
	}

	if (bMGR_AT_CMD_handleNewTxData(pu8_cmdParamString, cAtCmdPattern))
		return true;
	else
		return false;
}

#ifdef KIMX_FW
bool bMGR_AT_CMD_RX_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode)
{
	int16_t scanParamRes;
	uint8_t rxMode = 0;
	struct KNS_MAC_appEvt_t appEvt;
	enum KNS_status_t status = KNS_STATUS_OK;

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		MGR_LOG_VERBOSE("[ERROR] Status mode is unauthorized for this AT cmd\r\n");
		return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
	}

	scanParamRes = sscanf((const char *)pu8_cmdParamString,
			(const char *)"AT+RX=%hhu",
			&rxMode);

	if (scanParamRes == 1) {
		switch (rxMode) {
		case 0:
			MGR_LOG_VERBOSE("stop RX\r\n");
			appEvt.id = KNS_MAC_RX_STOP;
			status = KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt);
			assert_param(status == KNS_STATUS_OK);
			return true;
		break;
		case 1:
		{
			MGR_LOG_VERBOSE("start RX\r\n");
			appEvt.id = KNS_MAC_RX_START;
			status = KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt);
			assert_param(status == KNS_STATUS_OK);
			return true;
		}
		break;
		default:
			return bMGR_AT_CMD_logFailedMsg(ERROR_INCOMPATIBLE_VALUE);
		break;
		}
	}

	return bMGR_AT_CMD_logFailedMsg(ERROR_PARAMETER_FORMAT);
}
#endif

enum KNS_status_t MGR_AT_CMD_macEvtProcess(void)
{
	enum KNS_status_t cbStatus;
	struct KNS_MAC_srvcEvt_t srvcEvt;
	struct sUserDataTxFifoElt_t *spUserDataMsg = &sUserDataTx;

	cbStatus = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt);

	if (cbStatus == KNS_STATUS_OK) {
		switch (srvcEvt.id) {
		case (KNS_MAC_TX_DONE):
			MGR_LOG_DEBUG("MGR_AT_CMD TX_DONE callback reached\r\n");
			MGR_LOG_DEBUG("TX DONE for usrdata (%d bits = %d bytes + %d bits): 0x",
				srvcEvt.tx_ctxt.data_bitlen,
				srvcEvt.tx_ctxt.data_bitlen>>3,
				srvcEvt.tx_ctxt.data_bitlen&0x07);
			MGR_LOG_array(srvcEvt.tx_ctxt.data, (srvcEvt.tx_ctxt.data_bitlen+7)>>3);

			assert_param(sUserDataTx.bIsToBeTransmit);
			/** Upon TX done of a mail request message, it means some DL_BC was received
			 * Thus, UL ACK of DL_BC will transmitted by lower layer internally just
			 * after the end of this callback.
			 * AT response "+TX=..." will be sent to UART once UL-ACK is really
			 * transmitted, cf TXACK_DONE below
			 */
			if (sUserDataTx.u8Attr.sf == ATTR_MAIL_REQUEST) {
				Set_TX_LED(0);
			} else {
				/** @todo Should check integrity between data reported by event
				 * above and the one stored in user data buffer
				 *
				 * So far, as only one user data, consider this is the correct one:
				 * * notify host with AT cmd response then
				 * * free element from user data buffer.
				 */
				bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXOK, (void *)&sUserDataTx);
				sUserDataTx.bIsToBeTransmit = false; /* Free as host notified */
				Set_TX_LED(0);
			}
			cbStatus = KNS_STATUS_OK;
		break;
		case (KNS_MAC_TXACK_DONE):
			MGR_LOG_DEBUG("MGR_AT_CMD TXACK_DONE callback reached\r\n");
			assert_param(sUserDataTx.bIsToBeTransmit);
			/** Upon TX done of a mail request message, it means some DL_BC was received
			 * previously and UL ACK of DL_BC was just transmitted.
			 * Send +TX= instead of +TACK=, meaning this is the real end of TX data
			 * transmission
			 */
			if (sUserDataTx.u8Attr.sf == ATTR_MAIL_REQUEST)
				bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXOK, (void *)&sUserDataTx);
			else
				bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXACKOK, NULL);

			sUserDataTx.bIsToBeTransmit = false; /* Free as host is notified */
			Set_TX_LED(0);
			cbStatus = KNS_STATUS_OK;
		break;
		case (KNS_MAC_TX_TIMEOUT):
			MGR_LOG_DEBUG("MGR_AT_CMD TX_TIMEOUT callback reached\r\n");
			MGR_LOG_DEBUG("TX TIMEOUT for usrdata (%d bits = %d bytes + %d bits): 0X",
				srvcEvt.tx_ctxt.data_bitlen,
				srvcEvt.tx_ctxt.data_bitlen>>3,
				srvcEvt.tx_ctxt.data_bitlen&0x07);
			MGR_LOG_array(srvcEvt.tx_ctxt.data, (srvcEvt.tx_ctxt.data_bitlen+7)>>3);
			assert_param(sUserDataTx.bIsToBeTransmit);
			/** @todo Should check integrity between data reported by event above and
			 * the one stored in user data buffer
			 *
			 * So far, as only one user data, consider this is the correct one:
			 * * notify host with AT cmd response then
			 * * free element from user data buffer.
			 */
			bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXNOTOK, (void *)&sUserDataTx);
			sUserDataTx.bIsToBeTransmit = false;
			Set_TX_LED(0);
			cbStatus = KNS_STATUS_TIMEOUT;
		break;
		case (KNS_MAC_TXACK_TIMEOUT):
			MGR_LOG_DEBUG("MGR_AT_CMD TXACK_TIMEOUT callback reached\r\n");
			assert_param(sUserDataTx.bIsToBeTransmit);
			bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXACKNOTOK, NULL);
			sUserDataTx.bIsToBeTransmit = false;
			Set_TX_LED(0);
			cbStatus = KNS_STATUS_TIMEOUT;
		break;
		case (KNS_MAC_RX_ERROR):  /**< RX error during TRX frame, report TX failure then */
			MGR_LOG_DEBUG("MGR_AT_CMD TX callback reached\r\n");
			if (sUserDataTx.bIsToBeTransmit) {
				MGR_LOG_DEBUG("RX enable ERROR (%d bits = %d bytes + %d bits): 0x",
					srvcEvt.tx_ctxt.data_bitlen,
					srvcEvt.tx_ctxt.data_bitlen>>3,
					srvcEvt.tx_ctxt.data_bitlen&0x07);
				MGR_LOG_array(srvcEvt.tx_ctxt.data,
					(srvcEvt.tx_ctxt.data_bitlen+7)>>3);
				/** @todo Should check integrity between data reported by event
				 * above and the one stored in user data buffer
				 *
				 * So far, as only one user data, consider this is the correct one:
				 * * notify host with AT cmd response then
				 * * free element from user data buffer.
				 */
				bMGR_AT_CMD_sendResponse(ATCMD_RSP_TXNOTOK, (void *)&sUserDataTx);
				sUserDataTx.bIsToBeTransmit = false;
				Set_TX_LED(0);
				cbStatus = KNS_STATUS_TR_ERR;
			} else {
				MGR_LOG_DEBUG("MGR_AT_CMD RX callback reached\r\n");
				MGR_LOG_DEBUG("RX ERROR  unexpected event received.\r\n");
				cbStatus = KNS_STATUS_ERROR;
			}
		break;
		case (KNS_MAC_RX_TIMEOUT): /**< RX window reached during TRX, report failure then */
			/** @attention so far, TX failure is reported upon TRX RX timeout when:
			 * * no DL-ACK received for a TX requesting ACK (this case is fine)
			 * * no DL-BC received for a TX mail request. Actually this may really occur
			 * as no BC was available on board (protocol to be discussed)
			 */
			MGR_LOG_DEBUG("MGR_AT_CMD RX timeout callback reached\r\n");
			MGR_LOG_DEBUG("ERROR: no DL frm for (%d bits = %d bytes + %d bits): 0x",
				srvcEvt.tx_ctxt.data_bitlen,
				srvcEvt.tx_ctxt.data_bitlen>>3,
				srvcEvt.tx_ctxt.data_bitlen&0x07);
			MGR_LOG_array(srvcEvt.tx_ctxt.data, (srvcEvt.tx_ctxt.data_bitlen+7)>>3);
			/** @todo Should check integrity between data reported by event above and
			 * the one stored in user data buffer
			 *
			 * So far, as only one user data, consider this is the correct one:
			 * * notify host with AT cmd response then
			 * * free element from user data buffer.
			 */
			bMGR_AT_CMD_sendResponse(ATCMD_RSP_RXTIMEOUT, (void *)&sUserDataTx);
			sUserDataTx.bIsToBeTransmit = false;
			Set_TX_LED(0);
			cbStatus = KNS_STATUS_TIMEOUT;
		break;
#ifdef KIMX_FW
		case (KNS_MAC_DL_ACK):
		case (KNS_MAC_DL_BC):
			MGR_LOG_DEBUG("MGR_AT_CMD DL callback reached\r\n");
	//		MGR_LOG_DEBUG("decoded msg (%d bits = %d bytes + %d bits): 0x",
	//			srvcEvt.rx_ctxt.data_bitlen,
	//			srvcEvt.rx_ctxt.data_bitlen>>3,
	//			srvcEvt.rx_ctxt.data_bitlen&0x07);
	//		MGR_LOG_array(srvcEvt.rx_ctxt.data, (srvcEvt.rx_ctxt.data_bitlen+7)>>3);
			/** @todo Should check integrity between data reported by event above and
			 * the one stored in user data buffer
			 *
			 * So far, as only one user data, consider this is the correct one:
			 * * notify host with AT cmd response then
			 * * free element from user data buffer.
			 */
			bMGR_AT_CMD_sendResponse(ATCMD_RSP_DLOK, (void *)&(srvcEvt.rx_ctxt));
			cbStatus = KNS_STATUS_OK;
		break;
		case (KNS_MAC_RX_RECEIVED):
			MGR_LOG_DEBUG("MGR_AT_CMD RX callback reached\r\n");
	//		MGR_LOG_DEBUG("bitstream (%d bits = %d bytes + %d bits): 0x",
	//			srvcEvt.rx_ctxt.data_bitlen,
	//			srvcEvt.rx_ctxt.data_bitlen>>3,
	//			srvcEvt.rx_ctxt.data_bitlen&0x07);
	//		MGR_LOG_array(srvcEvt.rx_ctxt.data, (srvcEvt.rx_ctxt.data_bitlen+7)>>3);
			/** @todo Should check integrity between data reported by event above and
			 * the one stored in user data buffer
			 *
			 * So far, as only one user data, consider this is the correct one:
			 * * notify host with AT cmd response then
			 * * free element from user data buffer.
			 */
			bMGR_AT_CMD_sendResponse(ATCMD_RSP_RXOK, (void *)&(srvcEvt.rx_ctxt));
			cbStatus = KNS_STATUS_OK;
		break;
		case (KNS_MAC_SAT_DETECTED):
			MGR_LOG_DEBUG("MGR_AT_CMD SAT detect callback reached\r\n");
			bMGR_AT_CMD_sendResponse(ATCMD_RSP_SATDET, NULL);
			cbStatus = KNS_STATUS_OK;
		break;
#endif
		case (KNS_MAC_OK):
			MGR_LOG_DEBUG("MGR_AT_CMD MAC reported OK to previous command.\r\n");
			bMGR_AT_CMD_logSucceedMsg();
			if (srvcEvt.app_evt == KNS_MAC_SEND_DATA)
				Set_TX_LED(1);
			cbStatus = KNS_STATUS_OK;
		break;
		case (KNS_MAC_ERROR):
			MGR_LOG_DEBUG("MGR_AT_CMD MAC reported ERROR to previous command.\r\n");
			bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN);
			if (srvcEvt.app_evt == KNS_MAC_SEND_DATA)
				spUserDataMsg->bIsToBeTransmit = false;
			cbStatus = KNS_STATUS_ERROR;
		break;
		default:
			assert_param(0);
			cbStatus = KNS_STATUS_ERROR;
		break;
		}
	}

	return cbStatus;
}

/**
 * @}
 */
