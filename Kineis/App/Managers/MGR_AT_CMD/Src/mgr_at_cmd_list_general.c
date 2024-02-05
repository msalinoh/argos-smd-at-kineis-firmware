// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_list_general.c
 * @author  Kineis
 * @brief subset of AT commands concerning general purpose (get ID, FW version, ...)
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "kns_types.h"
#include "mgr_at_cmd_common.h"
#include "mgr_at_cmd_list_general.h"
#include "mgr_at_cmd_list_user_data.h"
#include "mcu_at_console.h"
/* @todo PRODEV-68: remove specific flag when HW setting check is implemented on all platforms */
#if defined(KRD_FW_MP) ||  defined(KRD_FW_LP)
#include "mcu_misc.h" // use to check RF HW setting vers radio confoguration
#endif
#include "kns_cfg.h"
#include "build_info.h"
#include "mgr_log.h"


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

bool bMGR_AT_CMD_ADDR_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	uint8_t dev_addr[DEVICE_ADDR_LENGTH];

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		if (KNS_CFG_getAddr(dev_addr) != KNS_STATUS_OK)
			/* TODO: add a new error code ? */
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);
		MCU_AT_CONSOLE_send("+ADDR=%02x%02x%02x%02x\r\n", dev_addr[0], dev_addr[1],
								  dev_addr[2], dev_addr[3]);

		return true;
	}
	return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
}

bool bMGR_AT_CMD_ID_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	uint32_t dev_id;

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		if (KNS_CFG_getId(&dev_id) != KNS_STATUS_OK)
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);
		/* ID is printed as a number, with decimal representation.
		 * ID is stored in memory in little endian format.
		 */
		MCU_AT_CONSOLE_send("+ID=%d\r\n", dev_id);

		return true;
	}
	return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
}

bool bMGR_AT_CMD_SN_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	uint8_t dev_sn[DEVICE_SN_LENGTH + 1];

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		if (KNS_CFG_getSN(dev_sn) != KNS_STATUS_OK)
			/* TODO: add a new error code ? */
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);
		dev_sn[DEVICE_SN_LENGTH] = '\0';
		MCU_AT_CONSOLE_send("+SN=%s\r\n", dev_sn);

		return true;
	}
	return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
}

bool bMGR_AT_CMD_RCONF_cmd(uint8_t *pu8_cmdParamString __attribute__((unused)),
	enum atcmd_type_t e_exec_mode)
{
	struct KNS_CFG_radio_t radio_cfg;
	uint8_t modulation[8];
	uint16_t nbBits;
#if defined(KRD_FW_MP) ||  defined(KRD_FW_LP)
	struct rfSettings_t hwSettings;
#endif

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		if (KNS_CFG_getRadioInfo(&radio_cfg) != KNS_STATUS_OK)
			/* TODO: add a new error code ? */
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);
		if (radio_cfg.modulation == KNS_TX_MOD_LDA2)
			strcpy((char*)modulation, "LDA2");
		else if (radio_cfg.modulation == KNS_TX_MOD_LDA2L)
			strcpy((char*)modulation, "LDA2L");
		else if (radio_cfg.modulation == KNS_TX_MOD_VLDA4)
			strcpy((char*)modulation, "VLDA4");
		else if (radio_cfg.modulation == KNS_TX_MOD_HDA4)
			strcpy((char*)modulation, "HDA4");
		else if (radio_cfg.modulation == KNS_TX_MOD_LDK)
			strcpy((char*)modulation, "LDK");
		else
			strcpy((char*)modulation, "UNKNOWN");
		MCU_AT_CONSOLE_send("+RCONF=%u,%u,%d,%s\r\n", radio_cfg.min_frequency,
			radio_cfg.max_frequency, radio_cfg.rf_level, modulation);

		/** @todo PRODEV-68: check radio conf versus HW settings for other platforms */
#if defined(KRD_FW_MP) ||  defined(KRD_FW_LP)
		if (MCU_MISC_getSettingsHwRf(radio_cfg.rf_level, &hwSettings) == KNS_STATUS_OK)
			return bMGR_AT_CMD_logSucceedMsg();
		return bMGR_AT_CMD_logFailedMsg(ERROR_INCOMPATIBLE_VALUE);
#else
		return bMGR_AT_CMD_logSucceedMsg();
#endif
	}
	if (e_exec_mode == ATCMD_ACTION_MODE) {
		while(pu8_cmdParamString[strlen((char*)pu8_cmdParamString) - 1] == '\r' ||
			pu8_cmdParamString[strlen((char*)pu8_cmdParamString) - 1] == '\n')
			pu8_cmdParamString[strlen((char*)pu8_cmdParamString) - 1] = '\0';
		nbBits = u16MGR_AT_CMD_convertAsciiBinary(pu8_cmdParamString + 9,
			strlen((char*)(pu8_cmdParamString + 9)));

		if (nbBits != 128)
			/* TODO: add a new error code ? */
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);

		if (KNS_CFG_setRadioInfo(pu8_cmdParamString + 9) != KNS_STATUS_OK)
			/* TODO: add a new error code ? */
			return bMGR_AT_CMD_logFailedMsg(ERROR_INVALID_ID);
		return bMGR_AT_CMD_logSucceedMsg();
	}
	return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN_AT_CMD);
}

/**
 * @}
 */
