#include "led_state_data.h"

#include <stdio.h>

void set_led_state(ledstate* ls, uint8_t value) {
    ls->state = value;
    ls->not_state = ~value;
}

bool led_state_is_valid(const ledstate* ls) {
    return ls->state == (uint8_t)~ls->not_state;
}
bool load_led_state(ledstate *saved_state, bool *led_d1_on, bool *led_d2_on, bool *led_d3_on) {
    if (led_state_is_valid(saved_state)) {
        printf("Valid data!\n");
        if (saved_state->state == 0) {  //000
            *led_d1_on = false;
            *led_d2_on = false;
            *led_d3_on = false;
        } else if (saved_state->state == 1) {  //001
            *led_d1_on = true;
            *led_d2_on = false;
            *led_d3_on = false;
        } else if (saved_state->state == 2) {  //010
            *led_d1_on = false;
            *led_d2_on = true;
            *led_d3_on = false;
        } else if (saved_state->state == 3) {  //011
            *led_d1_on = true;
            *led_d2_on = true;
            *led_d3_on = false;
        } else if (saved_state->state == 4) {  //100
            *led_d1_on = false;
            *led_d2_on = false;
            *led_d3_on = true;
        } else if (saved_state->state == 5) {  //101
            *led_d1_on = true;
            *led_d2_on = false;
            *led_d3_on = true;
        } else if (saved_state->state == 6) {  //110
            *led_d1_on = false;
            *led_d2_on = true;
            *led_d3_on = true;
        } else if (saved_state->state == 7) {  //111
            *led_d1_on = true;
            *led_d2_on = true;
            *led_d3_on = true;
        } else {
            printf("Error in state value!\n");
            *led_d1_on = false;
            *led_d2_on = true;
            *led_d3_on = false;
        }
        return true;
    } else {
        printf("Invalid data! Turn only middle on!\n");
        *led_d1_on = false;
        *led_d2_on = true;
        *led_d3_on = false;
        return false;
    }
}
