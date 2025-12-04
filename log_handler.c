#include "log_handler.h"

#include <stdio.h>
#include <string.h>

#include "board_config.h"
#include "eeprom_driver.h"

uint16_t crc16(const uint8_t *data_p, size_t length) {
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while (length--) {
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    return crc;
}

bool is_log_entry_valid(uint16_t entry_address) {
    uint8_t buffer[LOG_ENTRY_SIZE];

    eeprom_status status = eeprom_read_bytes(entry_address, buffer, LOG_ENTRY_SIZE);
    if (status != EEPROM_OK) {
        return false;
    }

    //1st byte is 0 --> empty
    if (buffer[0] == 0) {
        return false;
    }

    //find null terminator
    size_t string_length = 0;
    bool found_null = false;
    for (size_t i = 0; i < LOG_MAX_STRING_LENGTH + 1; i++) {
        if (buffer[i] == 0) {
            string_length = i;
            found_null = true;
            break;
        }
    }

    if (!found_null) {
        return false;
    }

    size_t crc_check_length = string_length + 1 + 2;
    uint16_t crc_result = crc16(buffer, crc_check_length);

    if (crc_result != 0) {
        return false;
    }
    return true;
}

int16_t find_next_free_entry(void) {
    for (int i = 0; i < LOG_MAX_ENTRIES; i++) {
        uint16_t entry_address = LOG_START_ADDRESS + (i * LOG_ENTRY_SIZE);
        if (!is_log_entry_valid(entry_address)) {
            return entry_address;
        }
    }
    return -1; //full log
}

bool log_write_entry(const char *message) {
    if (!message) {
        return false;
    }

    size_t message_len = strlen(message);

    //message len 0<x<61
    if (message_len == 0 || message_len > LOG_MAX_STRING_LENGTH) {
        printf("Log message length ERROR\n");
        return false;
    }

    //find next free entry
    int16_t entry_address = find_next_free_entry();

    //full log -> erase
    if (entry_address < 0) {
        printf("Log full, erasing...\n");
        log_erase();
        entry_address = LOG_START_ADDRESS;
    }

    // Prepare buffer: message + null + CRC (MSB, LSB)
    uint8_t buffer[LOG_ENTRY_SIZE] = {0};
    //memset(buffer, 0, LOG_ENTRY_SIZE);

    // Copy message and null terminator
    strcpy((char *)buffer, message);

    // Calculate CRC over message + null
    size_t data_length = message_len + 1;
    uint16_t crc = crc16(buffer, data_length);

    // Add CRC
    buffer[data_length] = (uint8_t)(crc >> 8);
    buffer[data_length + 1] = (uint8_t)(crc & 0xFF);

    // Write to EEPROM
    eeprom_status status = eeprom_write_bytes(entry_address, buffer, LOG_ENTRY_SIZE); //!
    if (status != EEPROM_OK) {
        printf("Failed to write log entry\n");
        return false;
    }

    return true;
}

void log_read_all(void) {
    printf("\n---------LOG--------\n");

    for (int i = 0; i < LOG_MAX_ENTRIES; i++) {
        uint16_t entry_address = LOG_START_ADDRESS + (i * LOG_ENTRY_SIZE);

        if (!is_log_entry_valid(entry_address)) {
            break;
        }

        // Read and print the entry
        uint8_t buffer[LOG_ENTRY_SIZE];
        eeprom_status status = eeprom_read_bytes(entry_address, buffer, LOG_ENTRY_SIZE);
        if (status == EEPROM_OK) {
            printf("%d: %s\n", i, (char *)buffer);
        }
    }

    printf("---------END--------\n\n");
}

// write 0 at the begin
void log_erase(void) {

    for (int i = 0; i < LOG_MAX_ENTRIES; i++) {
        uint16_t entry_address = LOG_START_ADDRESS + (i * LOG_ENTRY_SIZE);
        eeprom_status status = eeprom_write_byte(entry_address, 0);
        if (status != EEPROM_OK) {
            printf("Error erasing log entry %d\n", i);
        }
    }
    printf("Log erased\n");
}