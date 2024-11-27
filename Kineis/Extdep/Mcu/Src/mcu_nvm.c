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
#include "aes.h"
#include "mcu_aes.h"
#include "mcu_nvm.h"
#include <stdbool.h>

/* Variables ---------------------------------------------------------*/

/** @note The message counter be stored in a non volatile memory (flash, RTC backup reg, etc.).
 * Proposed implementation below is mapping the message_counter variable in a memory section:
 * * Ensure .msgCntSectionData section is mapped at the correct place
 * * or fill free to change this proposal as per you needs.
 *
 * It could remain in RAM as long as the device is not POWER OFF and is not entering LPM.
 * The message counter value should be recovered after each device power off/on and low power wakeup
 *
 * If you use a flash memory, be careful about the storage of this data. The message counter is
 * updated at each message transmission, so this implies a lot of erase/write cycles.
 */
__attribute__((__section__(".msgCntSectionData")))
static uint16_t message_counter;

__attribute__((__section__(".radioConfFlag")))
static bool radioConfZoneSavedFlag;

static
__attribute__((__section__(".radioConfSection")))
uint32_t radioConfZoneSaved[16];

/** The device identifier may be stored in a secured way (encryption, etc.) */
static const uint32_t device_id = 214012;

/** The device address may be stored in a secured way (encryption, etc.) */
static const uint8_t device_addr[4] = { 0x22, 0x67, 0x5C, 0x70 };

/** Radio configuration : Ensure radio configuration is valid when Kineis stack is called.
 *
 * Depending on the application, before calling the stack, you can:
 * * set it in your application through MCU_NVM_setRadioConfZone()
 * * load it in RAM at startup
 * * point directly on it in flash memory...
 *
 * Several radio configurations are listed in table below. Uncomment/comment the one you want to
 * use/not use.
 */
static uint8_t radioConfZone[16] = {

#ifdef KRD_FW_LP
	/** ---- radio configuration at 22 dBm instead of 27 dBm, needed for KRD_LP platform ---- */

	/** ---- 401625000,401635000,22,LDA2 ---- */
	0xb2, 0x64, 0x9b, 0x2c, 0x75, 0x6c, 0x93, 0xae,
	0xd8, 0xf9, 0xb3, 0x88, 0xe9, 0x06, 0x12, 0xcf,

	/** ---- 401625000,401635000,22,LDA2L ---- */
//	0x32, 0x15, 0xe2, 0x07, 0x5a, 0xe7, 0x2c, 0xdd,
//	0x16, 0xd4, 0x75, 0x88, 0xfd, 0x65, 0xe7, 0x62,

	/** ---- 401625000,401635000,22,VLDA4 ---- */
//	0xef, 0xd2, 0x41, 0x2f, 0x85, 0x70, 0x58, 0x14,
//	0x57, 0xf2, 0xd9, 0x82, 0xe7, 0x6d, 0x44, 0xd7,

	/** ---- 401625000,401635000,22,LDK ---- */
//	0xbd, 0x14, 0xce, 0x0d, 0xc6, 0x69, 0x75, 0xc5,
//	0x40, 0x28, 0xc3, 0xf6, 0x7f, 0x9d, 0x18, 0x94,

#else
	/** ---- 401625000,401635000,27,LDA2 ---- */
	0x44, 0xcd, 0x3a, 0x29, 0x90, 0x68, 0x29, 0x2a,
	0x74, 0xd2, 0x12, 0x6f, 0x34, 0x02, 0x61, 0x0d

	/** ---- 401625000,401635000,27,LDA2L ---- */
//	0xbd, 0x17, 0x65, 0x35, 0xb3, 0x94, 0xa6, 0x65,
//	0xbd, 0x86, 0xf3, 0x54, 0xc5, 0xf4, 0x24, 0xfb

	/** ---- 401625000,401635000,27,VLDA4 ---- */
//	0x82, 0xd0, 0x7f, 0x9d, 0x9c, 0xe0, 0x81, 0xee,
//	0x44, 0x92, 0x98, 0x36, 0x72, 0xd7, 0x54, 0x93

	/** ---- 401625000,401635000,27,LDK ---- */
//	0x41, 0xbc, 0x11, 0xb8, 0x98, 0x0d, 0xf0, 0x1b,
//	0xa8, 0xb4, 0xb8, 0xf4, 0x10, 0x99, 0x62, 0x0b
#endif

};

/* Device serial number */
static const uint8_t device_sn[DEVICE_SN_LENGTH] = { 'S', 'M', 'D', '_', '1', '0', '_', \
					      '_', 'T', 'E', 'S', 'T', '0', '2' };
/* Functions -------------------------------------------------------------*/

enum KNS_status_t MCU_NVM_getMC(uint16_t *mc_ptr)
{
	*mc_ptr = message_counter;

	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_setMC(uint16_t mcTmp)
{
	message_counter = mcTmp;

	return KNS_STATUS_OK;

}

enum KNS_status_t MCU_NVM_getRadioConfZonePtr(void **ConfZonePtr)
{
	if (radioConfZoneSavedFlag)
	{
		// Set ConfZonePtr to point to the appropriate memory location
		unsigned int i;
		for(i = 0; i < sizeof(radioConfZoneSaved)/ sizeof(uint32_t) ; i++)
		{
			radioConfZone[i] = (uint8_t)(radioConfZoneSaved[i]);
		}
	}


		*ConfZonePtr = radioConfZone;

	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_setRadioConfZone(void *ConfZonePtr, uint16_t ConfZoneSize)
{
	int i;

	if (sizeof(radioConfZone) > ConfZoneSize)
		return KNS_STATUS_ERROR;

	for (i = 0 ; i < ConfZoneSize ; i++)
	{
		radioConfZone[i] = ((uint8_t*)ConfZonePtr)[i];
		radioConfZoneSaved[i] = (uint32_t)((uint8_t*)ConfZonePtr)[i];
	}


	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_saveRadioConfZone(void)
{

	radioConfZoneSavedFlag = true;

	return KNS_STATUS_OK;
}


enum KNS_status_t MCU_NVM_getID(uint32_t *id)
{
	*id = device_id;

	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_getAddr(uint8_t addr[])
{
	uint16_t i;

	for (i = 0 ; i < 4 ; i++)
		addr[i] = device_addr[i];

	return KNS_STATUS_OK;
}

enum KNS_status_t MCU_NVM_getSN(uint8_t sn[])
{
    uint16_t i;

    for (i = 0 ; i < DEVICE_SN_LENGTH ; i++)
        sn[i] = device_sn[i];

    return KNS_STATUS_OK;
}

/**
 * @}
 */
