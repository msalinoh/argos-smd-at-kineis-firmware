// SPDX-License-Identifier: no SPDX license
/**
 * @file   mgr_spi_cmd.c
 * @author Arribada
 * @brief  TBD
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */


/* Includes -------------------------------------------------------------------------------------*/
#include <string.h>

#include "kns_types.h"
#include "mgr_spi_cmd.h"
// #include "mgr_at_cmd_common.h"
// #include "mgr_at_cmd_list.h"
#include "kineis_sw_conf.h"
#include KINEIS_SW_ASSERT_H
#include "mgr_log.h"
#include "build_info.h"
#include "mcu_nvm.h"
#include "kns_cfg.h"
#include "mgr_at_cmd_list_user_data.h"
#include "kns_mac.h"
#include "kns_q.h"
#include "lpm.h"
#include "user_data.h"
/* Defines --------------------------------------------------------------------------------------*/


/* Structure Declaration ------------------------------------------------------------------------*/
/* Private variables ----------------------------------------------------------------------------*/
uint8_t mac_status = (0 | MAC_OK);
const uint8_t spicmd_version = 3;
uint16_t txBytesWaiting;
/* Private functions ----------------------------------------------------------------------------*/
/**
 * @brief
 *
 * @attention This fct may be called from ISR context
 *
 * @note
 *
 * @param[in,out] pu8_RxBuffer pointer to start of RX buffer
 * @param[in,out] pi16_nbRxValidChar number of valid charecters in RX buffer
 *
 * @retval Return number of byt to read
 */
