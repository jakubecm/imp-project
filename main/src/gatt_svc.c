/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "gatt_svc.h"
#include "gap.h"
#include "access_control.h"

/* Private function declarations */
static int access_code_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                      struct ble_gatt_access_ctxt *ctxt, void *arg);
static int unlock_duration_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                          struct ble_gatt_access_ctxt *ctxt, void *arg);

/* Private variables */
static const ble_uuid16_t access_control_svc_uuid = BLE_UUID16_INIT(0xFFF0);
static const ble_uuid16_t access_code_chr_uuid = BLE_UUID16_INIT(0xFFF1);
static const ble_uuid16_t unlock_duration_chr_uuid = BLE_UUID16_INIT(0xFFF2);

/* GATT services table */
static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    /* Access Control service */
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &access_control_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){
            {
                /* Access Code characteristic */
                .uuid = &access_code_chr_uuid.u,
                .access_cb = access_code_char_access_cb,
                .flags = BLE_GATT_CHR_F_WRITE,
            },
            {
                /* Unlock Duration characteristic */
                .uuid = &unlock_duration_chr_uuid.u,
                .access_cb = unlock_duration_char_access_cb,
                .flags = BLE_GATT_CHR_F_WRITE,
            },
            {
                0, /* No more characteristics in this service */
            },
        },
    },
    {
        0, /* Termination entry */
    },
};

/* Private functions */
/* Callback for access code characteristic */
static int access_code_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                      struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    switch (ctxt->op)
    {
    case BLE_GATT_ACCESS_OP_WRITE_CHR:
        if (is_device_unlocked())
        {
            strncpy(access_code, (char *)ctxt->om->om_data, min(ctxt->om->om_len, sizeof(access_code) - 1));
            update_nvs_access_code(access_code);
            return 0;
        }
        return BLE_ATT_ERR_WRITE_NOT_PERMITTED;
    default:
        return BLE_ATT_ERR_UNLIKELY;
    }
}

/* Callback for unlock duration characteristic */
static int unlock_duration_char_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                          struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    switch (ctxt->op)
    {
    case BLE_GATT_ACCESS_OP_WRITE_CHR:
        if (ctxt->om->om_len == sizeof(unlock_duration))
        {
            if (is_device_unlocked())
            {
                memcpy(&unlock_duration, ctxt->om->om_data, sizeof(unlock_duration));
                update_nvs_unlock_duration(unlock_duration);
                return 0;
            }
            return BLE_ATT_ERR_WRITE_NOT_PERMITTED;
        }
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    default:
        return BLE_ATT_ERR_UNLIKELY;
    }
}

/* Public functions */

/*
 *  Handle GATT attribute register events
 *      - Service register event
 *      - Characteristic register event
 *      - Descriptor register event
 */
void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    /* Local variables */
    char buf[BLE_UUID_STR_LEN];

    /* Handle GATT attributes register events */
    switch (ctxt->op)
    {

    /* Service register event */
    case BLE_GATT_REGISTER_OP_SVC:
        ESP_LOGD(TAG, "registered service %s with handle=%d",
                 ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                 ctxt->svc.handle);
        break;

    /* Characteristic register event */
    case BLE_GATT_REGISTER_OP_CHR:
        ESP_LOGD(TAG,
                 "registering characteristic %s with "
                 "def_handle=%d val_handle=%d",
                 ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                 ctxt->chr.def_handle, ctxt->chr.val_handle);
        break;

    /* Descriptor register event */
    case BLE_GATT_REGISTER_OP_DSC:
        ESP_LOGD(TAG, "registering descriptor %s with handle=%d",
                 ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                 ctxt->dsc.handle);
        break;

    /* Unknown event */
    default:
        assert(0);
        break;
    }
}

/*
 *  GATT server initialization
 *      1. Initialize GATT service
 *      2. Update NimBLE host GATT services counter
 *      3. Add GATT services to server
 */
int gatt_svc_init(void)
{
    /* Local variables */
    int rc;

    /* 1. GATT service initialization */
    ble_svc_gatt_init();

    /* 2. Update GATT services counter */
    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    /* 3. Add GATT services */
    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    return 0;
}