/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_common.h
 * @author Kineis
 * @brief header file for common part of the AT cmd manager
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_COMMON_H
#define __MGR_SPI_CMD_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "strutil_lib.h"
#include "kineis_sw_conf.h" // get ERROR_RETURN_T types
#include "mcu_spi_driver.h"
#include "kns_mac.h"
/* Defines ------------------------------------------------------------------*/
typedef enum {
    SPICMD_INIT,                // Register SPI command manager
    SPICMD_IDLE,                // Idle waiting TX request
    SPICMD_PROCESS_CMD,         // Process incoming command
    SPICMD_WAITING_RX,          // Waiting for RX data with specified length
    SPICMD_WAITING_TX,          // Waiting for TX data to be sent
    SPICMD_WAITING_MAC_EVT,     // Waiting for MAC event (e.g., TX done, RX done)
    SPICMD_ERROR                // Error state
} SpiState;

extern SpiState spiState;

typedef enum {
    MAC_OK            = 0x01,
    MAC_TX_DONE       = 0x02,
    MAC_TX_SIZE_ERROR = 0x03,
    MAC_TXACK_DONE    = 0x04,
    MAC_TX_TIMEOUT    = 0x05,
    MAC_TXACK_TIMEOUT = 0x06,
    MAC_RX_ERROR      = 0x07,
    MAC_RX_TIMEOUT    = 0x08,
    MAC_ERROR         = 0x09
} MACStatus;
extern MACStatus macStatus;
//extern enum KNS_MAC_srvcEvtId_t macStatus;
/* functions prototypes -----------------------------------------------------*/

/** @brief Log in debug interface a failed message
 *
 * @param[in] eErrorType : the error type
 *
 * @return : always return false
 */
bool bMGR_SPI_CMD_logFailedMsg(enum ERROR_RETURN_T eErrorType, SPI_Buffer *tx);

uint8_t bMGR_SPI_DRIVER_wait_next();
uint8_t bMGR_SPI_DRIVER_resp();
#endif /* __MGR_SPI_CMD_COMMON_H */

/**
 * @}
 */
