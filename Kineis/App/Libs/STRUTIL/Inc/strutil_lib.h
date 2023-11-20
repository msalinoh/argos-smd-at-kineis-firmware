/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    strutil_lib.h
 * @brief   small utilities to manipulate strings
 * @author  Kinéis
 */

/**
 * \page strutil_lib_page String utils library
 *
 * This is the library containing utils to manipulate string data.
 *
 * Several software components are manipulating input string and output on UART.
 * Those utils are usefull to do parsing, format or find a string.
 */

/**
 * @addtogroup STRUTILS
 * @brief  String utils library. (refer to \ref strutil_lib_page page for general description).
 * @{
 */

#ifndef __STRLIB_H
#define __STRLIB_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Private defines -----------------------------------------------------------*/

/** Value returned by \ref u8UTIL_convertCharToHex4bits on failing conversion */
#define HEX_DEC_4BIT_CONVERSION_FAILED_CODE   0xFF

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Compare 2 strings
 *
 * This is equivalent to strncmp
 *
 * @param [in] pu8_Str1 : Pointer to first string
 * @param [in] pu8_Str2 : Pointer to second string
 * @param [in] u8_strLength : maximum number of chars to compare
 *
 * @return true if match, else false
 */
bool bUTIL_strcmp(uint8_t *pu8_Str1, const uint8_t *pu8_Str2, uint8_t u8_strLength);

/**
 * @brief convert an hexadecimal ASCII digit to a 4bits integer
 *
 * @param [in] u8_num : the ASCII value to be converted
 *
 * @return value on 4bits or \ref HEX_DEC_4BIT_CONVERSION_FAILED_CODE if input is not valid.
 */
uint8_t u8UTIL_convertCharToHex4bits(uint8_t u8_num);

#endif /* __STRLIB_H */

/**
 * @}
 */
