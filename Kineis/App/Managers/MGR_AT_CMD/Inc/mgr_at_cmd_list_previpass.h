/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_previpass.h
 * @author  Kinéis
 * @brief subset of AT commands concerning satellite PASS predictions, usefull for Medium Acces
 *
 * @note This subset of commands are actually stubs, only present because GUI is sending such AT
 *       commands to KIM device.
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_CMD_PREVIPASS_H
#define __MGR_AT_CMD_PREVIPASS_H

/* Includes ------------------------------------------------------------------*/
#include "mgr_at_cmd_common.h"


/* Functions -----------------------------------------------------------------*/

/** @brief Process AT command "AT+PREPASS_EN" Enabling/disabling PREVIPASS (sat pass prediction)
 * computation, leading to TX/RX only during satellite pass.
 *
 * @note This command is actually a stub, used to anwser correctly to GUI. There is no functionnal
 *       impact on Kinéis SW stack so far.
 *
 * 1) "AT+PREPASS_EN=<enable_disable>" 0 disable PREPAS, 1 enable PREPAS
 * * Response format: "+OK" or "+ERROR=<error_code>" (See \ref ERROR_RETURN_T)
 *
 * 2) "AT+PREPASS_EN=?" replies: if PREPASS is enabled (1) or not (0)
 * * "+PREPASS_EN=0" if PREPASS is disabled
 * * "+PREPASS_EN=1" if PREPASS is enabled.
 *
 * "enable_disable" is an integer telling true or false
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed
 *         false if error
 */
bool bMGR_AT_CMD_PREPASS_EN_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

/** @brief Process AT command "AT+UDATE" get/set the current user date and UTC time.
 *
 * @note This command is actually a stub, used to anwser correctly to GUI. There is no functionnal
 *       impact on Kinéis SW stack so far.
 *
 * 1) "AT+UDATE=<datetime>[,updateConstStatusAge]" Setting the current time
 * Response format: "+OK" or "+ERROR=<error_code>" (See \ref ERROR_RETURN_T)
 *
 * 2) "AT+UDATE=?" returns current value of date and time
 * Response format: "+UDATE=<datetime>"
 *
 * "datetime": an ASCII string with ISO8601 format: YYYY-MM-DDThh:mm:ssZ
 *
 * @note the field "updateConstStatusAge" is an optional parameter.
 * if (u8_updateConstStatusAge == 0), then constellation status birthday must not be updated.
 * Otherwise (if it's missing or (u8_updateConstStatusAge != 0), then constellation status must
 * be updated.
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed
 *         false if error
 *
 */
bool bMGR_AT_CMD_UDATE_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#endif /* __MGR_AT_CMD_PREVIPASS_H */

/**
 * @}
 */
