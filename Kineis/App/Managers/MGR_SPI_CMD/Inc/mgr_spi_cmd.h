/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_spi_cmd.h
 * @brief SPI driver command parser.
 *
 * This module is responsible for parsing an incoming SPI data stream, extracting commands,
 * and calling the corresponding handlers.
 *
 * @page mgr_spi_cmd_page SPI commands manager
 *
 * The SPI command manager is in charge of parsing an incoming data stream and extracting commands.
 * It then calls the corresponding handler to process each command.
 *
 * @section mgr_spi_cmd_subpages Sub-pages
 * Additional sub-pages can be used to document specific subsets of SPI commands.
 *
 * @author Arribada
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_H
#define __MGR_SPI_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "kns_types.h"
#include "kineis_sw_conf.h" // get ERROR_RETURN_T types
#include "mcu_spi_driver.h"
#include "mgr_spi_cmd_list.h"

/* Defines -------------------------------------------------------------------*/
#define CMD_IT_TIMEOUT 1000      /**< Timeout in milliseconds for SPI command processing interrupts */

/* Extern Variables ----------------------------------------------------------*/
extern CmdValue cmdInProgress;   /**< Current SPI command in progress */

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Start the SPI command manager.
 *
 * This is the main API used to start the SPI command manager. Internally, it calls the MCU wrapper
 * to start the SPI command console (e.g., enabling SPI RX interrupts) and prepares the system to process
 * incoming SPI commands.
 *
 * @param[in] context Handler or pointer to the required hardware settings.
 *                    This context is passed directly to the MCU wrappers, allowing the application
 *                    to customize its type as needed.
 *
 * @retval true if the SPI command manager is successfully started, false otherwise.
 */
bool MGR_SPI_CMD_start(void *context);

/**
 * @brief Process events from the Kineis stack as responses to SPI commands.
 *
 * This function retrieves and processes events coming from the Kineis stack as answers to SPI commands.
 * It typically handles events such as TX-done, TX-timeout, RX-timeout, and in some cases RX events like
 * frame reception or downlink messages.
 *
 * @retval KNS_STATUS_OK if the TX is completed successfully,
 *         KNS_STATUS_TIMEOUT if a timeout is reached,
 *         or KNS_STATUS_ERROR if an error occurred.
 */
enum KNS_status_t MGR_SPI_CMD_macEvtProcess(void);

/**
 * @brief Handle state transitions for the SPI command manager.
 *
 * This function is responsible for processing and updating the internal state of the SPI command manager.
 * It should be called regularly to ensure proper state management and timely processing of SPI commands.
 */
void MGR_SPI_CMD_state_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MGR_SPI_CMD_H */

/**
 * @}
 */
