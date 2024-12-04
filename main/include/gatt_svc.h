/**
 * @file gatt_svc.h
 * @brief Header file for generic attribute profile (GATT) functionalities.
 * @author Milan Jakubec (xjakub41)
 *
 * This file contains the declarations for functions used to initialize the GATT server.
 * Parts of this code have been adapted from the NimBLE library examples, copyright down below:
 *
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#ifndef GATT_SVR_H
#define GATT_SVR_H

/* Includes */
/* NimBLE GATT APIs */
#include "host/ble_gatt.h"
#include "services/gatt/ble_svc_gatt.h"

/* NimBLE GAP APIs */
#include "host/ble_gap.h"

/* Public function declarations */
/**
 * @brief Callback function for GATT server registration.
 *
 * This function is called when a GATT service or characteristic is registered.
 *
 * @param ctxt Pointer to the GATT register context.
 * @param arg User-defined argument.
 */
void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);

/**
 * @brief Initialize the GATT service.
 *
 * This function initializes the GATT service and registers the necessary
 * characteristics and services.
 *
 * @return 0 on success, non-zero error code on failure.
 */
int gatt_svc_init(void);

#endif // GATT_SVR_H