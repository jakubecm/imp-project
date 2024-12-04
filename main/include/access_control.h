/**
 * @file access_control.h
 * @brief Header file for access control functionalities.
 * @author Milan Jakubec (xjakub41)
 *
 * This file contains the declarations for functions and variables used
 * to manage access control, including setting and verifying access codes,
 * managing unlock durations, and indicating success or failure states.
 */

#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Access code for unlocking the device.
 */
extern char access_code[5];

/**
 * @brief Duration for which the device remains unlocked, in milliseconds.
 */
extern int32_t unlock_duration;

/**
 * @brief Indicates whether the device is currently unlocked.
 */
extern bool device_unlocked;

/**
 * @brief Initializes the non-volatile storage (NVS).
 */
void init_nvs();

/**
 * @brief Updates the access code stored in NVS.
 *
 * @param new_code The new access code to be stored.
 */
void update_nvs_access_code(char *new_code);

/**
 * @brief Updates the unlock duration stored in NVS.
 *
 * @param new_duration The new unlock duration in milliseconds.
 */
void update_nvs_unlock_duration(int new_duration);

/**
 * @brief Blinks an LED to indicate an incorrect access code.
 */
void blink_incorrect_code();

/**
 * @brief Blinks an LED to indicate a successful access code entry.
 */
void blink_success();

/**
 * @brief Sets the device's unlocked state.
 *
 * @param state The new unlocked state (true for unlocked, false for locked).
 */
void set_device_unlocked(bool state);

/**
 * @brief Checks if the device is currently unlocked.
 *
 * @return true if the device is unlocked, false otherwise.
 */
bool is_device_unlocked();

#endif // ACCESS_CONTROL_H