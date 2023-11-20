/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_q_conf.h
 * @brief   Configuration of queues used in kineis_sw
 * @author  William Begoc
 * @date    creation 21/07/2022
 * @version 1.0
 * @note
 */

/**
 * @page kns_q_conf_page Kineis OS queue configuration
 *
 * This page is presenting the configuration parameters of Kineis software queues. It should be
 * fixed as per software architecture. Parameters are:
 * * size of each queue (maximum number of items in queue)
 * * size of each element of the queue.
 *
 * So far, description is done for UL queue between DRIVER layer and SERVICE layer.
 *
 * Additional later supports will probably add descriptions of following queues:
 * * DL PROFILE to SERVICE queue
 * * UL DRIVER to PROFILE queue
 * * UL SERVICE to PROFILE queue
 *
 */

/**
 * @addtogroup KNS_Q
 * @brief Kineis SW queues. (refer to @ref kns_q_page for general description).
 * @{
 */

#ifndef KNS_Q_CONF_H
#define KNS_Q_CONF_H

/* Includes ------------------------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>

#pragma GCC visibility push(default)

/* Defines ------------------------------------------------------------------------------------- */

/**< @attention queue length should be one more than expected in case of baremetal KNS OS */
#ifdef USE_BAREMETAL
#define KNS_Q_DL_APP2MAC_LEN           4
#else // else of USE_BAREMETAL
#define KNS_Q_DL_APP2MAC_LEN           3
#endif // end of USE_BAREMETAL
#define KNS_Q_DL_APP2MAC_ITEM_BYTESIZE 30

/**< @attention queue length should be one more than expected in case of baremetal KNS OS */
#ifdef USE_BAREMETAL
#define KNS_Q_UL_SRVC2MAC_LEN           4
#else // else of USE_BAREMETAL
#define KNS_Q_UL_SRVC2MAC_LEN           3
#endif // end of USE_BAREMETAL
#define KNS_Q_UL_SRVC2MAC_ITEM_BYTESIZE 16

#endif /* KNS_Q_CONF_H */

/**
 * @}
 */
