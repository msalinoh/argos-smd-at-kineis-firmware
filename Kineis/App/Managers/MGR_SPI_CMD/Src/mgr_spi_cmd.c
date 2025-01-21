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
// #include "mgr_at_cmd_list.h"
#include "kineis_sw_conf.h"
#include KINEIS_SW_ASSERT_H
#include "mgr_log.h"
#include "mcu_nvm.h"
#include "mgr_at_cmd_list_user_data.h"
#include "kns_mac.h"
#include "kns_q.h"
#include "user_data.h"
#include "mcu_spi_driver.h"
#include "mgr_spi_cmd_common.h"
#include "mgr_spi_cmd_list_general.h"
#include "mgr_spi_cmd_list.h"
#include "kns_cfg.h"
/* Defines --------------------------------------------------------------------------------------*/


/* Structure Declaration ------------------------------------------------------------------------*/
/* Private variables ----------------------------------------------------------------------------*/
SpiState spiState = SPICMD_INIT;
MACStatus macStatus = MAC_OK;
CmdValue cmdInProgress = CMD_NONE;
//spiState state = INIT;
/* Private functions ----------------------------------------------------------------------------*/
bool handleInit();
bool handleIdle();
bool handleProcessCmd();
bool handleWaitingRx();
bool handleWaitingTx();
bool handleWaitingMacEvt();
bool handleError();
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
	if (rx->size > 0)
	{
		cmdInProgress = rx->data[0];
		spiState = SPICMD_PROCESS_CMD;
	}
	else {
		MGR_LOG_DEBUG("%s: RX size less than 0\n\r",__func__);
		cmdInProgress = CMD_NONE;
		spiState = SPICMD_IDLE;
	}

	return cmdInProgress;
}



static uint8_t MGR_SPI_CMD_process_cmd(uint8_t cmd)
{
	bool ret = true;
	if ((cmd > 0) && cmd < SPICMD_MAX_COUNT)
	{
		ret = cas_spicmd_list_array[cmd].f_ht_cmd_fun_proc(&rxBuf, &txBuf);

	} else {
		MGR_LOG_DEBUG("NONE/Unknown command %u", cmd);
		rxBuf.next_req = 1;
		bMGR_SPI_DRIVER_wait_next();
	}
//			break;
//		case CMD_READ_CW:
//			MGR_LOG_DEBUG("Handling READ_CW command : TBD\n\r");
//			break;
//		case CMD_WRITE_CW_REQ:
//			MGR_LOG_DEBUG("Handling WRITE_CW_REQ command : TBD\n\r");
//			break;
//		case CMD_WRITE_CW:
//			MGR_LOG_DEBUG("Handling WRITE_CW command : TBD\n\r");
//			break;
//		case CMD_READ_PREPASSEN:
//			MGR_LOG_DEBUG("Handling READ_PREPASSEN command : TBD\n\r");
//			break;
//		case CMD_WRITE_PREPASSEN_REQ:
//			MGR_LOG_DEBUG("Handling WRITE_PREPASSEN_REQ command : TBD\n\r");
//			break;
//		case CMD_WRITE_PREPASSEN:
//			MGR_LOG_DEBUG("Handling WRITE_CW command : TBD\n\r");
//			break;
//		case CMD_READ_UDATE:
//			MGR_LOG_DEBUG("Handling READ_UDATE command : TBD\n\r");
//			break;
//		case CMD_WRITE_UDATE_REQ:
//			MGR_LOG_DEBUG("Handling WRITE_UDATE_REQ command : TBD\n\r");
//			break;
//		case CMD_WRITE_UDATE:
//			MGR_LOG_DEBUG("Handling WRITE_UDATE command : TBD\n\r");
//			break;
//		default:
//			MGR_LOG_DEBUG("Unknown command: 0x%02X\n\r", cmd);
//			rxBuf.next_req = 1;
//			ret = bMGR_SPI_DRIVER_wait_next(cmd);
//			break;
//	}

	return (ret);
}

/* Functions ------------------------------------------------------------------------------------*/

bool MGR_SPI_CMD_start(void *context)
{
	//spi handler stored and receive interrupt one byte running
	bool ret = MCU_SPI_DRIVER_register(context, MGR_SPI_CMD_parseStreamCb);
	if (ret) {
		spiState = SPICMD_IDLE;
	} else {
		spiState = SPICMD_ERROR;
	}
	return ret;
}


void MGR_SPI_CMD_state_handler() {
   switch (spiState) {
       case SPICMD_INIT:
	   	   // Only in case of restart service and spi ptr should not be changed.
		   if (handleInit())
		   {
				spiState = SPICMD_IDLE;
		   } else {
				spiState = SPICMD_ERROR;
		   }
           break;
       case SPICMD_IDLE:
    	   // start RX IT for waiting command:

           break;
       case SPICMD_PROCESS_CMD:
			MGR_SPI_CMD_process_cmd(cmdInProgress);
           break;
       case SPICMD_WAITING_RX:
           break;
       case SPICMD_WAITING_TX:
           break;
       case SPICMD_ERROR:
           break;
       default:
           printf("Unknown spiState\n");
           break;
   }
}


