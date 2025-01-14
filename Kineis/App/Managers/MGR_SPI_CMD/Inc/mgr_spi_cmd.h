/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_spi_cmd.h
 * @author  Arribada
 * @brief SPI driver cmd parser
 */

/**
 * @page mgr_at_cmd_page AT commands manager
 *
 * The AT cmd manager is in charge of parsing an incoming data stream and extract commands. Then
 * call the corresponding handler.
 *
 * @section mgr_at_cmd_subpages Sub-pages
 *
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


typedef enum {
	CMD_UNKNOWN        = 0x00,  // Cmd unknoiwn
    CMD_READ           = 0x01,  // Ping command
    CMD_PING           = 0x02,  // Ping command
    CMD_MAC_STATUS     = 0x03,  // Read MAC status
    CMD_SPI_STATUS     = 0x04,  // Read SPI status
    CMD_READ_VERSION   = 0x05,  // Read version
    CMD_READ_FIRMWARE  = 0x06,  // Read firmware
    CMD_READ_ADDR      = 0x07,  // Read address
    CMD_READ_ID        = 0x08,  // Read ID
    CMD_READ_SN        = 0x09,  // Read serial number
    CMD_READ_RCONF     = 0x0A,  // Read configuration
    CMD_WRITE_RCONF_REQ= 0x0B,  // Write configuration
    CMD_WRITE_RCONF    = 0x0C,  // Write configuration
    CMD_SAVE_RCONF     = 0x0D,  // Save configuration
    CMD_READ_KMAC      = 0x0E,  // Reload configuration
    CMD_WRITE_KMAC_REQ = 0x0F,  // Reload configuration
    CMD_WRITE_KMAC     = 0x10,  // Reload configuration
    CMD_READ_LPM       = 0x11,  // Read low power mode
    CMD_WRITE_LPM_REQ  = 0x12,  // Set low power mode
    CMD_WRITE_LPM      = 0x13,  // Set low power mode
    CMD_WRITE_TX_REQ   = 0x14,  // Read low power mode
    CMD_WRITE_TX_SIZE  = 0x15,  // Read low power mode
    CMD_WRITE_TX       = 0x16,  // Set low power mode
	CMD_READ_CW        = 0x17,  // Set low power mode
	CMD_WRITE_CW_REQ   = 0x18,  // Read low power mode
	CMD_WRITE_CW       = 0x19,  // Set low power mode
	CMD_READ_PREPASSEN        = 0x1A,  // Set low power mode
	CMD_WRITE_PREPASSEN_REQ   = 0x1B,  // Read low power mode
	CMD_WRITE_PREPASSEN       = 0x1C,  // Set low power mode
	CMD_READ_UDATE        = 0x1D,  // Set low power mode
	CMD_WRITE_UDATE_REQ   = 0x1E,  // Read low power mode
	CMD_WRITE_UDATE       = 0x1F,  // Set low power mode

} CmdValue;

typedef enum {
    MAC_OK            = 0x01,
    MAC_TX_DONE       = 0x02,
    MAC_TX_SIZE_ERROR = 0x03,
    MAC_TXACK_DONE    = 0x04,
    MAC_TX_TIMEOUT    = 0x05,
    MAC_TXACK_TIMEOUT = 0x06,
    MAC_RX_ERROR      = 0x07,
    MAC_RX_TIMEOUT    = 0x08,
    MAC_ERROR         = 0x10
} MACStatusFlags;

extern const uint8_t spicmd_version;
extern uint16_t txBytesWaiting;


/*Functions ------------------------------------------------------------------*/


/**
 * @brief main API used to start AT command Manager
 *
 * This function will internally call MCU wrapper to start AT CMD console (such as UART RX isr
 * enabling)
 *
 * @param[in] context some handler on required HW setting.
 *            @note this handler will be directly sent as is, to MCU wrappers. Thus application can
 *            change its type as per needs.
 *
 * @retval true if AT command manager is correctly started, false otherwise
 */
bool MGR_SPI_CMD_start(void *context);

//bool MGR_AT_SPI_CMD_start(void *context);
/**
 * @brief API used to check there is some AT command in internal fifo
 *
 * @retval true if there is some AT command in fifo, false otherwise
 */
//bool MGR_AT_CMD_isPendingAt(void);

/**
 * @brief API used to get next AT command stored in internal fifo
 *
 * @retval Pointer to the AT cmd to be decoded
 */
//uint8_t *MGR_AT_CMD_popNextAt(void);

/**
 * @brief Decode and exectue AT cmd if valid
 *
 * @param[in] pu8_atcmd pointer to AT command
 *
 * @retval true if command was decoded and correctly executed, false otherwise
 */
//bool MGR_AT_CMD_decodeAt(uint8_t *pu8_atcmd);

int8_t bMGR_SPI_CMD_logFailedMsg(enum ERROR_RETURN_T eErrorType, SPI_Buffer *tx);
/**
 * @brief Fct used to retreive and process event coming from kineis stack as answers to AT commands
 *
 * Typically, this is about processing TX events such as TX-done, TX-timeout, RX-timeout in case of
 * TRX. In case of KIM2 HW, it is also about RX events such as RX-frame-received, DL-msg-received.
 *
 * @retval KNS_STATUS_OK if TX DONE or KNS_STATUS_TIMEOUT if timeout reached, else KNS_STATUS_ERROR
 */
enum KNS_status_t MGR_SPI_CMD_macEvtProcess(void);

#endif /* __MGR_SPI_CMD_H */

/**
 * @}
 */
