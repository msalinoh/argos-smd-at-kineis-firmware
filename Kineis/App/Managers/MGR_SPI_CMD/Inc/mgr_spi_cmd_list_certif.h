/* SPDX-License-Identifier: no SPDX license */
/**
 * @file mgr_spi_cmd_list_certif.h
 * @brief Subset of SPI commands concerning certification-specific operations.
 *
 * This header file declares functions for handling SPI commands that are specific
 * to certification tests, including reading and writing Continuous Wave (CW) parameters.
 * These functions are used to validate and test device performance during certification.
 *
 * @author Arribada
 */

/**
 * @addtogroup MGR_SPI_CMD
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MGR_SPI_CMD_CERTIF_H
#define __MGR_SPI_CMD_CERTIF_H

/* Includes ------------------------------------------------------------------*/
/* (Include necessary headers if required) */

/**
 * @brief Read Continuous Wave (CW) parameters for certification.
 *
 * This function processes the SPI command to read the current Continuous Wave (CW)
 * settings, which are used during certification tests to evaluate device performance.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the read command.
 * @param[out] tx Pointer to the SPI transmit buffer where the CW parameters will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_READCW_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Initiate a request to write new Continuous Wave (CW) parameters for certification.
 *
 * This function processes the SPI command that initiates the procedure for updating
 * the Continuous Wave (CW) settings used during certification tests.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the write request command.
 * @param[out] tx Pointer to the SPI transmit buffer where the request acknowledgment will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITECWREQ_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

/**
 * @brief Write new Continuous Wave (CW) parameters for certification.
 *
 * This function processes the SPI command to update the Continuous Wave (CW) settings
 * after a valid write request has been received. It is used to configure the device's
 * CW parameters during certification tests.
 *
 * @param[in] rx Pointer to the SPI receive buffer containing the new CW parameters.
 * @param[out] tx Pointer to the SPI transmit buffer where the result of the write operation will be sent.
 *
 * @return true if the command is correctly received and processed,
 *         false if an error occurs.
 */
bool bMGR_SPI_CMD_WRITECW_cmd(SPI_Buffer *rx, SPI_Buffer *tx);

#endif /* __MGR_SPI_CMD_CERTIF_H */

/**
 * @}
 */
