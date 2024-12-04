// main.c
#include "keypad.h"
#include "gpio_config.h"
#include "access_control.h"
#include <stdio.h>
#include <string.h>

void app_main()
{
    init_gpio();
    init_nvs();

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
                    gpio_set_level(PIN_GREEN_LED, 1);       // Turn on green LED
                    gpio_set_level(PIN_RED_LED, 0);         // Turn off red LED
                    vTaskDelay(10000 / portTICK_PERIOD_MS); // Keep green LED on for 10 seconds
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
                    vTaskDelay(10000 / portTICK_PERIOD_MS); // Keep green LED on for 10 seconds
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
