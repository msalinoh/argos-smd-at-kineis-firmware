// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_list_general.c
 * @author  Kinéis
 * @brief subset of AT commands concerning general purpose (get ID, FW version, ...)
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "mgr_at_cmd_list_general.h"
#include "mcu_at_console.h"
#include "kns_cfg.h"

#include "build_info.h"


/* Functions -----------------------------------------------------------------*/

bool bMGR_AT_CMD_PING_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	if (e_exec_mode == ATCMD_ACTION_MODE) {
		MGR_LOG_VERBOSE("[ERROR] Action mode is unauthorized for this AT cmd\r\n");
		return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
	}
	return bMGR_AT_CMD_logSucceedMsg();
}

bool bMGR_AT_CMD_FW_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	if (e_exec_mode == ATCMD_ACTION_MODE) {
		MGR_LOG_VERBOSE("[ERROR] Action mode is unauthorized for this AT cmd\r\n");
		return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
	}

	MCU_AT_CONSOLE_send("+FW=%s,%s_%s\r\n", uc_fw_vers_commit_id, __DATE__, __TIME__);

	return true;
}

bool bMGR_AT_CMD_ID_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	uint8_t pmt_add[4] = {0};
	uint32_t pmt_id;

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		if ((KNS_CFG_GetAddr(pmt_add) != KNS_STATUS_OK) ||
		    (KNS_CFG_GetIdDec(&pmt_id) != KNS_STATUS_OK))
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);
		MCU_AT_CONSOLE_send("+ID=%02x%02x%02x%02x\r\n", pmt_add[0], pmt_add[1], pmt_add[2],
			pmt_add[3]);
		MCU_AT_CONSOLE_send("ID_DEC=%d\r\n", pmt_id);

		return true;
	}
	return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
}

/**
 * @}
 */