static int8_t MGR_SPI_CMD_parseStreamCb(SPI_Buffer *rx, SPI_Buffer *tx)
{
	uint8_t cmd = CMD_UNKNOWN;
	enum KNS_status_t status = KNS_STATUS_OK;
	if (rx->size > 0)
	{
		cmd = rx->data[0];
		switch (cmd) {
			case CMD_READ:
				MGR_LOG_DEBUG("Handling Read command\n\r");
				break;
			case CMD_PING:
				MGR_LOG_DEBUG("Handling PING command\n\r");
				tx->data[0] = 1;
				tx->next_req = 1;
				rx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_MAC_STATUS:
				MGR_LOG_DEBUG("Handling MAC_STATUS command\n\r");
				tx->data[0] = mac_status;
				tx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				//Reset tx/rx state if MAC_OK
			    if (mac_status & MAC_OK)
			    {
			    	mac_status = (0 | MAC_OK);
			    }
				rx->next_req = 1;
				break;
			case CMD_SPI_STATUS:
				MGR_LOG_DEBUG("Handling SPI_STATUS command\n\r");
				tx->data[0] = spi_status;
				tx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_VERSION:
				MGR_LOG_DEBUG("Handling READ_VERSION command\n\r");
				tx->data[0] = spicmd_version;
				tx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_FIRMWARE:
				MGR_LOG_DEBUG("Handling READ_FIRMWARE command\n\r");
				memcpy(&tx->data[0], uc_fw_vers_commit_id, FW_VERSION_LENGTH);  // Copy the entire fixed-length string
				tx->next_req = FW_VERSION_LENGTH;  // Total bytes to send
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_ADDR:
				MGR_LOG_DEBUG("Handling READ_ADDR command\n\r");
				uint8_t dev_addr[DEVICE_ADDR_LENGTH];
				if (KNS_CFG_getAddr(dev_addr) != KNS_STATUS_OK)
				{
					return bMGR_SPI_CMD_logFailedMsg(ERROR_INVALID_ID, tx);
				}
				tx->next_req = DEVICE_ADDR_LENGTH;
				memcpy(&tx->data[0], dev_addr, DEVICE_ADDR_LENGTH);
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_ID:
				MGR_LOG_DEBUG("Handling READ_ID command\n\r");
				uint32_t dev_id;
				if (KNS_CFG_getId(&dev_id) != KNS_STATUS_OK)
				{
					return bMGR_SPI_CMD_logFailedMsg(ERROR_INVALID_ID, tx);
						/* ID is printed as a number, with decimal representation.
						 * ID is stored in memory in little endian format.
						 */
				}
				tx->next_req = sizeof(dev_id);
				memcpy(&tx->data[0], dev_addr, tx->next_req);
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_SN:
				MGR_LOG_DEBUG("Handling READ_SN command\n\r");
				uint8_t dev_sn[DEVICE_SN_LENGTH +1];

				if (KNS_CFG_getSN(dev_sn) != KNS_STATUS_OK)
					/* TODO: add a new error code ? */
					return bMGR_SPI_CMD_logFailedMsg(ERROR_INVALID_ID, tx);
				dev_sn[DEVICE_SN_LENGTH] = '\0';
				memcpy(&tx->data[0], dev_sn, sizeof(dev_sn));  // Copy the entire fixed-length string
				tx->next_req = sizeof(dev_sn);  // Total bytes to send
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_RCONF:
				MGR_LOG_DEBUG("Handling READ_RCONF command\n\r");
				struct KNS_CFG_radio_t radio_cfg;

				if (KNS_CFG_getRadioInfo(&radio_cfg) != KNS_STATUS_OK)
					/* TODO: add a new error code ? */
					return bMGR_SPI_CMD_logFailedMsg(ERROR_INVALID_ID, tx);

				memcpy(&tx->data[0], &radio_cfg, sizeof(radio_cfg));  // Copy the entire fixed-length string
				tx->next_req = sizeof(radio_cfg);  // Total bytes to send
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_WRITE_RCONF_REQ:
				MGR_LOG_DEBUG("Handling WRITE_RCONF_REQ command\n\r");
				tx->data[0] = cmd;
				rx->next_req = 33;
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_WRITE_RCONF:
				MGR_LOG_DEBUG("Handling WRITE_RCONF command\n\r");

				uint16_t nbBits = u16MGR_AT_CMD_convertAsciiBinary(&(rx->data[1]), 32);

				if (nbBits != 128)
					/* TODO: add a new error code ? */
					return bMGR_SPI_CMD_logFailedMsg(ERROR_INVALID_ID, tx);

				if (KNS_CFG_setRadioInfo(&(rx->data[1])) != KNS_STATUS_OK)
					/* TODO: add a new error code ? */
					return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);

				rx->next_req = 1;
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_SAVE_RCONF:
				MGR_LOG_DEBUG("Handling SAVE_RCONF command\n\r");
				if (KNS_CFG_saveRadioInfo() != KNS_STATUS_OK)
					/* TODO: add a new error code ? */
					return bMGR_SPI_CMD_logFailedMsg(ERROR_INVALID_ID, tx);
				tx->data[0] = 1;
				tx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_KMAC:
				MGR_LOG_DEBUG("Handling READ_KMAC command\n\r");
				struct KNS_MAC_prflInfo_t prfl_info;
				uint8_t *prflCfgPtr;

				KNS_MAC_getPrflInfo(&prfl_info);
				prflCfgPtr = &prfl_info.prflCfgPtr;
				tx->data[0] = prfl_info.id;
				memcpy(&(tx->data[1]), prflCfgPtr, sizeof(prfl_info) - 1);
				tx->next_req = sizeof(prfl_info);
				MCU_SPI_DRIVER_resp(cmd);
				// log profile context, reduce size by 1 due to `id` field of KNS_MAC_prflInfo_t
				break;
			case CMD_WRITE_KMAC_REQ:
				MGR_LOG_DEBUG("Handling WRITE_KMAC_REQ command\n\r");
				tx->data[0] = cmd;
				rx->next_req = 2; // Only waiting profile id for the moment
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_WRITE_KMAC:
				MGR_LOG_DEBUG("Handling WRITE_KMAC command\n\r");
				status = KNS_STATUS_OK;
				struct KNS_MAC_appEvt_t appEvt = {
					.id = KNS_MAC_INIT,
					.init_prfl_ctxt = {
						.id =  KNS_MAC_PRFL_NONE,
						.blindCfg = {0}
					}
				};
				appEvt.init_prfl_ctxt.id = rx->data[1];

				status = KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvt);
				switch (status) {
					case KNS_STATUS_QFULL:
						return bMGR_SPI_CMD_logFailedMsg(ERROR_DATA_QUEUE_FULL, tx);
					break;
					case KNS_STATUS_OK:
					break;
					default:
						return bMGR_SPI_CMD_logFailedMsg(ERROR_UNKNOWN, tx);
					break;
				}

				rx->next_req = 1;
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_READ_LPM:
				MGR_LOG_DEBUG("Handling READ_LPM command\n\r");
				tx->data[0] = lpm_config.allowedLPMbitmap;
				tx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_WRITE_LPM_REQ:
				MGR_LOG_DEBUG("Handling WRITE_LPM_REQ command\n\r");
				tx->data[0] = cmd;
				rx->next_req = 2; // Only waiting profile id for the moment
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_WRITE_LPM:
				MGR_LOG_DEBUG("Handling WRITE_LPM command\n\r");
				uint8_t lpm = rx->data[1];  // Extract LPM value from rx->data[1]

				// Define allowed low-power modes
				const uint8_t allowedModes = LOW_POWER_MODE_NONE |
											 LOW_POWER_MODE_SLEEP |
											 LOW_POWER_MODE_STOP |
											 LOW_POWER_MODE_STANDBY |
											 LOW_POWER_MODE_SHUTDOWN;

				// Validate the received LPM value
				if ((lpm & ~allowedModes) != 0) {
					// Invalid value: contains bits outside the allowed set
					return bMGR_SPI_CMD_logFailedMsg(ERROR_PARAMETER_FORMAT, tx);
				}
				lpm_config.allowedLPMbitmap = lpm;
				tx->data[0] = cmd;
				rx->next_req = 1; // Only waiting profile id for the moment
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_WRITE_TX_REQ:
				MGR_LOG_DEBUG("Handling WRITE_TX_REQ command\n\r");
				tx->data[0] = cmd;
				rx->next_req = 3; // Waiting TX_Size req
				txBytesWaiting = 0;
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
			case CMD_WRITE_TX_SIZE:
				MGR_LOG_DEBUG("Handling WRITE_TX_SIZE command\n\r");
				txBytesWaiting = (rx->data[1] <<8 )| rx->data[2];
				tx->data[0] = cmd;
//				if(txBytesWaiting <= (USERDATA_TX_DATAFIELD_SIZE))
				if(txBytesWaiting <= (USERDATA_TX_PAYLOAD_MAX_SIZE))
				{
					rx->next_req = txBytesWaiting + 1; // Command + user mesage to send
					MCU_SPI_DRIVER_wait_next(cmd);
				} else {
					mac_status = MAC_TX_SIZE_ERROR;
					return bMGR_SPI_CMD_logFailedMsg(ERROR_PARAMETER_FORMAT, tx);
				}
				break;
			case CMD_WRITE_TX:
				MGR_LOG_DEBUG("Handling WRITE_TX command\n\r");


				struct sUserDataTxFifoElt_t *spUserDataMsg;
				union sUserDataAttribute_t u8UserDataAttr;
				uint8_t *pu8UserDataBuf;
				uint16_t u16UserDataBitlen;
				uint16_t idx;

				//	enum KNS_status_t knsStatus;
				struct KNS_MAC_appEvt_t appEvtTx = {
					.id = KNS_MAC_SEND_DATA,
					.data_ctxt = {
						.usrdata = {0},
						.usrdata_bitlen = 0,      /* to be filled-up later below */
						.sf = KNS_SF_NO_SERVICE,
					}
				};


				spUserDataMsg = USERDATA_txFifoReserveElt();
				if (spUserDataMsg != NULL) {

					// Clear the buffer
					for (idx = 0; idx < sizeof(spUserDataMsg->u8DataBuf); idx++)
						spUserDataMsg->u8DataBuf[idx] = 0;
					//memset(spUserDataMsg->u8DataBuf, 0, sizeof(spUserDataMsg->u8DataBuf));
					pu8UserDataBuf = spUserDataMsg->u8DataBuf;
					kns_assert(pu8UserDataBuf != NULL);

					// Copy the raw bytes received via SPI
//					memcpy(pu8UserDataBuf, &(rx->data[1]), txBytesWaiting);
					for (idx = 0; idx < sizeof(spUserDataMsg->u8DataBuf); idx++)
						pu8UserDataBuf[idx] = rx->data[1 + idx];

					// Default attribute: raw data, no special service
					u8UserDataAttr.u8_raw = 0x0;

					// Set the bit length for the message
					u16UserDataBitlen = txBytesWaiting * 8;
					spUserDataMsg->u16DataBitLen = u16UserDataBitlen;
					spUserDataMsg->u8Attr = u8UserDataAttr;

					// Add the message to the FIFO
					kns_assert(USERDATA_txFifoAddElt(spUserDataMsg, true));

					// Populate the application event structure
					//memcpy(appEvtTx.data_ctxt.usrdata, spUserDataMsg->u8DataBuf, sizeof(appEvtTx.data_ctxt.usrdata));
					for (idx = 0; idx < sizeof(appEvtTx.data_ctxt.usrdata); idx++)
						appEvtTx.data_ctxt.usrdata[idx] = spUserDataMsg->u8DataBuf[idx];

					appEvtTx.data_ctxt.usrdata_bitlen = spUserDataMsg->u16DataBitLen;
					appEvtTx.data_ctxt.sf = (enum KNS_serviceFlag_t)(spUserDataMsg->u8Attr.sf);

					 // Push the event to the MAC layer
					status = KNS_Q_push(KNS_Q_DL_APP2MAC, (void *)&appEvtTx);
					switch (status) {
					case KNS_STATUS_QFULL:
						MGR_LOG_VERBOSE("[ERROR] TX FIFO full, cannot push new data.\r\n");
						return bMGR_SPI_CMD_logFailedMsg(ERROR_DATA_QUEUE_FULL, tx);
						break;
					case KNS_STATUS_OK:
						// Successfully pushed the event
						break;
					default:
						MGR_LOG_VERBOSE("[ERROR] Unknown status when pushing TX data.\r\n");
						return bMGR_SPI_CMD_logFailedMsg(ERROR_UNKNOWN, tx);
						break;
					}
				} else {
					MGR_LOG_VERBOSE("[ERROR] TX FIFO full, cannot get extra data.\r\n");
					return bMGR_SPI_CMD_logFailedMsg(ERROR_DATA_QUEUE_FULL, tx);
				}
				tx->data[0] = 1;
				tx->next_req = 1;
				MCU_SPI_DRIVER_resp(cmd);
				break;
			case CMD_READ_CW:
				MGR_LOG_DEBUG("Handling READ_CW command : TBD\n\r");
				break;
			case CMD_WRITE_CW_REQ:
				MGR_LOG_DEBUG("Handling WRITE_CW_REQ command : TBD\n\r");
				break;
			case CMD_WRITE_CW:
				MGR_LOG_DEBUG("Handling WRITE_CW command : TBD\n\r");
				break;
			case CMD_READ_PREPASSEN:
				MGR_LOG_DEBUG("Handling READ_PREPASSEN command : TBD\n\r");
				break;
			case CMD_WRITE_PREPASSEN_REQ:
				MGR_LOG_DEBUG("Handling WRITE_PREPASSEN_REQ command : TBD\n\r");
				break;
			case CMD_WRITE_PREPASSEN:
				MGR_LOG_DEBUG("Handling WRITE_CW command : TBD\n\r");
				break;
			case CMD_READ_UDATE:
				MGR_LOG_DEBUG("Handling READ_UDATE command : TBD\n\r");
				break;
			case CMD_WRITE_UDATE_REQ:
				MGR_LOG_DEBUG("Handling WRITE_UDATE_REQ command : TBD\n\r");
				break;
			case CMD_WRITE_UDATE:
				MGR_LOG_DEBUG("Handling WRITE_UDATE command : TBD\n\r");
				break;
			default:
				MGR_LOG_DEBUG("Unknown command: 0x%02X\n\r", cmd);
				rx->next_req = 1;
				MCU_SPI_DRIVER_wait_next(cmd);
				break;
		}
	}
	else {
		MGR_LOG_DEBUG("%s: RX size less than 0\n\r",__func__);
		cmd = CMD_UNKNOWN;
		rx->next_req = 1;
		MCU_SPI_DRIVER_wait_next(cmd);
	}

	return cmd;
}

