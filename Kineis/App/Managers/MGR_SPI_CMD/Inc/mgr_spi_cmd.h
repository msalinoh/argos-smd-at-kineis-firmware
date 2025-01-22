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
#include "mgr_spi_cmd_list.h"

#define CMD_IT_TIMEOUT 1000
extern CmdValue cmdInProgress;
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
 * @brief Fct used to retreive and process event coming from kineis stack as answers to AT commands
 *
 * Typically, this is about processing TX events such as TX-done, TX-timeout, RX-timeout in case of
 * TRX. In case of KIM2 HW, it is also about RX events such as RX-frame-received, DL-msg-received.
 *
 * @retval KNS_STATUS_OK if TX DONE or KNS_STATUS_TIMEOUT if timeout reached, else KNS_STATUS_ERROR
 */
enum KNS_status_t MGR_SPI_CMD_macEvtProcess(void);

void MGR_SPI_CMD_state_handler(void);
#endif /* __MGR_SPI_CMD_H */

/**
 * @}
 */
