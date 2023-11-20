// SPDX-License-Identifier: no SPDX license
/**
 * @file mgr_at_cmd_list_mac.c
 * @author  Kinéis
 * @brief subset of AT commands concerning Kinéis Medium Acces Channel (MAC).
 *
 * @note This subset of commands are actually stubs, only present because GUI is sending such AT
 *       commands to KIM device.
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "mcu_at_console.h"
#include "mgr_at_cmd_list_mac.h"

/* Functions -----------------------------------------------------------------*/

bool bMGR_AT_CMD_ATXRP_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode)
{
	int16_t i16_scan_param_res;
	int16_t i16_nb_repetition = 0;
	uint32_t u32timeBetweenSendMs = 0;

	if (e_exec_mode == ATCMD_STATUS_MODE) {
		MCU_AT_CONSOLE_send("+ATXRP=%d,%ld\r\n ", i16_nb_repetition, u32timeBetweenSendMs);
		return true;
	}

	/* Scan the Transmission repetition for one message
	 * and the time (in ms) between two Transmission
	 */
	i16_scan_param_res = sscanf((const char *)pu8_cmdParamString,
			(const char *)"AT+ATXRP=%hd,%lu",
			&i16_nb_repetition,
			&u32timeBetweenSendMs);

	/* Only one Tr parameter */
	if (i16_scan_param_res == 0)
		/* Tr and pmt_mode parameters */
		return bMGR_AT_CMD_logFailedMsg(ERROR_PARAMETER_FORMAT);

	if (i16_scan_param_res == 1) {
		/* Do not allow "AT+ATXRP=<NbRp>," or "AT+ATXRP=<NbRp>,<somedata>
		 * where "somedata" is not an integer.
		 */
		if (strchr((const char *)pu8_cmdParamString, ',') != NULL)
			return bMGR_AT_CMD_logFailedMsg(ERROR_PARAMETER_FORMAT);

		return bMGR_AT_CMD_logSucceedMsg();
	} else if (i16_scan_param_res == 2) {
		return bMGR_AT_CMD_logSucceedMsg();
	}

	return bMGR_AT_CMD_logFailedMsg(ERROR_UNKNOWN);
}

/**
 * @}
 */
