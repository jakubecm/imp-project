/**
 * @file gap.h
 * @brief Header file for generic access profile (GAP) functionalities.
 * @author Milan Jakubec (xjakub41)
 *
 * This file contains the declarations for functions used to initialize the GAP.
 * Parts of this code have been adapted from the NimBLE library examples, copyright down below:
 *
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#ifndef GAP_SVC_H
#define GAP_SVC_H

#define TAG "NimBLE_GATT_Server"
#define DEVICE_NAME "ESP32_BLE"

/* Includes */
/* NimBLE GAP APIs */
#include "host/ble_gap.h"
#include "services/gap/ble_svc_gap.h"

/* Defines */
#define BLE_GAP_APPEARANCE_GENERIC_TAG 0x0200
#define BLE_GAP_URI_PREFIX_HTTPS 0x17
#define BLE_GAP_LE_ROLE_PERIPHERAL 0x00

/**
 * @brief Initialize the advertising module.
 */
void adv_init(void);

/**
 * @brief Initialize the GAP (Generic Access Profile) module.
 *
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int gap_init(void);

#endif // GAP_SVC_H