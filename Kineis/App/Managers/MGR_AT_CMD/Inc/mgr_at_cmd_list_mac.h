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
 * @brief Process AT command "AT+KMAC" get/set the Kineis MAC profile
 *
 * 1) "AT+KMAC=<profile ID>,<profile context>"
 * Response format: "+OK" or "+ERROR=<error_code>" (See \ref ERROR_RETURN_T)
 *
 * Profile ID:
 * * 0 none
 * * 1 basic
 * * 2 blind
 *
 * 2) "AT+KMAC=?" returns the profil ID
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_AT_CMD_KMAC_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#endif /* __MGR_AT_CMD_MAC_H */
/**
 * @}
 */
