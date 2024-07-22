/* SPDX-License-Identifier: no SPDX license */
/**
 * @file    kns_app_conf.h
 * @brief   Kineis Application configurations depending on platform used
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
 * @addtogroup KNS_APP
 * @brief Kineis Application configuration. This module can be adapted by external user depending
 * on project needs.
 * @{
 */

#ifndef KNS_APP_CONF_H
#define KNS_APP_CONF_H

/* Defines ------------------------------------------------------------------------------------- */

/**
 * @brief define STM32 HAL APIs as per platform, here STM32WL microcontroller
 *
 * @note This is not used by Kineis stack. It is only used by the Kineis APP (open code). You can
 * change adapt it the way you want.
 */
#define STM32_HAL_H		"stm32wlxx_hal.h"
#define STM32_HAL_RTC_H		"rtc.h"

#define INCLUDE_NOP_H		"stm32wlxx_hal.h"

#endif // end KNS_APP_CONF_H

/**
 * @}
 */
