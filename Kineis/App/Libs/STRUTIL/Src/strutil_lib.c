// SPDX-License-Identifier: no SPDX license
/**
 * @file    strutil_lib.c
 * @brief   Application general purpose routines for data conversion / generation
 * @author  Ahmed JEDIDI / David Panetto
 * @date creation 18/12/2013
 * @date update 18/03/2020
 * @version 1.0
 */

/**
 * @addtogroup STRUTILS
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "strutil_lib.h"

/* Functions Implementation --------------------------------------------------*/

bool bUTIL_strcmp(uint8_t *pu8_Str1, const uint8_t *pu8_Str2, uint8_t u8_strLength)
{
	bool b_retvalue = true;
	uint8_t u8_loopCounter;

	for (u8_loopCounter = 0 ; u8_loopCounter < u8_strLength ; u8_loopCounter++) {
		if (pu8_Str1[u8_loopCounter] != pu8_Str2[u8_loopCounter]) {
			b_retvalue = false;
			break;
		}
	}
	return b_retvalue;

}

uint8_t u8UTIL_convertCharToHex4bits(uint8_t u8_num)
{
	if ((u8_num >= 'A') && (u8_num <= 'F'))
		return u8_num - 'A' + 10;

	if ((u8_num >= 'a') && (u8_num <= 'f'))
		return u8_num - 'a' + 10;

	if ((u8_num >= '0') && (u8_num <= '9'))
		return u8_num - '0';

	return HEX_DEC_4BIT_CONVERSION_FAILED_CODE;
}

/**
 * @}
 */