int8_t bMGR_SPI_CMD_logFailedMsg(enum ERROR_RETURN_T eErrorType, SPI_Buffer *tx)
{
	//return NACK
	tx->data[0] = 0;
	tx->next_req = 1;
	MCU_SPI_DRIVER_resp(CMD_UNKNOWN);
	return -eErrorType;
}

/* Functions ------------------------------------------------------------------------------------*/

bool MGR_SPI_CMD_start(void *context)
{
	return MCU_SPI_DRIVER_register(context, MGR_SPI_CMD_parseStreamCb);

}



//__attribute((__weak__))
enum KNS_status_t MGR_SPI_CMD_macEvtProcess(void)
{
	/** Empty weak core, can be overwritten, depending on AT cmd processed */
	enum KNS_status_t cbStatus;
	struct KNS_MAC_srvcEvt_t srvcEvt;
	struct sUserDataTxFifoElt_t *spUserDataMsg = USERDATA_txFifoGetFirst();

	cbStatus = KNS_Q_pop(KNS_Q_UL_MAC2APP, (void *)&srvcEvt);

	if (cbStatus != KNS_STATUS_QEMPTY)
	{
		MGR_LOG_DEBUG("KNS STATUS not QUEMPTYU \r\n");
	}

	if (cbStatus != KNS_STATUS_OK)
		return cbStatus;

