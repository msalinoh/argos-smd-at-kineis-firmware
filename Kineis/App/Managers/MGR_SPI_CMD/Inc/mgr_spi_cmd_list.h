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
#include "mcu_aes.h"
#include "mcu_nvm.h"

#define CMD_VARIABLE_LEN 0xFF
#define CMD_WRITEID_WAIT_LEN 5 // 4 for uint32 size + 1 cmd
#define CMD_WRITEADDRESS_WAIT_LEN (DEVICE_ADDR_LENGTH+1) // 4 addr size + 1 cmd
#define CMD_WRITESECKEY_WAIT_LEN (DSK_BYTE_LENGTH+1) // 4 addr size + 1 cmd
#define CMD_WRITERCONF_WAIT_LEN 33 // 32 conf size + 1 cmd
#define CMD_WRITELPM_WAIT_LEN 2    // 1 LPM mode + 1 cmd
#define CMD_WRITEKMAC_WAIT_LEN 2    // 1 write only ID for the moment + 1 cmd
#define CMD_WRITETX_WAIT_LEN 3    // 1 write only ID for the moment + 2 datasize (u16)
/* Defines -------------------------------------------------------------------*/

typedef enum {
	CMD_NONE           = 0x00,  // Cmd unknown
    CMD_READ           = 0x01,  // Read command (delete me ?)
    CMD_PING           = 0x02,  // Ping command
    CMD_MAC_STATUS     = 0x03,  // Read MAC status
    CMD_SPI_STATUS     = 0x04,  // Read SPI status
    CMD_READ_VERSION   = 0x05,  // Read version
    CMD_READ_FIRMWARE  = 0x06,  // Read firmware
    CMD_READ_ADDR      = 0x07,  // Read address
    CMD_READ_ID        = 0x08,  // Read ID
    CMD_READ_SN        = 0x09,  // Read serial number
    CMD_READ_RCONF     = 0x0A,  // Read configuration
    CMD_WRITE_RCONF_REQ= 0x0B,  // Write configuration Request
    CMD_WRITE_RCONF    = 0x0C,  // Write configuration Value
    CMD_SAVE_RCONF     = 0x0D,  // Save configuration
    CMD_READ_KMAC      = 0x0E,  // Read KMAC profile
    CMD_WRITE_KMAC_REQ = 0x0F,  // Write KMAC profile Request
    CMD_WRITE_KMAC     = 0x10,  // Write KMAC profile value
    CMD_READ_LPM       = 0x11,  // Read low power mode
    CMD_WRITE_LPM_REQ  = 0x12,  // Set low power mode Request
    CMD_WRITE_LPM      = 0x13,  // Set low power mode Value
    CMD_WRITE_TX_REQ   = 0x14,  // TX uplink request
    CMD_WRITE_TX_SIZE  = 0x15,  // Waiting Size request to read
    CMD_WRITE_TX       = 0x16,  // Write TX uplink value
	CMD_READ_CW        = 0x17,  // Read Continuous wave (not implemented in SPI)
	CMD_WRITE_CW_REQ   = 0x18,  // Write CW request (not implemented in SPI driver)
	CMD_WRITE_CW       = 0x19,  // Write CW value (not implmented in SPI driver)
	CMD_READ_PREPASSEN        = 0x1A,  // Read Prepass enable
	CMD_WRITE_PREPASSEN_REQ   = 0x1B,  // Write prepass enable request
	CMD_WRITE_PREPASSEN       = 0x1C,  // Write prepass enable value
	CMD_READ_UDATE            = 0x1D,  // Read date
	CMD_WRITE_UDATE_REQ       = 0x1E,  // Write date request
	CMD_WRITE_UDATE           = 0x1F,  // Write date value
    CMD_WRITE_ID_REQ          = 0x20,  // Write ID Request
    CMD_WRITE_ID              = 0x21,  // Write ID value
    CMD_WRITE_ADDR_REQ        = 0x22,  // Write addres REquest
    CMD_WRITE_ADDR            = 0x23,  // Write Address Value
	CMD_READ_SECKEY           = 0x24,  // Read Secret key
	CMD_WRITE_SECKEY_REQ      = 0x25,  // Write Secret key request
	CMD_WRITE_SECKEY		  = 0x26,  // Write Secret key value
	CMD_READ_SPIMAC_STATE	  = 0x27,  // Write Secret key value
	SPICMD_MAX_COUNT          = 0x28
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
