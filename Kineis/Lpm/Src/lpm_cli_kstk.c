// SPDX-License-Identifier: no SPDX license
/**
 * @file    lpm_cli_kstk.c
 * @brief   Kineis stack's LPM client. It is implementing APIs needed to interface with the low
 *          power manager (MGR_LPM)
 * @author  Kineis
 */

/**
 * @addtogroup MGR_LPM
 * @{
 */

/* Includes ------------------------------------------------------------------------------------ */
#include <stdbool.h>
#include "lpm_cli_kstk.h"
#include "mgr_lpm.h"
#include "kns_mac.h"

/* Variables ----------------------------------------------------------------------------------- */

struct MgrLpmClientCb_t mgrLpmCliKstk =
{     .fpMGR_LPM_LpmReqCb        = KSTK_lpmReq,
      .fpMGR_LPM_LpmNotifEnterCb = KSTK_lpmNotifEnter,
      .fpMGR_LPM_LpmNotifExitCb  = KSTK_lpmNotifExit
};

/* Functions ----------------------------------------------------------------------------------- */

enum MgrLpm_LPM_t KSTK_lpmReq(void)
{
	union KNS_MAC_rsrcStatus_t rsrc;

	rsrc = KNS_MAC_getRsrcStatus();

	/** On STM32WL55C + kineis stack, possible LPM strategy is:
	 * * RUNTIME when woken-up to:
	 *     * configure/start the SUBGHZ RF through SPI bus
	 *     * start the TX- TIMEOUT timer
	 * * LPSLEEP or STOP during SUBGHZ transmission, until TX DONE or until TX TIMEOUT
	 * * STANDBY in any other situation (RTC timer is used for periodical transmission
	 * * SHUTDOWN when there is no ressource to backup.
	 *   @attention KNS-MAC-ressource status is an internal variable which acyually needs to
	 *   remain over LPM, but when all is cleared, it is ok to enter a LPM (e.g. shutdown)
	 *   which clears this variables (as it is already cleared!!)
	 *
	 * * @note SUBGHZ peripheral is able to wakeup the STM32WL55xx uC from STNDBY, but the timer
	 * TIM16 is only able to wakeup from LPSLEEP
	 * */
	if (rsrc.raw == 0x0)
		return LOW_POWER_MODE_SHUTDOWN;
	if (rsrc.txTimeout == 1) // as TX timeout timer is htim16 (cf aks_l1_cfg.h)
		return LOW_POWER_MODE_SLEEP;
	return LOW_POWER_MODE_STANDBY;
}

bool KSTK_lpmNotifEnter(__attribute__((unused)) enum MgrLpm_LPM_t enteringLpm)
{
	return true;
}

bool KSTK_lpmNotifExit(__attribute__((unused)) enum MgrLpm_LPM_t exitingLpm)
{
	return true;
}

/**
 * @}
 */
