#include "access_control.h"
#include "gpio_config.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"

char access_code[5] = "2165";
int32_t unlock_duration = 10000; // ms, so 10 seconds
bool device_unlocked = false;

void init_nvs()
{
    nvs_flash_init();
    nvs_handle_t my_handle;
    nvs_open("storage", NVS_READWRITE, &my_handle);

    // Read stored code or set default
    size_t required_size = sizeof(access_code);
    if (nvs_get_str(my_handle, "access_code", access_code, &required_size) != ESP_OK)
    {
        nvs_set_str(my_handle, "access_code", access_code);
        nvs_commit(my_handle);
    }

    // Read stored unlock duration or set default
    if (nvs_get_i32(my_handle, "unlock_duration", &unlock_duration) != ESP_OK)
    {
        nvs_set_i32(my_handle, "unlock_duration", unlock_duration);
        nvs_commit(my_handle);
    }

    nvs_close(my_handle);
}

void update_nvs_access_code(char *new_code)
{
    nvs_handle_t my_handle;
    nvs_open("storage", NVS_READWRITE, &my_handle);

    nvs_set_str(my_handle, "access_code", new_code);
    nvs_commit(my_handle);
    nvs_close(my_handle);
}

void update_nvs_unlock_duration(int new_duration)
{
    nvs_handle_t my_handle;
    nvs_open("storage", NVS_READWRITE, &my_handle);

    nvs_set_i32(my_handle, "unlock_duration", new_duration);
    nvs_commit(my_handle);
    nvs_close(my_handle);
}

void blink_incorrect_code()
{
    for (int i = 0; i < 3; i++)
    {
        gpio_set_level(PIN_RED_LED, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(PIN_RED_LED, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void blink_success()
{
    for (int i = 0; i < 3; i++)
    {
        gpio_set_level(PIN_GREEN_LED, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(PIN_GREEN_LED, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void set_device_unlocked(bool state)
{
    device_unlocked = state;
}

bool is_device_unlocked()
{
    return device_unlocked;
}
