/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    mgr_lpm.h
 * @brief   low power mode manager is in charge to collect all low power needs from 'clients' and
 *          setup the corresponding low power mode.
 *
 * @date    creation 2023/02/20
 * @author  Kineis
 */

/**
 * @page mgr_lpm_page Low Power Mode Manager (generic code)
 *
 * This page is presenting the low power mode manager. This software entity is in charge to enter
 * and exit low power modes thanks to client requests.
 *
 * @section mgr_lpm_design Design
 *
 * Here is the rough design we want to apply on low power mode manager. It has to be detailled
 * deeper.
 *
 * The LPM manager should work as a client/server model.
 * When enterLPM() API is called, the LPM manager:
 * * Asks the deepest LPM allowed by each client
 * * The shallowest mode is chosen
 * * Each client is notified by the manager
 * * Finally, the manager sets the chosen LPM
 *
 *                         enterLPM()
 *                                \_________
 *                                | MGR_LPM |
 *                                |_________|
 *                               //   ||    \\
 *                              //    ||     \\
 *                       Client1   Client2    ClientX
 *
 * Refer to mgr_lpm.h for extra details on the Apis
 */

/**
 * @addtogroup MGR_LPM
 * @brief low power mode manager module (refer to @ref mgr_lpm_page page for general description).
 * @{
 */

#ifndef MGR_LPM_H
#define MGR_LPM_H


/* Includes ------------------------------------------------------------------------------------ */
#include <stdbool.h>
#include <stdint.h>
#include "kns_types.h"


/* Defines ------------------------------------------------------------------------------------- */


/* Enums --------------------------------------------------------------------------------------- */

/** @brief Enumeration describing Low Power Mode (LPM)
 *
 * @attention This enum must be sorted from lowest LPM to deepest as numerical value is used
 * for comparision in bMGR_LPM_enter function as a bitmap
 */
enum MgrLpm_LPM_t {
	LOW_POWER_MODE_NONE     = 0x00,
	LOW_POWER_MODE_SLEEP    = 0x01,
	LOW_POWER_MODE_STOP     = 0x02,
	LOW_POWER_MODE_STANDBY  = 0x04,
	LOW_POWER_MODE_SHUTDOWN = 0x08
};


/* Structures ---------------------------------------------------------------------------------- */

/** @brief Structure containing parameters about the environment
 */
struct MgrLpm_EnvConfig_t {
	uint8_t allowedLPMbitmap;        ///< Bitmap for allowed LPM (see @ref MgrLpm_LPM_t definition)
	void (*fp_sleep_enter)   (void); ///< Called before entering sleep mode
	void (*fp_sleep_exit)    (void); ///< Called after  exiting  sleep mode
	void (*fp_stop_enter)    (void); ///< Called before entering stop mode
	void (*fp_stop_exit)     (void); ///< Called after  exiting  stop mode
	void (*fp_standby_enter) (void); ///< Called before entering standby
	void (*fp_shutdown_enter)(void); ///< Called before entering shutdown
};

/**
 * @brief Structure containing MGR_LPM client callbacks. It lists all callbacks needed per client.
 *
 * list of callback is:
 * - enum MgrLpm_LPM_t (*fpMGR_LPM_LpmReqCb)(void);
 *    @return MgrLpm_LPM_t The client returns the deepest LPM mode it can accept.
 
 * - bool (*fpMGR_LPM_LpmNotifEnterCb)(enum MgrLpm_LPM_t);
 *    @param[in] MgrLpm_LPM_t MGR_LPM notifies the client about LPM mode it enters
 *    @return    1:OK   0:Unable to enter to this LPM
 
 * - bool (*fpMGR_LPM_LpmNotifExitCb)(enum MgrLpm_LPM_t);
 *    @param[in] MgrLpm_LPM_t MGR_LPM notifies the client about LPM mode it exits
 *    @return    1:OK   0:Unable to exit from this LPM
 */
struct MgrLpmClientCb_t {
     enum MgrLpm_LPM_t (*fpMGR_LPM_LpmReqCb)(void);
     bool              (*fpMGR_LPM_LpmNotifEnterCb)(enum MgrLpm_LPM_t enteringLpm);
     bool              (*fpMGR_LPM_LpmNotifExitCb)(enum MgrLpm_LPM_t exitingLpm);
};

struct MgrLpm_ctxt_t {
	uint32_t reserved             : 28;
	uint32_t low_power_mode       :  4; /* current LPM mode */
};


/* Functions ----------------------------------------------------------------------------------- */
/**
 * @brief This initialize the callback client tab with NULL pointer.
 *
 * @param[in] env_config: structure pointing to environment parameters needed by low power
 *            manager
 *
 * @return KNS_STATUS_OK
 */
enum KNS_status_t MGR_LPM_init(struct MgrLpm_EnvConfig_t env_config);

/**
 * @brief This function adds a client in the client table
 *
 * @note To update client callback pointers, unregister the corresponding client and register it again with
 *          the updated pointers.
 *
 * @note When a client is unregistered and an other one is registered, the new one is set at the old
 * one location :
 *
 *       * Unregister n°2 :      * N°2 unregistered :      * Register n°4:
 *            ___________             ___________               ___________
 *           |_____0_____|           |_____0_____|             |_____0_____|
 *           |_____1_____|           |_____1_____|             |_____1_____|
 *           |_____2_____|<          |____NULL___|<            |_____4_____|<
 *           |_____3_____|           |_____3_____|             |_____3_____|
 *
 * @param[in] mgrLpmClient: client structure to be added (with the right callback pointer definition)
 *
 * @return KNS_STATUS_OK or KNS_STATUS_ERROR
 */
enum KNS_status_t MGR_LPM_registerClient(struct MgrLpmClientCb_t mgrLpmClient);

/**
 * @brief This function remove a client from the client tab.
 *
 * @param[in] mgrLpmClient: client structure to be removed (with the right callback pointer definition)
 *
 * @return KNS_STATUS_OK or KNS_STATUS_ERROR
 */
enum KNS_status_t MGR_LPM_unregisterClient(struct MgrLpmClientCb_t mgrLpmClient);

/**
 * @brief This is the main Entry point to the low power mode manager
 *
 * Depending on the state of clients, the low power mode manager will choose the best low
 * power mode.
 *
 * @note When entering in LPM, clients are notified from client [0] to client [MGR_LPM_CLIENT_NBR_MAX-1]. When
 * exiting LPM, clients are notified in the reverse order.
 *
 *       Notify entering :                 Notify exiting :
 *           ___________                      ___________
 *          |_____0_____|  |                 |_____0_____| /|\
 *          |_____1_____|  |                 |_____1_____|  |
 *          |____...____|  |                 |____...____|  |
 *          |____MAX____| \|/                |____MAX____|  |
 *
 * @param[in] env_config: structure pointing to environment parameters needed by low power
 *            manager
 * @param[out] mgr_lpm_ctxt output context telling which low power mode was set
 *
 * @return KNS_STATUS_OK or KNS_STATUS_ERROR
 */
enum KNS_status_t MGR_LPM_enter(struct MgrLpm_EnvConfig_t env_config,
	struct MgrLpm_ctxt_t *mgr_lpm_ctxt);


#endif /* MGR_LPM_H */

/**
 * @}
 */
