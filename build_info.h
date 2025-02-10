/**
 * @file    build_info.h
 * @brief   contains references to SW version information. Corresponding builf_info.c will be
 *          generated during build
 *
 * @date    creation 2020/08/28
 * @author  William BEGOC
 * @version 1.0
 * @note
 *
 */

/**
 * @addtogroup BUILD_INFO
 * @brief   SW versino information
 * @{
 */

#ifndef BUILD_INFO_H
#define BUILD_INFO_H

/*******************************************************************
 * INCLUDES
 ******************************************************************/
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * EXTERNS
 ******************************************************************************/
/**
 * This variable contains last GIT commit ID
 */
#define FW_VERSION_LENGTH 128  // Define the fixed length

extern const char uc_fw_vers_commit_id[FW_VERSION_LENGTH];

#endif /* BUILD_INFO_H */

/**
 * @}
 */