bool handleInit() 
{
	return (MGR_SPI_CMD_start(NULL)); // Pass NULL since the spi handle should already be initialized and passed in.
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
			macStatus = MAC_RX_TIMEOUT;
		break;
		case (KNS_MAC_ERROR):
			if (srvcEvt.app_evt == KNS_MAC_SEND_DATA) {
				spUserDataMsg = USERDATA_txFifoFindPayload(srvcEvt.tx_ctxt.data,
					srvcEvt.tx_ctxt.data_bitlen);
				kns_assert(spUserDataMsg != NULL);
				macStatus = MAC_ERROR;
			}
		break;
		default:
		break;
	}

	/** process event */
	switch (srvcEvt.id) {
	case (KNS_MAC_TX_DONE):
		MGR_LOG_DEBUG("MGR_SPI_CMD TX_DONE callback reached\r\n");
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
			macStatus = MAC_TX_DONE;
			USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
			//Set_TX_LED(0);
		}
		cbStatus = KNS_STATUS_OK;
		break;
	case (KNS_MAC_TXACK_DONE):
		MGR_LOG_DEBUG("MGR_SPI_CMD TXACK_DONE callback reached\r\n");
		kns_assert(spUserDataMsg->bIsToBeTransmit);
		/** Upon TX done of a mail request message, it means some DL_BC was received
		 * previously and UL ACK of DL_BC was just transmitted.
		 * Send +TX= instead of +TACK=, meaning this is the real end of TX data
		 * transmission
		 */
		macStatus = MAC_TXACK_DONE;

		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		//Set_TX_LED(0);
		cbStatus = KNS_STATUS_OK;
		break;
	case (KNS_MAC_TX_TIMEOUT):
		MGR_LOG_DEBUG("MGR_SPI_CMD TX_TIMEOUT callback reached\r\n");
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
		macStatus = MAC_TX_TIMEOUT;
		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
//		Set_TX_LED(0);
		cbStatus = KNS_STATUS_TIMEOUT;
		break;
	case (KNS_MAC_TXACK_TIMEOUT):
		MGR_LOG_DEBUG("MGR_SPI_CMD TXACK_TIMEOUT callback reached\r\n");
		kns_assert(spUserDataMsg->bIsToBeTransmit);
		macStatus = MAC_TXACK_TIMEOUT;
		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		//Set_TX_LED(0);
		cbStatus = KNS_STATUS_TIMEOUT;
		break;
	case (KNS_MAC_RX_ERROR):  /**< RX error during TRX frame, report TX failure then */
		MGR_LOG_DEBUG("MGR_SPI_CMD RX ERROR callback reached\r\n");
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
			macStatus = MAC_RX_ERROR;
			USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
			//Set_TX_LED(0);
			cbStatus = KNS_STATUS_TR_ERR;
		} else {
			MGR_LOG_DEBUG("MGR_SPI_CMD RX callback reached\r\n");
			MGR_LOG_DEBUG("RX ERROR  unexpected event received.\r\n");
			macStatus = MAC_ERROR;
			cbStatus = KNS_STATUS_ERROR;
		}
	break;
	case (KNS_MAC_RX_TIMEOUT): /**< RX window reached during TRX, report failure then */
		/** @attention so far, TX failure is reported upon TRX RX timeout when:
		 * * no DL-ACK received for a TX requesting ACK (this case is fine)
		 * * no DL-BC received for a TX mail request. Actually this may really occur
		 * as no BC was available on board (protocol to be discussed)
		 */
		MGR_LOG_DEBUG("MGR_SPI_CMD RX timeout callback reached\r\n");
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
		macStatus = MAC_RX_TIMEOUT;
		USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		//Set_TX_LED(0);
		cbStatus = KNS_STATUS_TIMEOUT;
	break;
	case (KNS_MAC_OK):
		MGR_LOG_DEBUG("MGR_SPI_CMD MAC reported OK to previous command.\r\n");
		macStatus = MAC_OK;
		if (srvcEvt.app_evt == KNS_MAC_SEND_DATA)
			//Set_TX_LED(1);
		if (srvcEvt.app_evt == KNS_MAC_STOP_SEND_DATA)
			kns_assert(USERDATA_txFifoFlush() == true);
		cbStatus = KNS_STATUS_OK;
	break;
	case (KNS_MAC_ERROR):
		MGR_LOG_DEBUG("MGR_SPI_CMD MAC reported ERROR to previous command.\r\n");
		macStatus = MAC_ERROR;
		if (srvcEvt.app_evt == KNS_MAC_SEND_DATA)
			USERDATA_txFifoRemoveElt(spUserDataMsg);/* Free as host notified */
		cbStatus = KNS_STATUS_ERROR;
	break;
	default:
		kns_assert(0);
		macStatus = MAC_ERROR;
		cbStatus = KNS_STATUS_ERROR;
	break;
	}

	return cbStatus;
}

/**
 * @}
 */
