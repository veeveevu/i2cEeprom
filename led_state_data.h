#ifndef I2CEEPROM_LED_STATE_DTA_H
#define I2CEEPROM_LED_STATE_DTA_H

#include <stdint.h>
#include <stdbool.h>
#include "board_config.h"

typedef struct ledstate {
    uint8_t state;
    uint8_t not_state;
} ledstate;

#define LED_STATE_ADDRESS (EEPROM_MAX_SIZE - (int)sizeof(ledstate))

void set_led_state(ledstate* ls, uint8_t value);
bool led_state_is_valid(const ledstate* ls);
bool load_led_state(ledstate *saved_state, bool *led_d1_on, bool *led_d2_on, bool *led_d3_on);

#endif //I2CEEPROM_LED_STATE_DTA_H