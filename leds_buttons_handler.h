#ifndef I2CEEPROM_LEDS_BUTTONS_HANDLER_H
#define I2CEEPROM_LEDS_BUTTONS_HANDLER_H

#include <stdbool.h>
#include "board_config.h"


void led_init(int led);
void button_init(int button);
void change_leds(bool d1, bool d2, bool d3);
void init_hardware(void);
bool handle_buttons_press(bool *led_d1_on, bool *led_d2_on, bool *led_d3_on);


#endif //I2CEEPROM_LEDS_BUTTONS_HANDLER_H