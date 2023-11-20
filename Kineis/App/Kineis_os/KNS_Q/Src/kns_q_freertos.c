// SPDX-License-Identifier: no SPDX license
/**
 * @file    kns_q_freertos.c
 * @brief   Configuration of queues used in kineis_swr
 * @author  William Begoc
 * @date    creation 21/07/2022
 * @version 1.0
 * @note
 */

/**
 * @addtogroup KNS_Q
 * @brief Kineis SW queue utilities based on FreeRTOS
 * @{
 */

#ifndef KNS_Q_FREERTOS_C
#define KNS_Q_FREERTOS_C

/* Includes ------------------------------------------------------------------------------------ */

#include "kineis_sw_conf.h"
#include "kns_q_conf.h"
#include "kns_q.h"
#include "mgr_log.h"
#include "FreeRTOS.h"
#include "queue.h"

#pragma GCC visibility push(default)

/* Defines ------------------------------------------------------------------------------------- */

/* Structures ---------------------------------------------------------------------------------- */

static void *qPoolHandle[KNS_Q_MAX];

/* Function prototypes ------------------------------------------------------------------------- */

#ifndef UNIT_TEST
__attribute((__weak__))
#endif
enum KNS_status_t KNS_Q_create(enum KNS_Q_handle_t qHandle, uint8_t qLength, uint8_t qEltByteSize)
{

	QueueHandle_t xQueue;

	xQueue = xQueueCreate(qLength, qEltByteSize);
	if (xQueue != NULL) {
		qPoolHandle[qHandle] = (void *)xQueue;
		MGR_LOG_VERBOSE("[KNS_Q] create q %d, len %d, elt size %d, RTOS hdl 0x%p\r\n",
			qHandle, qLength, qEltByteSize, (void *)xQueue);
		return KNS_STATUS_OK;
	} else
		return KNS_STATUS_ERROR;
}

#ifndef UNIT_TEST
__attribute((__weak__))
#endif
enum KNS_status_t KNS_Q_push(enum KNS_Q_handle_t qHandle, void *qItem)
{
	if (xQueueSendToBack(qPoolHandle[qHandle], (void *)qItem, 0) == pdPASS) {
		MGR_LOG_VERBOSE("[KNS_Q] push q %d, elt 0x%p\r\n", qHandle, (void *)qItem);
		return KNS_STATUS_OK;
	}
	MGR_LOG_DEBUG("[KNS_Q][ERROR] push q %d FAILED, elt 0x%p\r\n", qHandle, (void *)qItem);
	return KNS_STATUS_ERROR;
}

#ifndef UNIT_TEST
__attribute((__weak__))
#endif
enum KNS_status_t KNS_Q_pop(enum KNS_Q_handle_t qHandle, void *qItem)
{
	/** @todo handle empty queue should return KNS_STATUS_QEMPTY,
	 * KNS_STATUS_OK means some valid element received,
	 * KNS_STATUS_ERROR for any other reason
	 */
	if (xQueueReceive(qPoolHandle[qHandle], (void *)qItem, 0) == pdPASS) {
		MGR_LOG_VERBOSE("[KNS_Q] pop  q %d\r\n", qHandle);
		return KNS_STATUS_OK;
	}
	MGR_LOG_DEBUG("[KNS_Q][ERROR] pop q %d FAILED\r\n", qHandle);
	return KNS_STATUS_ERROR;
}

#endif /* KNS_Q_FREERTOS_C */

/**
 * @}
 */
