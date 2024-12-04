#include "gpio_config.h"

// GPIO pin setup
void init_gpio()
{
    gpio_reset_pin(PIN_RED_LED);
    gpio_reset_pin(PIN_GREEN_LED);
    int cols[] = COLUMNS;
    int rows[] = ROWS;
    for (int i = 0; i < 3; i++)
    {
        gpio_reset_pin(cols[i]);
    }
    for (int j = 0; j < 4; j++)
    {
        gpio_reset_pin(rows[j]);
    }
    // Initialize LEDs
    gpio_set_direction(PIN_RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_GREEN_LED, GPIO_MODE_OUTPUT);

    // Columns as outputs
    for (int i = 0; i < 3; i++)
    {
        gpio_set_direction(cols[i], GPIO_MODE_OUTPUT);
        gpio_set_level(cols[i], 1); // Initialize as high (inactive)
    }

    // Rows as inputs with pull-ups
    for (int j = 0; j < 4; j++)
    {
        gpio_set_direction(rows[j], GPIO_MODE_INPUT);
        gpio_set_pull_mode(rows[j], GPIO_PULLUP_ONLY);
    }

    gpio_set_level(PIN_GREEN_LED, 0); // Turn green LED off
    gpio_set_level(PIN_RED_LED, 1);   // Turn red LED on
}