#include "leds_buttons_handler.h"

#include "hardware/i2c.h"
#include "pico/stdlib.h"

void led_init(int led) {
    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);
    gpio_put(led, 0);
}

void button_init(int button) {
    gpio_init(button);
    gpio_set_dir(button, GPIO_IN);
    gpio_pull_up(button);
}

void init_hardware(void) {
    //-----------i2c
    i2c_init(i2c0, I2C_FREQUENCY);

    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    //----------leds
    led_init(LED_D1_PIN);
    led_init(LED_D2_PIN);
    led_init(LED_D3_PIN);

    //-----------buttons
    button_init(SW0_PIN);
    button_init(SW1_PIN);
    button_init(SW2_PIN);
}

void change_leds(bool d1, bool d2, bool d3) {
    if (d1) {
        gpio_put(LED_D1_PIN, 1);
    } else {
        gpio_put(LED_D1_PIN, 0);
    }

    if (d2) {
        gpio_put(LED_D2_PIN, 1);
    } else {
        gpio_put(LED_D2_PIN, 0);
    }

    if (d3) {
        gpio_put(LED_D3_PIN, 1);
    } else {
        gpio_put(LED_D3_PIN, 0);
    }
}

bool handle_buttons_press(bool *led_d1_on, bool *led_d2_on, bool *led_d3_on) {
    bool is_state_changed = false;

    if (gpio_get(SW2_PIN) == 0) {
        *led_d1_on = !(*led_d1_on);

        change_leds(*led_d1_on, *led_d2_on, *led_d3_on);
        is_state_changed = true;

        while (gpio_get(SW2_PIN) == 0) {
            sleep_ms(DEBOUNCE_MS);
        }
    }

    if (gpio_get(SW1_PIN) == 0) {
        *led_d2_on = !(*led_d2_on);

        change_leds(*led_d1_on, *led_d2_on, *led_d3_on);
        is_state_changed = true;

        while (gpio_get(SW1_PIN) == 0) {
            sleep_ms(DEBOUNCE_MS);
        }
    }

    if (gpio_get(SW0_PIN) == 0) {
        *led_d3_on = !(*led_d3_on);

        change_leds(*led_d1_on, *led_d2_on, *led_d3_on);
        is_state_changed = true;

        while (gpio_get(SW0_PIN) == 0) {
            sleep_ms(DEBOUNCE_MS);
        }
    }

    return is_state_changed;
}