// SPDX-License-Identifier: no SPDX license
/**
 * @file    mcu_misc.c
 * @brief   File to implement miscellaneous features for a specific design
 * @author  Kineis
 * @date    Creation 2023/07/06
 * @version 1.0
 * @note
 */

/**
 * @addtogroup MCU_MISC
 * @{
 */

#include <stdbool.h>
#include "mcu_misc.h"
#include "main.h"
#include "mgr_log.h"

/* Functions ------------------------------------------------------------------------------------ */

void MCU_MISC_turn_on_pa()
{
#ifdef KRD_FW_MP
	HAL_GPIO_WritePin(PA_PSU_EN_GPIO_Port, PA_PSU_EN_Pin, GPIO_PIN_SET);
        HAL_Delay(10);
#endif
}

void MCU_MISC_turn_off_pa()
{
#ifdef KRD_FW_MP
	HAL_GPIO_WritePin(PA_PSU_EN_GPIO_Port, PA_PSU_EN_Pin, GPIO_PIN_RESET);
#endif
}

enum KNS_status_t MCU_MISC_getSettingsHwRf(int8_t rf_level_dBm, void* rfSettings)
{
	struct rfSettings_t *settings = (struct rfSettings_t *)rfSettings;
	int8_t rficPaLevel, rficPaLevel_min, rficPaLevel_max;


#if defined(KRD_FW_MP)
	rficPaLevel_min = -17; // dBm
	rficPaLevel_max = 15;  // dBm

	settings->isRficHPA = false;
	settings->externalPaGain = 36;
#elif defined(KRD_FW_LP)
	rficPaLevel_min = -9; // dBm
	rficPaLevel_max = 22; // dBm

	settings->isRficHPA = true;
	settings->externalPaGain = 0;
#endif
	rficPaLevel = rf_level_dBm - settings->externalPaGain;

	MGR_LOG_VERBOSE("[MCU_MISC] RFIC PA select (0:LPA, 1:HPA) = %d\r\n",
		settings->isRficHPA);
	MGR_LOG_VERBOSE("[MCU_MISC] RFIC PA level=%d dBm, external PA gain = %d dB\r\n",
		rficPaLevel,
		settings->externalPaGain);

	/* check settings */
	if ((rficPaLevel >= rficPaLevel_min) && (rficPaLevel <= rficPaLevel_max))
		settings->rficPaLevel = rficPaLevel;
	else
		return KNS_STATUS_BAD_SETTING;

	return KNS_STATUS_OK;
}

/**
 * @}
 */
