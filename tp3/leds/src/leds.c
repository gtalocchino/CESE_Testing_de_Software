/**
 * @file leds.c
 * @author Gianfranco Talocchino (gftalocchino@gmail.com)
 * @brief LEDs driver module
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "leds.h"

#define LEDS_OFFSET  (1u)
#define FIRST_BIT_ON (1u)

#define ALL_LEDS_OFF (0x0)
#define ALL_LEDS_ON  (0xffff);

static uint16_t *port;


static uint16_t led_to_mask(uint8_t led) {
    return FIRST_BIT_ON << (led - LEDS_OFFSET);
}

void LEDS_init(uint16_t *address) {
    port = address;
    *port = ALL_LEDS_OFF;
}

void LEDS_turn_on(uint8_t led) {
    *port |= led_to_mask(led);
}

void LEDS_turn_off(uint8_t led) {
    *port &= ~led_to_mask(led);
}

void LEDS_turn_on_all(void) {
    *port = ALL_LEDS_ON;
}

void LEDS_turn_off_all(void) {
    *port = ALL_LEDS_OFF;
}

bool LEDS_get_status(uint8_t led) {
    return (*port & led_to_mask(led)) ? true : false;
}
