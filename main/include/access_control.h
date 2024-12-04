#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

extern char access_code[5];
extern int32_t unlock_duration; // ms, so 10 seconds
extern bool device_unlocked;

void init_nvs();
void update_nvs_access_code(char *new_code);
void update_nvs_unlock_duration(int new_duration);
void blink_incorrect_code();
void blink_success();

void set_device_unlocked(bool state);
bool is_device_unlocked();
#endif // ACCESS_CONTROL_H
