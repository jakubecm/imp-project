// main.c
#include "keypad.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "gpio_config.h"
#include "access_control.h"
#include <stdio.h>
#include <string.h>

#include "gap.h"
#include "gatt_svc.h"
#include "nimble/ble.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"

/* Library function declarations */
void ble_store_config_init(void);

/* Private function declarations */
static void on_stack_reset(int reason);
static void on_stack_sync(void);
static void nimble_host_config_init(void);
static void nimble_host_task(void *param);

/* Private functions */
/*
 *  Stack event callback functions
 *      - on_stack_reset is called when host resets BLE stack due to errors
 *      - on_stack_sync is called when host has synced with controller
 */
static void on_stack_reset(int reason)
{
    /* On reset, print reset reason to console */
    ESP_LOGI(TAG, "nimble stack reset, reset reason: %d", reason);
}

static void on_stack_sync(void)
{
    /* On stack sync, do advertising initialization */
    adv_init();
}

static void nimble_host_config_init(void)
{
    /* Set host callbacks */
    ble_hs_cfg.reset_cb = on_stack_reset;
    ble_hs_cfg.sync_cb = on_stack_sync;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    /* Store host configuration */
    ble_store_config_init();
}

static void nimble_host_task(void *param)
{
    /* Task entry log */
    ESP_LOGI(TAG, "nimble host task has been started!");

    /* This function won't return until nimble_port_stop() is executed */
    nimble_port_run();

    /* Clean up at exit */
    vTaskDelete(NULL);
}

void app_main()
{
    /* Local variables */
    int rc;
    esp_err_t ret;

    init_gpio();
    init_nvs();

    /* Initialize the NimBLE host. */
    ret = nimble_port_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "NimBLE port init failed, error code = %d", ret);
        return;
    }

    rc = gap_init();
    if (rc != 0)
    {
        ESP_LOGE(TAG, "GAP initialization failed, error code = %d", rc);
        return;
    }

    rc = gatt_svc_init();
    if (rc != 0)
    {
        ESP_LOGE(TAG, "GATT service initialization failed, error code = %d", rc);
        return;
    }

    nimble_host_config_init();
    xTaskCreate(nimble_host_task, "nimble_host_task", 4 * 1024, NULL, 5, NULL);

    char input_code[11] = {0}; // Buffer for the code entered
    int input_pos = 0;         // Position in the buffer

    while (1)
    {
        char key = scan_keypad(); // Scan for keypress
        if (key != '\0')
        {
            printf("Key pressed: %c\n", key);
            input_code[input_pos++] = key; // Store keypress and increment position
            if (input_pos == 4 && input_code[0] != '#')
            {
                input_code[input_pos] = '\0'; // Null terminate string

                if (strcmp(input_code, access_code) == 0)
                {
                    set_device_unlocked(true);

                    gpio_set_level(PIN_GREEN_LED, 1); // Turn on green LED
                    gpio_set_level(PIN_RED_LED, 0);   // Turn off red LED

                    vTaskDelay(unlock_duration / portTICK_PERIOD_MS); // Keep green LED on for 10 seconds
                    set_device_unlocked(false);                       // Lock device after 10 seconds
                }
                else
                {
                    blink_incorrect_code();
                    gpio_set_level(PIN_RED_LED, 1); // Incorrect code, keep red LED on
                }

                // Reset for next input
                input_pos = 0;
                memset(input_code, 0, sizeof(input_code)); // Clear the input buffer
                gpio_set_level(PIN_GREEN_LED, 0);          // Turn off green LED
                gpio_set_level(PIN_RED_LED, 1);            // Turn red LED back on
            }

            else if (input_pos == 10 && input_code[0] == '#' && input_code[5] == '*')
            {
                input_code[input_pos] = '\0'; // Null terminate string

                char current_code[5];
                strncpy(current_code, input_code + 1, 4); // Extract the current access code part
                current_code[4] = '\0';

                char new_code[5];
                strncpy(new_code, input_code + 6, 4); // Extract the new access code part
                new_code[4] = '\0';

                if (strcmp(current_code, access_code) == 0)
                {
                    // Update the access code
                    strcpy(access_code, new_code);
                    update_nvs_access_code(access_code); // Assume function to update NVS

                    gpio_set_level(PIN_RED_LED, 0);

                    blink_success();
                    gpio_set_level(PIN_GREEN_LED, 1);
                    set_device_unlocked(true);
                    vTaskDelay(unlock_duration / portTICK_PERIOD_MS); // Keep green LED on for 10 seconds
                    set_device_unlocked(false);                       // Lock device after 10 seconds
                }
                else
                {
                    blink_incorrect_code();
                }

                // Reset for next input
                input_pos = 0;
                memset(input_code, 0, sizeof(input_code)); // Clear the input buffer
                gpio_set_level(PIN_GREEN_LED, 0);          // Turn off green LED
                gpio_set_level(PIN_RED_LED, 1);            // Turn red LED back on
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Polling delay
    }
}
