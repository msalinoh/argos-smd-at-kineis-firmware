/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_cmd_list.h
 * @author  Kineis
 * @brief AT commands list header file
 */

/**
 * @addtogroup MGR_AT_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_CONF_H
#define __MGR_SPI_CMD_CONF_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "mcu_spi_driver.h"

#define CMD_VARIABLE_LEN 0xFF
#define CMD_WRITERCONF_WAIT_LEN 33 // 32 conf size + 1 cmd
#define CMD_WRITELPM_WAIT_LEN 2    // 1 LPM mode + 1 cmd
#define CMD_WRITEKMAC_WAIT_LEN 2    // 1 write only ID for the moment + 1 cmd
#define CMD_WRITETX_WAIT_LEN 3    // 1 write only ID for the moment + 2 datasize (u16)
/* Defines -------------------------------------------------------------------*/

typedef enum {
	CMD_NONE           = 0x00,  // Cmd unknoiwn
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
	SPICMD_MAX_COUNT      = 0x20
} CmdValue;

/* Types ---------------------------------------------------------------------*/

/** Function entry point to process action/status commands. */
typedef bool (*pvSPICMD_cmd_proc_fun_type)(SPI_Buffer *rx, SPI_Buffer *tx);

/** Elementary structure for each AT command to hold necessary information
 * to handle action/status/information sub commands type
 */
struct spicmd_desc_t {
	/**<Name of the command */
	CmdValue cmd;

	/* Waiting specific next cmd*/
	CmdValue next_cmd;


	/**< Command function entry point to process action/status cmd type
	 * AT+XXXX=... or AT+XXXX=?
	 */
	pvSPICMD_cmd_proc_fun_type f_ht_cmd_fun_proc;

};

/* Extern ---------------------------------------------------------------------------------------*/
extern const uint8_t spicmd_version;
extern const struct spicmd_desc_t cas_spicmd_list_array[];
extern uint16_t userTxPayloadSize;



#endif /* __MGR_SPI_CMD_CONF_H */
/**
 * @}
 */
