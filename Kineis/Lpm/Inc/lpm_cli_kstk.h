/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    lpm_cli_kstk.h
 * @brief   Kineis stack's LPM client. It is implementing APIs needed to interface with the low
 *          power manager (MGR_LPM)
 * @author  Kineis
 */

/**
 * @addtogroup MGR_LPM
 * @{
 */

#ifndef LPM_CLI_KSTK_H
#define LPM_CLI_KSTK_H

/* Includes ------------------------------------------------------------------------------------ */
#include <stdbool.h>
#include "mgr_lpm.h"

/* Enums --------------------------------------------------------------------------------------- */

extern struct MgrLpmClientCb_t mgrLpmCliKstk;

/* Functions ----------------------------------------------------------------------------------- */

/**
 * @brief Request deepest LPM allowed by the Kineis stack client
 *
 * Thus function is computing the deepest LPM from:
 * * the active-ressources list reported by Kineis stack through KNS_MAC_getRsrcStatus
 * * the corresponding HW peripherals reserved through MCU wrappers (MCU_...)
 * * the available LPM on the hosting-UC all this code
 *
 * Actually on KIMx, so far, deepest LPM is:
 * * STANDBY almost all time except if modem is ON
 * * NONE (i.e. runtime) when modem is ON
 * * SHUTDOWN if kineis stack is not started yet
 *
 * @return MgrLpm_LPM_t return the low power mode as per MGR_LPM definition
 */
enum MgrLpm_LPM_t KSTK_lpmReq(void);

/**
 * @brief Notify the Kineis stack client which LPM is going to enter
 *
 * @param[in] enteringLpm entering LPM as per MGR_LPM definition
 *
 * @return true is status is OK, false otherwise
 */
bool KSTK_lpmNotifEnter(enum MgrLpm_LPM_t enteringLpm);

/**
 * @brief Notify the Kineis stack client from which LPM UC just exited
 *
 * @param[in] exitingLpm exiting LPM as per MGR_LPM definition
 *
 * @return true is status is OK, false otherwise
 */
bool KSTK_lpmNotifExit(enum MgrLpm_LPM_t exitingLpm);


/* Functions ----------------------------------------------------------------------------------- */


#endif /* LPM_CLI_KSTK_H */

/**
 * @}
 */
