// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_common.c
 * @author Kineis
 * @brief common part of the AT cmd manager (logging, AT cmd response api)
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "mgr_at_cmd_common.h"
#include "mcu_at_console.h"
#include "user_data.h"
#include "kns_mac.h"


/* Public functions ----------------------------------------------------------*/

bool bMGR_AT_CMD_logSucceedMsg(void)
{
	MCU_AT_CONSOLE_send("+OK\r\n");
	return true;
}

bool bMGR_AT_CMD_logFailedMsg(enum ERROR_RETURN_T eErrorType)
{
	MCU_AT_CONSOLE_send("+ERROR=%i\r\n", eErrorType);
	return false;
}

bool bMGR_AT_CMD_sendResponse(enum atcmd_rsp_type_t atcmd_response_type, void *atcmd_rsp_data)
{

	switch (atcmd_response_type) {
	case ATCMD_RSP_TXOK:
	{
		if (atcmd_rsp_data != NULL) {
			struct sUserDataTxFifoElt_t *spUserDataMsg =
						(struct sUserDataTxFifoElt_t *)atcmd_rsp_data;
			uint8_t *pu8UserDataPtr = spUserDataMsg->u8DataBuf;
			uint16_t u16UserDataBitlen = spUserDataMsg->u16DataBitLen;

			MCU_AT_CONSOLE_send("+TX=0,");
			MCU_AT_CONSOLE_send_dataBuf(pu8UserDataPtr, u16UserDataBitlen);
			MCU_AT_CONSOLE_send("\r\n");
		}
		return true;
	}
	break;
	case ATCMD_RSP_RXTIMEOUT:
	case ATCMD_RSP_TXNOTOK:
	{
		if (atcmd_rsp_data != NULL) {
			struct sUserDataTxFifoElt_t *spUserDataMsg =
						(struct sUserDataTxFifoElt_t *)atcmd_rsp_data;
			uint8_t *pu8UserDataPtr = spUserDataMsg->u8DataBuf;
			uint16_t u16UserDataBitlen = spUserDataMsg->u16DataBitLen;
			enum ERROR_RETURN_T error_id = ERROR_UNKNOWN;

			if (atcmd_response_type == ATCMD_RSP_RXTIMEOUT)
				error_id = ERROR_RX_TIMEOUT;

			MCU_AT_CONSOLE_send("+TX=%d,", error_id);
			MCU_AT_CONSOLE_send_dataBuf(pu8UserDataPtr, u16UserDataBitlen);
			MCU_AT_CONSOLE_send("\r\n");
		}
		return true;
	}
	break;
	case ATCMD_RSP_TXACKOK:
	{
		MCU_AT_CONSOLE_send("+TXACK=0\r\n");
		return true;
	}
	break;
	case ATCMD_RSP_TXACKNOTOK:
	{
		enum ERROR_RETURN_T error_id = ERROR_UNKNOWN;

		MCU_AT_CONSOLE_send("+TXACK=%d\r\n", error_id);
		return true;
	}
	break;
	case ATCMD_RSP_SATDET:
	{
		MCU_AT_CONSOLE_send("+SATDET=\r\n");
		return true;
	}
	break;
	case ATCMD_RSP_RXOK:
	{
		if (atcmd_rsp_data != NULL) {
			struct KNS_MAC_RX_frm_ctxt_t *receivedFrm =
						(struct KNS_MAC_RX_frm_ctxt_t *)atcmd_rsp_data;
			uint8_t *rxFrmDataPtr = receivedFrm->data;
			uint16_t rxFrmDataBitlen = receivedFrm->data_bitlen;

			MCU_AT_CONSOLE_send("+RX=");
			MCU_AT_CONSOLE_send_dataBuf(rxFrmDataPtr, rxFrmDataBitlen);
			MCU_AT_CONSOLE_send("\r\n");
		}
		return true;
	}
	break;
	case ATCMD_RSP_DLOK:
	{
		if (atcmd_rsp_data != NULL) {
			struct KNS_MAC_RX_frm_ctxt_t *reeceivedMsg =
						(struct KNS_MAC_RX_frm_ctxt_t *)atcmd_rsp_data;
			uint8_t *rxMsgDataPtr = reeceivedMsg->data;
			uint16_t rxMsgDataBitlen = reeceivedMsg->data_bitlen;

			MCU_AT_CONSOLE_send("+DL=");
			MCU_AT_CONSOLE_send_dataBuf(rxMsgDataPtr, rxMsgDataBitlen);
			MCU_AT_CONSOLE_send("\r\n");
		}
		return true;
	}
	break;
	default:
		break;
	}
	return false;
}

/**
 * @}
 */
