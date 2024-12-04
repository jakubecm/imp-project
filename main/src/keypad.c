// keypad.c
#include "keypad.h"
#include <string.h>
#include <stdio.h>

// Key scanning function
char scan_keypad()
{
    static unsigned long last_press_time = 0;
    static char last_key_pressed = '\0';
    static bool key_released = true;

    unsigned long current_time = xTaskGetTickCount();

    int cols[] = COLUMNS;
    int rows[] = ROWS;
    char keys[4][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};

    for (int col = 0; col < 3; col++)
    {
        gpio_set_level(cols[col], 0);        // Activate column
        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to stabilize input

        for (int row = 0; row < 4; row++)
        {
            if (gpio_get_level(rows[row]) == 0)
            {
                if (last_key_pressed != keys[row][col] || key_released)
                {
                    if ((current_time - last_press_time) > DEBOUNCE_TIME)
                    {
                        last_press_time = current_time;    // Update last press time for debouncing
                        last_key_pressed = keys[row][col]; // Record last key pressed
                        key_released = false;              // Mark key as not released
                        gpio_set_level(cols[col], 1);      // Deactivate column immediately after detection
                        return keys[row][col];             // Return the pressed key
                    }
                }
            }
            else if (last_key_pressed == keys[row][col])
            {
                key_released = true; // Mark key as released if it is no longer pressed
            }
        }
        gpio_set_level(cols[col], 1); // Deactivate column regardless of detection just in case
    }
    return '\0'; // No key pressed
}
