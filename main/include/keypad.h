// keypad.h
#ifndef KEYPAD_H
#define KEYPAD_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO definitions
#define PIN_RED_LED 18
#define PIN_GREEN_LED 19
// Pins connected to keyboard:
#define COLUMNS {27, 12, 17}
#define ROWS {14, 26, 25, 16}
#define DEBOUNCE_TIME 25 // milliseconds

char scan_keypad();

#endif // KEYPAD_H
