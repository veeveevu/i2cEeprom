#include "eeprom_driver.h"

#include <stdio.h>
#include <string.h>

#include "led_state_data.h"
#include "log_handler.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

eeprom_status eeprom_write_byte(uint16_t address, uint8_t data) {

    uint8_t buffer[3];
    buffer[0] = (uint8_t)(address >> 8); //top 8 bits (high byte)
    buffer[1] = (uint8_t)(address & 0xFF); //& 0000 0000 1111 1111 --> bottom 8 bits (low byte)
    buffer[2] = data;

    int write = i2c_write_blocking(I2C_PORT, EEPROM_I2C_ADDRESS, buffer, 3, false);

    if (write < 0) {
        return EEPROM_ERR_I2C;
    }
    if (write != 3) {
        return EEPROM_ERR_PARTIAL;
    }

    sleep_ms(EEPROM_WRITE_DELAY_MS);
    return EEPROM_OK;
}

eeprom_status eeprom_read_byte(uint16_t address, uint8_t *output) {
    if (!output) {
        return EEPROM_ERR_I2C;
    }

    uint8_t address_buffer[2];
    address_buffer[0] = address >> 8;
    address_buffer[1] = address & 0xFF;

    int write = i2c_write_blocking(I2C_PORT, EEPROM_I2C_ADDRESS, address_buffer, 2, true);
    if (write < 0) {
        return EEPROM_ERR_I2C;
    }
    if (write != 2) {
        return EEPROM_ERR_PARTIAL;
    }

    int read = i2c_read_blocking(I2C_PORT, EEPROM_I2C_ADDRESS, output, 1, false);
    if (read < 0) {
        return EEPROM_ERR_I2C;
    }
    if (read != 1) {
        return EEPROM_ERR_PARTIAL;
    }
    return EEPROM_OK;
}

eeprom_status eeprom_write_bytes(uint16_t address, uint8_t *data, size_t len) {
    if (!data) {
        return EEPROM_ERR_I2C;
    }

    if (len > EEPROM_PAGE_SIZE || len == 0) {
        return EEPROM_ERR_PARTIAL;
    }

    uint8_t buffer[EEPROM_PAGE_SIZE + 2];
    buffer[0] = (uint8_t)(address >> 8); //top 8 bits (high byte)
    buffer[1] = (uint8_t)(address & 0xFF); //& 0000 0000 1111 1111 --> bottom 8 bits (low byte)

    memcpy(&buffer[2], data, len);

    int write = i2c_write_blocking(I2C_PORT, EEPROM_I2C_ADDRESS, buffer, len + 2, false);

    if (write < 0) {
        return EEPROM_ERR_I2C;
    }
    if (write != (int)(len + 2)) {
        return EEPROM_ERR_PARTIAL;
    }

    sleep_ms(EEPROM_WRITE_DELAY_MS);
    return EEPROM_OK;

}
eeprom_status eeprom_read_bytes(uint16_t address, uint8_t *output, size_t len) {

    if (!output) {
        return EEPROM_ERR_I2C;
    }
    if (len > EEPROM_MAX_SIZE || len == 0) {
        return EEPROM_ERR_PARTIAL;
    }

    uint8_t address_buffer[2];
    address_buffer[0] = address >> 8;
    address_buffer[1] = address & 0xFF;

    int write = i2c_write_blocking(I2C_PORT, EEPROM_I2C_ADDRESS, address_buffer, 2, true);
    if (write < 0) {
        return EEPROM_ERR_I2C;
    }
    if (write != 2) {
        return EEPROM_ERR_PARTIAL;
    }

    int read = i2c_read_blocking(I2C_PORT, EEPROM_I2C_ADDRESS, output, len, false);
    if (read < 0) {
        return EEPROM_ERR_I2C;
    }
    if (read != (int)len) {
        return EEPROM_ERR_PARTIAL;
    }
    return EEPROM_OK;

}

void save_state_to_eeprom(bool led_d1_on, bool led_d2_on, bool led_d3_on) {
    uint8_t new_state_byte = 0;
    ledstate new_state;

    if (led_d1_on) {
        new_state_byte = new_state_byte + 1; //001
    }

    if (led_d2_on) {
        new_state_byte = new_state_byte + 2; //010
    }

    if (led_d3_on) {
        new_state_byte = new_state_byte + 4;  //100
    }

    set_led_state(&new_state, new_state_byte);
    uint8_t wr_st_byte = eeprom_write_byte(LED_STATE_ADDRESS, new_state.state);
    if (wr_st_byte != EEPROM_OK) {
        printf("EEPROM write (state byte) ERROR: %d\n", wr_st_byte);
    }
    uint8_t wr_not_st_byte = eeprom_write_byte(LED_STATE_ADDRESS + 1, new_state.not_state);
    if (wr_not_st_byte != EEPROM_OK) {
        printf("EEPROM write (not_state byte) ERROR: %d\n", wr_not_st_byte);
    }

    double time_sec = time_us_64() / 1000000.0;
    char message[LOG_MAX_STRING_LENGTH];
    snprintf(message, sizeof(message),
             "%.2f (s) - Saved to EEPROM: 0x%02X",
             time_sec, new_state_byte);

    printf("%s\n\n", message);

    log_write_entry(message);
}
