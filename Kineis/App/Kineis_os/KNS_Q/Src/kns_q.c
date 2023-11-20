// SPDX-License-Identifier: no SPDX license
/**
 * @file    kns_q.c
 * @brief   Configuration of queues used in kineis_swr
 * @author  William
 * @date    creation 21/07/2022
 * @version 1.0
 * @note
 */

/**
 * @addtogroup KNS_Q
 * @brief Kineis SW queues generic entry point. (refer to @ref kns_q_page for general description).
 * @{
 */

#ifdef USE_BAREMETAL
#if (defined(USE_FREERTOS) && (USE_FREERTOS == 1U)) || \
(defined(USE_CMSIS_OS2) && (USE_CMSIS_OS2 == 1U))
#error choose one single Os in KNS libs
#endif
#include "kns_q_baremetal.c"
#endif

#ifdef USE_FREERTOS
#if (defined(USE_BAREMETAL) && (USE_BAREMETAL == 1U)) || \
(defined(USE_CMSIS_OS2) && (USE_CMSIS_OS2 == 1U))
#error choose one single Os in KNS libs
#endif
#include "kns_q_freertos.c"
#endif

#ifdef USE_CMSIS_OS2
#if (defined(USE_BAREMETAL) && (USE_BAREMETAL == 1U)) || \
(defined(USE_FREERTOS) && (USE_FREERTOS == 1U))
#error choose one single Os in KNS libs
#endif
#include "kns_q_cmsis_os2.c"
#endif

/**
 * @}
 */
