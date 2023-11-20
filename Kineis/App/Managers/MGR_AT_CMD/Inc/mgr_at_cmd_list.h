/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list.h
 * @author  Kinéis
 * @brief AT commands list header file
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_CMD_CONF_H
#define __MGR_AT_CMD_CONF_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "mgr_at_cmd_common.h"

/* Defines -------------------------------------------------------------------*/

/** Indexes for AT commands present in system */
enum  atcmd_idx_t {
	// General commands
	AT_PING,         /**< Ping commands */
	AT_FW,           /**< get fw version commands */
	AT_ID,           /**< get ID version commands */

	// User data commands
	AT_TX,           /**< Index for TX commands */
#ifdef KIMX_FW
	AT_RX,           /**< Index for TX commands */
#endif

	// Satellite pass predictions commands
	AT_PREPASS_EN,   /**< Index for get/set PREVIPASS algo */
	AT_UDATE,        /**< Index for UTC date/time update */

	// MAC commands
	AT_ATXRP,        /**< Index for get/set tx repetition  commands */

	ATCMD_MAX_COUNT,
	ATCMD_UNKNOWN_COMMAND = ATCMD_MAX_COUNT
};


/* Types ---------------------------------------------------------------------*/

/** Function entry point to process action/status commands. */
typedef bool (*pvATCMD_cmd_proc_fun_type)(uint8_t *pu8_cmdParamString,
		enum atcmd_type_t e_exec_mode);

/** Elementary structure for each AT command to hold necessary information
 * to handle action/status/information sub commands type
 */
struct atcmd_desc_t {
	/**<Name of the command */
	const char *pu8_cmdNameString;

	/**< Min input command length included AT name.
	 * For example AT basic command will have length=2,
	 * and AT+TRUN a length=7
	 */
	uint8_t u8_cmdNameLen;

	/**< Command function entry point to process action/status cmd type
	 * AT+XXXX=... or AT+XXXX=?
	 */
	pvATCMD_cmd_proc_fun_type f_ht_cmd_fun_proc;

};

/* Extern ---------------------------------------------------------------------------------------*/
extern const struct atcmd_desc_t cas_atcmd_list_array[];



#endif /* __MGR_AT_CMD_CONF_H */
/**
 * @}
 */
