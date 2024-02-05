// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_list.c
 * @author  Kineis
 * @brief AT commands list main file
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "mgr_at_cmd_common.h"
#include "mgr_at_cmd_list.h"
#include "mgr_at_cmd_list_general.h"
#include "mgr_at_cmd_list_user_data.h"
#include "mgr_at_cmd_list_previpass.h"
#include "mgr_at_cmd_list_mac.h"

const struct atcmd_desc_t cas_atcmd_list_array[ATCMD_MAX_COUNT] = {
	/**< General commands */
	{ "AT+PING",          7, bMGR_AT_CMD_PING_cmd},
	{ "AT+FW",            5, bMGR_AT_CMD_FW_cmd},
	{ "AT+ADDR",          7, bMGR_AT_CMD_ADDR_cmd},
	{ "AT+ID",            5, bMGR_AT_CMD_ID_cmd},
	{ "AT+SN",            5, bMGR_AT_CMD_SN_cmd},
	{ "AT+RCONF",         8, bMGR_AT_CMD_RCONF_cmd},

	/**< User data commands */
	{ "AT+TX",            5, bMGR_AT_CMD_TX_cmd},
#ifdef KIMX_FW
	{ "AT+RX",            5, bMGR_AT_CMD_RX_cmd},
#endif

	/**< Satellite pass predictions commands (not functionnal, only to avoid GUI to crash) */
	{ "AT+PREPASS_EN",   13, bMGR_AT_CMD_PREPASS_EN_cmd},
	{ "AT+UDATE",         8, bMGR_AT_CMD_UDATE_cmd},

	/**< MAC commands (not functionnal, only to avoid GUI to crash) */
	{ "AT+ATXRP",         8, bMGR_AT_CMD_ATXRP_cmd},
};

/**
 * @}
 */
