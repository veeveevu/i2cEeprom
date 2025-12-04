#ifndef I2CEEPROM_LOG_HANDLER_H
#define I2CEEPROM_LOG_HANDLER_H


#include "pico/stdlib.h"
#include "hardware/i2c.h"

uint16_t crc16(const uint8_t *data_p, size_t length);

bool is_log_entry_valid(uint16_t entry_address);
int16_t find_next_free_entry(void);
bool log_write_entry(const char *message);
void log_read_all(void);
void log_erase(void);

#endif //I2CEEPROM_LOG_HANDLER_H