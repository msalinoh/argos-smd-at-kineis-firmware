/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_os_conf.h
 * @brief   Configuration of task used with/in kineis_sw
 * @author  Kineis
 * @date    creation 23/06/2023
 */

/**
 * @page kns_os_conf_page Kineis OS configuration
 *
 * This page is presenting the configuration parameters of Kineis OS.
 *
 * So far, Kineis barematal OS needs to get a list of all tasks to be registerd. This is a simple
 * enumerate containing all handler. there is no notoin of priorities on tasks in this OS.
 * Priorities are actually handled when popping events in queues, refer to \ref kns_q_page for extra
 * details on this.
 */

/**
 * @addtogroup KNS_OS
 * @{
 */

#ifndef KNS_OS_CONF_H
#define KNS_OS_CONF_H

/* Includes ------------------------------------------------------------------------------------ */

#pragma GCC visibility push(default)

/* Enums --------------------------------------------------------------------------------------- */

/**
 * @enum KNS_OS_taskHdlr_t
 * @brief Queue handlers
 * @attention Sort handlers by priority from lower to higher. SERVICE queue is higher priority than
 *       PROFILE's one. This will be important in case of baremetal "OS" (refer to KNS_SCHED).
 */
enum KNS_OS_taskHdlr_t {
	KNS_OS_TASK_APP, /**< application task */
	KNS_OS_TASK_MAC, /**< Kineis stack task (MAC layer) */
	KNS_OS_TASK_MAX	 /**< number of queues */
};

#endif /* KNS_OS_CONF_H */

/**
 * @}
 */
