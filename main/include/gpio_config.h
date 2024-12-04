/**
 * @file gpio_config.h
 * @brief Header file for GPIO configuration of the ESP32.
 * @author Milan Jakubec (xjakub41)
 *
 * This file contains the declarations for functions used to initialize the GPIO pins and their macros
 */
#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "driver/gpio.h"

// GPIO definitions
#define PIN_RED_LED 18
#define PIN_GREEN_LED 19

#define COLUMNS {27, 12, 17}
#define ROWS {14, 26, 25, 16}

/**
 * @brief Initialize the GPIO pins.
 */
void init_gpio();

#endif
