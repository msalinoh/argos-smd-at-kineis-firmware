// SPDX-License-Identifier: no SPDX license
/**
 * @file    mcu_nvm.c
 * @brief   MCU wrapper for any access to the non volatile memory accesses
 *
 * @author  shamard
 * @date    Creation 2022/11/18
 * @version 1.0
 * @note
 */

/**
 * @addtogroup MCU_WRAPPERS
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "kns_types.h"
#include "mcu_nvm.h"

/* Variables ---------------------------------------------------------*/

int16_t message_counter;

/* Functions -------------------------------------------------------------*/

enum KNS_status_t MCU_NVM_getMC(int16_t *mc_ptr)
{
	*mc_ptr = message_counter ;
	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_setMC(int16_t mcTmp)
{
	message_counter = mcTmp;
	return KNS_STATUS_OK;

}

enum KNS_status_t MCU_NVM_getID(uint32_t *id)
{
	*id = 0x22675AD0;
	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_getAddr(uint8_t addr[])
{
	addr[0] = 0x22;
	addr[1] = 0x67;
	addr[2] = 0x5A;
	addr[3] = 0xD0;
	return KNS_STATUS_OK;
}

/**
 * @}
 */
