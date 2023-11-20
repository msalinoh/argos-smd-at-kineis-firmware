/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_mac.h
 * @author Kinéis
 * @brief subset of AT commands concerning Kinéis Medium Acces Channel (MAC).
 *
 * @note This subset of commands are actually stubs, only present because GUI is sending such AT
 *       commands to KIM device.
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */


#ifndef __MGR_AT_CMD_MAC_H
#define __MGR_AT_CMD_MAC_H

/* Includes ------------------------------------------------------------------*/

#include "mgr_at_cmd_common.h"

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Process AT command "AT+ATXRP" get/set the repetition number and the period between 2
 * transmissions
 *
 * @note This command is actually a stub, used to anwser correctly to GUI. There is no functionnal
 *       impact on Kinéis SW stack so far.
 *
 * 1) "AT+ATXRP=<rep_number[,period_ms>]"
 * Response format: "+OK" or "+ERROR=<error_code>" (See \ref ERROR_RETURN_T)
 *
 * 2) "AT+ATXRP=?" returns the number of transmissions and period between 2 TX.
 * Response format: "+ATXRP=<rep_number>[,<period_ms>]"
 *
 * "rep_number" is the repetition number of the transmission for one message.
 *
 * "period_ms" is the time between two transmissions in milliseconds .
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_AT_CMD_ATXRP_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#endif /* __MGR_AT_CMD_MAC_H */
/**
 * @}
 */
