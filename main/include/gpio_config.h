// keypad.h
#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "driver/gpio.h"

// GPIO definitions
#define PIN_RED_LED 18
#define PIN_GREEN_LED 19

#define COLUMNS {27, 12, 17}
#define ROWS {14, 26, 25, 16}

void init_gpio();

#endif
