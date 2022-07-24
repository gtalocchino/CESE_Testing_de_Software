/**
 * @file leds.h
 * @author Gianfranco Talocchino (gftalocchino@gmail.com)
 * @brief LEDs driver module
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief Initializes and turns off all LEDs.
 * 
 * @param address Address of the GPIO port driving the LEDs.
 */
void LEDS_init(uint16_t *address);

/**
 * @brief Turns on one LED individually.
 * 
 * @param led Number of the LED to turn on (1 to 16).
 */
void LEDS_turn_on(uint8_t led);

/**
 * @brief Turns off one LED individually.
 * 
 * @param led Number of the LED to turn off (1 to 16).
 */
void LEDS_turn_off(uint8_t led);

/**
 * @brief Turns on all LEDs.
 * 
 */
void LEDS_turn_on_all(void);

/**
 * @brief Turns off all LEDs.
 * 
 */
void LEDS_turn_off_all(void);

/**
 * @brief Gets the state of a LED.
 * 
 * @param led umber of the LED to get status (1 to 16).
 * @return true if the LED is on.
 * @return false if the LED is off.
 */
bool LEDS_get_status(uint8_t led);

#endif
