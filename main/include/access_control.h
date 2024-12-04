#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

extern char access_code[5];

void init_nvs();
void update_nvs_access_code(char *new_code);
void blink_incorrect_code();
void blink_success();

#endif // ACCESS_CONTROL_H
