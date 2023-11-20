/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_app.h
 * @brief   provide main application softwares using kineis sw satck
 * @author  Kinéis
 */

/**
 * @addtogroup KNS_APP
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef KNS_APP_H
#define KNS_APP_H

/* Functions prototypes ------------------------------------------------------*/

/** @brief This fct is a very basic standalone application
 *
 * So far, it is periodicaly transmitting one message with harcoded payload, each 60s.
 *
 * This application is internally posting a send-data event to MAC layer of Kineis software stack.
 * Then is is waiting for a sucessful TX-done event
 */
void KNS_APP_stdalone(void);

/** @brief This fct is same base as KNS_APP_stdalone, with some extra unexpected operations.
 *
 * So far, it is:
 * * Requesting transmission of one message (with harcoded payload)
 * * Trying to send same message once again while previous transmission is not finished yet.
 *   As there is no FIFO yetin MAC, this is not supported. Thus MAC reports an ERROR back to APP
 *
 * This application is internally posting a send-data event to MAC layer of Kineis software stack.
 * Then is is waiting for a sucessful TX-done event
 */
void KNS_APP_stdalone_stressTest(void);

/** @brief This fct is the application expected to run in front of the GUI
 *
 * This application is waiting for AT commands coming from GUI to perform some actions such as:
 * * Display the firmware version ("AT+FW=?\r\n")
 * * Display the ID ("AT+ID=?\r\n")
 * * Transmit some user data ("AT+TX=<USER DATA>\r\n")
 *
 * Refer to @ref mgr_at_cmd_page page to get supported AT commands.
 */
void KNS_APP_gui(void);

#endif // end KNS_APP_H

/**
 * @}
 */