	//MGR_LOG_DEBUG("macEVT status %u\r\n", cbStatus);
	/** get pointer to user data FIFO element when possible */
	switch (srvcEvt.id) {
		case (KNS_MAC_TX_DONE):
		case (KNS_MAC_TXACK_DONE):
		case (KNS_MAC_TX_TIMEOUT):
		case (KNS_MAC_TXACK_TIMEOUT):
		case (KNS_MAC_RX_ERROR):
		case (KNS_MAC_RX_TIMEOUT):
			spUserDataMsg = USERDATA_txFifoFindPayload(srvcEvt.tx_ctxt.data,
				srvcEvt.tx_ctxt.data_bitlen);
			kns_assert(spUserDataMsg != NULL);
			mac_status = MAC_RX_TIMEOUT;
		break;
		case (KNS_MAC_ERROR):
			if (srvcEvt.app_evt == KNS_MAC_SEND_DATA) {
				spUserDataMsg = USERDATA_txFifoFindPayload(srvcEvt.tx_ctxt.data,
					srvcEvt.tx_ctxt.data_bitlen);
				kns_assert(spUserDataMsg != NULL);
				mac_status = MAC_ERROR;
			}
		break;
		default:
		break;
	}

	/** process event */
	switch (srvcEvt.id) {
	case (KNS_MAC_TX_DONE):
		MGR_LOG_DEBUG("MGR_AT_CMD TX_DONE callback reached\r\n");
		kns_assert(spUserDataMsg->bIsToBeTransmit);
		/** Upon TX done of a mail request message, it means some DL_BC was received
		 * Thus, UL ACK of DL_BC will transmitted by lower layer internally just
		 * after the end of this callback.
		 * AT response "+TX=..." will be sent to UART once UL-ACK is really
		 * transmitted, cf TXACK_DONE below
		 */
		if (spUserDataMsg->u8Attr.sf == ATTR_MAIL_REQUEST) {
			//TODO no service used for the moment
			//Set_TX_LED(0);
		} else {
			/** @todo Should check integrity between data reported by event
			 * above and the one stored in user data buffer
			 *
			 * So far, as only one user data, consider this is the correct one:
			 * * notify host with AT cmd response then
			 * * free element from user data buffer.
			 */
			mac_status = MAC_TX_DONE;
			USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
			//Set_TX_LED(0);
		}
		cbStatus = KNS_STATUS_OK;
		break;
	case (KNS_MAC_TXACK_DONE):
		MGR_LOG_DEBUG("MGR_AT_CMD TXACK_DONE callback reached\r\n");
		kns_assert(spUserDataMsg->bIsToBeTransmit);
		/** Upon TX done of a mail request message, it means some DL_BC was received
		 * previously and UL ACK of DL_BC was just transmitted.
		 * Send +TX= instead of +TACK=, meaning this is the real end of TX data
		 * transmission
		 */
		mac_status = MAC_TXACK_DONE;

		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		//Set_TX_LED(0);
		cbStatus = KNS_STATUS_OK;
		break;
	case (KNS_MAC_TX_TIMEOUT):
		MGR_LOG_DEBUG("MGR_AT_CMD TX_TIMEOUT callback reached\r\n");
//		MGR_LOG_DEBUG("TX TIMEOUT for usrdata (%d bits = %d bytes + %d bits): 0X",
//			srvcEvt.tx_ctxt.data_bitlen,
//			srvcEvt.tx_ctxt.data_bitlen>>3,
//			srvcEvt.tx_ctxt.data_bitlen&0x07);
//		MGR_LOG_array(srvcEvt.tx_ctxt.data, (srvcEvt.tx_ctxt.data_bitlen+7)>>3);
		kns_assert(spUserDataMsg->bIsToBeTransmit);
		/** @todo Should check integrity between data reported by event above and
		 * the one stored in user data buffer
		 *
		 * So far, as only one user data, consider this is the correct one:
		 * * notify host with AT cmd response then
		 * * free element from user data buffer.
		 */
		mac_status = MAC_TX_TIMEOUT;
		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
//		Set_TX_LED(0);
		cbStatus = KNS_STATUS_TIMEOUT;
		break;
	case (KNS_MAC_TXACK_TIMEOUT):
		MGR_LOG_DEBUG("MGR_AT_CMD TXACK_TIMEOUT callback reached\r\n");
		kns_assert(spUserDataMsg->bIsToBeTransmit);
		mac_status = KNS_MAC_TXACK_TIMEOUT;
		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		//Set_TX_LED(0);
		cbStatus = KNS_STATUS_TIMEOUT;
		break;
	case (KNS_MAC_RX_ERROR):  /**< RX error during TRX frame, report TX failure then */
		MGR_LOG_DEBUG("MGR_AT_CMD RX ERROR callback reached\r\n");
		if (spUserDataMsg->bIsToBeTransmit) {
//			MGR_LOG_DEBUG("RX enable ERROR (%d bits = %d bytes + %d bits): 0x",
//				srvcEvt.tx_ctxt.data_bitlen,
//				srvcEvt.tx_ctxt.data_bitlen>>3,
//				srvcEvt.tx_ctxt.data_bitlen&0x07);
//			MGR_LOG_array(srvcEvt.tx_ctxt.data,
//				(srvcEvt.tx_ctxt.data_bitlen+7)>>3);
			/** @todo Should check integrity between data reported by event
			 * above and the one stored in user data buffer
			 *
			 * So far, as only one user data, consider this is the correct one:
			 * * notify host with AT cmd response then
			 * * free element from user data buffer.
			 */
			mac_status = MAC_RX_ERROR;
			USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
			//Set_TX_LED(0);
			cbStatus = KNS_STATUS_TR_ERR;
		} else {
			MGR_LOG_DEBUG("MGR_AT_CMD RX callback reached\r\n");
			MGR_LOG_DEBUG("RX ERROR  unexpected event received.\r\n");
			mac_status = MAC_ERROR;
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
//		MGR_LOG_DEBUG("ERROR: no DL frm for (%d bits = %d bytes + %d bits): 0x",
//			srvcEvt.tx_ctxt.data_bitlen,
//			srvcEvt.tx_ctxt.data_bitlen>>3,
//			srvcEvt.tx_ctxt.data_bitlen&0x07);
//		MGR_LOG_array(srvcEvt.tx_ctxt.data, (srvcEvt.tx_ctxt.data_bitlen+7)>>3);
		/** @todo Should check integrity between data reported by event above and
		 * the one stored in user data buffer
		 *
		 * So far, as only one user data, consider this is the correct one:
		 * * notify host with AT cmd response then
		 * * free element from user data buffer.
		 */
		mac_status = MAC_RX_TIMEOUT;
		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		//Set_TX_LED(0);
		cbStatus = KNS_STATUS_TIMEOUT;
	break;
	case (KNS_MAC_OK):
		MGR_LOG_DEBUG("MGR_AT_CMD MAC reported OK to previous command.\r\n");
		mac_status = MAC_OK;
		if (srvcEvt.app_evt == KNS_MAC_SEND_DATA)
			//Set_TX_LED(1);
		if (srvcEvt.app_evt == KNS_MAC_STOP_SEND_DATA)
			kns_assert(USERDATA_txFifoFlush() == true);
		cbStatus = KNS_STATUS_OK;
	break;
	case (KNS_MAC_ERROR):
		MGR_LOG_DEBUG("MGR_AT_CMD MAC reported ERROR to previous command.\r\n");
		mac_status = MAC_ERROR;
		if (srvcEvt.app_evt == KNS_MAC_SEND_DATA)
			USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		cbStatus = KNS_STATUS_ERROR;
	break;
	default:
		kns_assert(0);
		mac_status = MAC_ERROR;
		cbStatus = KNS_STATUS_ERROR;
	break;
	}

	return cbStatus;
}

/**
 * @}
 */
