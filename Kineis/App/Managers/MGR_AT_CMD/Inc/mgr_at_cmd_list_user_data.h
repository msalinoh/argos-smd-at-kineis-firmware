/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list_user_data.h
 * @author  Kineis
 * @brief subset of AT commands concerning user data manipulation such as TX
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_CMD_USERDATA_H
#define __MGR_AT_CMD_USERDATA_H

/* Includes ------------------------------------------------------------------*/

#include "kns_types.h"
#include "mgr_at_cmd_common.h"

/* Public functions ----------------------------------------------------------*/

/** @brief : convert the input form ASCII to binary.
 *
 * Output data (data after conversion) will be stored in the same buffer.
 *
 * The size of the buffer will be reduced to the half. So 0 will be set onto unused Bytes.
 *
 * Example:
 *           ______________________________________________________________
 * buffer   |  'A' |  'B' |  'C' |  'D' |  'E' |  'F' |  '0' |  '1' |  'F' |
 * before   ---------------------------------------------------------------
 * buffer    ______________________________________________________________
 * after    | 0xAB | 0xCD | 0xEF | 0x01 | 0xF0 | 0x00 | 0x00 | 0x00 | 0x00 |
 *
 * * In this example: u16_charNb is "9 characters" and the return value is "36 bits".
 *
 * @param[in,out] *pu8InputBuffer: pointer to the buffer that contains ASCII data. binary data
 * will be stored into the same buffer.
 * @param[in] u16_charNb: input data buffer length in ASCII char
 *
 * @return data length in bits if everything is OK, 0 otherwise.
 */
uint16_t u16MGR_AT_CMD_convertAsciiBinary(uint8_t *pu8InputBuffer, uint16_t u16_charNb);

uint16_t u16MGR_AT_CMD_convertAsciiToInt32(uint8_t *pu8InputBuffer, uint32_t *u32OutputValue);
/**
 * @brief Process AT command "AT+TX" send user data.
 *
 * 1) "AT+TX=<HexData>[,0x<Attr>]" starts a transmission of the "HexData" with "Attr" attribute
 *
 * 2) "AT+TX=?" Mode Not supported for this command
 *
 * "HexData": the input data in Hexadecimal characters. (3 byte for VLDA4, i.e. 6 ASCII digit)
 * "Attr": attribute of the data to be transmitted. So far, attribute is defined as 8-bits-bitmap:
 * * bit0-2: service (000 no, 001 mail request, 010 ACK, ... reserved)
 * * bit3  : add back/front (0 back, 1 front)
 * * bit4-7: reserved
 *
 * @attention For VLDA4 on Kineis, data payload is 3 bytes
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_TX_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);

#ifdef USE_RX_STACK
/**
 * @brief Process AT command "AT+RX" received data. This is mainly aimed at updating AOP/CS data
 *        in case of PREPASS MAC profile, or received some DL beacon command in programmed mode.
 *
 * 1) "AT+RX=0/1" starts/stops a reception for data
 *
 * some responses could be: "+DL=" (downlink data) or "+BC" (beacon command)
 *
 * 2) "AT+RX=?" Mode Not supported for this command
 *
 * @param[in] pu8_cmdParamString: string containing AT command
 * @param[in] e_exec_mode: type of the command (status command or action command)
 *
 * @return true if command is correctly received and processed, false if error
 */
bool bMGR_AT_CMD_RX_cmd(uint8_t *pu8_cmdParamString, enum atcmd_type_t e_exec_mode);
#endif // USE_RX_STACK

/**
 * @brief Fct used to retreive and process event coming from kineis stack as answers to AT commands
 *
 * Typically, this is about processing TX events such as TX-done, TX-timeout, RX-timeout in case of
 * TRX. In case of KIM2 HW, it is also about RX events such as RX-frame-received, DL-msg-received.
 *
 * @retval KNS_STATUS_OK if TX DONE or KNS_STATUS_TIMEOUT if timeout reached, else KNS_STATUS_ERROR
 */
enum KNS_status_t MGR_AT_CMD_macEvtProcess(void);

#endif /* __MGR_AT_CMD_USERDATA_H */

/**
 * @}
 */
