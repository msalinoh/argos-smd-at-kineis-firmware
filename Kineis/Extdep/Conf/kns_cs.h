/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_cs.h
 * @brief   critical section handling for kineis stack.
 * @author  Kineis
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Kineis. All rights reserved.</center></h2>
 *
 * This software component is licensed by Kineis under Ultimate Liberty license, the "License";
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 * * kineis.com
 *
 */

/**
 * @addtogroup KINEIS_SW_CONF
 * @brief Kineis SW configuration regarding critical sections
 * @{
 */

#ifndef KNS_CS_H
#define KNS_CS_H

/* Exported functions ------------------------------------------------------- */

/** @brief Enter critical section
  */
void KNS_CS_enter(void);

/** @brief Exit critical section
  */
void KNS_CS_exit(void);

#endif // end KNS_CS_H

/**
 * @}
 */
