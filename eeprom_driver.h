
#ifndef I2CEEPROM_EEPROM_DRIVER_H
#define I2CEEPROM_EEPROM_DRIVER_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "board_config.h"

typedef enum eeprom_status {
    EEPROM_OK = 0,
    EEPROM_ERR_I2C = -1,
    EEPROM_ERR_PARTIAL = -2,
} eeprom_status;

eeprom_status eeprom_write_byte(uint16_t address, uint8_t data);
eeprom_status eeprom_read_byte(uint16_t address, uint8_t *output);

eeprom_status eeprom_write_bytes(uint16_t address, uint8_t *data, size_t len);
eeprom_status eeprom_read_bytes(uint16_t address, uint8_t *output, size_t len);

void save_state_to_eeprom(bool led_d1_on, bool led_d2_on, bool led_d3_on);

#endif //I2CEEPROM_EEPROM_DRIVER_H