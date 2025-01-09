/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_at_spi.h
 * @author  Arribada
 * @brief AT SPI commands manager main header file
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_AT_SPI_CMD_H
#define __MGR_AT_SPI_CMD_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "kns_types.h"

/*Functions ------------------------------------------------------------------*/


bool MGR_AT_SPI_CMD_start(void *context);

bool MGR_AT_SPI_CMD_isPendingAt(void);

uint8_t *MGR_AT_SPI_CMD_popNextAt(void);

bool MGR_AT_SPI_CMD_decodeAt(uint8_t *pu8_atcmd);

enum KNS_status_t MGR_AT_SPI_CMD_macEvtProcess(void);

#endif /* __MGR_AT_SPI_CMD_H */

/**
 * @}
 */
