/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_certif.h
 * @author  Kinéis
 * @brief subset of AT commands concerning specifics for certification
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_CMD_CERTIF_H
#define __MGR_AT_CMD_CERTIF_H

/* Includes ------------------------------------------------------------------*/

/**
 * @brief Process AT command "AT+CW"
 *
 * 1)
 * "AT+CW=0,399950000,27" stops the wave
 * "AT+CW=1,399950000,27" starts a continuous CW
 * "AT+CW=2,399950000,27" starts a MW in LDA2  modulation (burst chained quickly)
 * "AT+CW=3,399950000,27" starts a MW in LDA2L modulation (burst chained quickly)
 * "AT+CW=4,399950000,27" starts a MW in VLDA4 modulation (burst chained quickly)
 * "AT+CW=5,399950000,27" starts a MW in LDK   modulation (burst chained quickly)
 *
 * 2) "AT+CW=?" Mode Not supported for this command
 *
 * @param[in] 0,1, 2, 3, 4, 5 : OFF, ON as described above
 * @param[in] Freq in Hz
 * @param[in] Power in dBm
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_CW_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#endif /* __MGR_AT_CMD_CERTIF_H */

/**
 * @}
 */
