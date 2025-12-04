#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "board_config.h"
#include "led_state_data.h"
#include "eeprom_driver.h"
#include "leds_buttons_handler.h"
#include "log_handler.h"

#define CMD_BUFFER_SIZE 64
static char cmd_buffer[CMD_BUFFER_SIZE];
static int cmd_index = 0;

//non-blocking command input
void handle_input_command(void) {
    int c = getchar_timeout_us(0); //non-blocking read

    if (c != PICO_ERROR_TIMEOUT) {
        if (c == '\n' || c == '\r') {
            putchar('\n');
            cmd_buffer[cmd_index] = '\0';

            if (cmd_index > 0) {
                if (strcmp(cmd_buffer, "read") == 0) {
                    log_read_all();
                }
                else if (strcmp(cmd_buffer, "erase") == 0) {
                    log_erase();
                }
                else {
                    printf("Unknown command: %s\n", cmd_buffer);
                }
            }
            cmd_index = 0; // Reset buffer
        }
        else {
            if (cmd_index < CMD_BUFFER_SIZE - 1) {
                cmd_buffer[cmd_index++] = (char)c;
            }
        }
    }
}

int main() {
    stdio_init_all();

    init_hardware();

    printf("System booted.\n");
    log_write_entry("Boot");

    ledstate saved_state;
    uint8_t state_byte = 0x02;
    uint8_t not_state_byte = (uint8_t)~state_byte;

    eeprom_status status = eeprom_read_byte(LED_STATE_ADDRESS, &state_byte);
    if (status != EEPROM_OK) {
        printf("EEPROM read (state byte) ERROR: %d - using default 0x%02X\n", status, state_byte);
    }
    status = eeprom_read_byte(LED_STATE_ADDRESS + 1, &not_state_byte);
    if (status != EEPROM_OK) {
        printf("EEPROM read (not_state byte) ERROR: %d - using default 0x%02X\n", status, not_state_byte);
    }

    saved_state.state = state_byte;
    saved_state.not_state = not_state_byte;
    printf("Read: state = 0x%02X, not_state = 0x%02X\n", saved_state.state, saved_state.not_state);

    bool led_d1_on;
    bool led_d2_on;
    bool led_d3_on;

    bool is_saved_data_valid = load_led_state(&saved_state, &led_d1_on, &led_d2_on, &led_d3_on);

    change_leds(led_d1_on, led_d2_on, led_d3_on);

    //uint64_t current_time = time_us_64();
    double time_sec = time_us_64() / 1000000.0;
    printf("%.2f (s) - State: 0x%02X\n\n", time_sec, saved_state.state);

    if (!is_saved_data_valid) {
        save_state_to_eeprom(led_d1_on, led_d2_on, led_d3_on);
    }

    while (true) {
        bool is_state_changed = handle_buttons_press(&led_d1_on, &led_d2_on, &led_d3_on);

        if (is_state_changed) {
            save_state_to_eeprom(led_d1_on, led_d2_on, led_d3_on);
        }
        handle_input_command();
    }
}