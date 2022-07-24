#include "unity.h"
#include "leds.h"


/*
 * - LIST OF TESTS
 * 
 * -  1. After initialization all LEDs are switched off
 * -  2. Turn on a single LED
 * -  3. Turn off a single LED
 * -  4. Turn multiple LED's on and off
 * -  5. Turn on all LEDs at once
 * -  6. Turn off all LEDs at once
 * -  7. Get the status of a LED that is on
 * -  8. Get the status of a LED that is off
 * -  9. Check parameter limits (optional)
 * - 10. Check parameters out of limits (optional)
 */


static uint16_t virtual_leds;


void setUp(void) {
    LEDS_init(&virtual_leds);
}

void tearDown(void) {
    /* Empty. */
}

/* 1. After initialization all LEDs should be off. */
void test_leds_off_at_startup(void) {
    uint16_t virtual_leds = 0xffff;

    LEDS_init(&virtual_leds);

    TEST_ASSERT_EQUAL(0, virtual_leds);
}

/* 2. Turn on a single LED. */
void test_turn_on_single_led(void) {
    LEDS_turn_on(3);

    TEST_ASSERT_EQUAL(1u << 2u, virtual_leds);
}

/* 3. Turn off a single LED. */
void test_turn_off_single_led(void) {
    LEDS_turn_on(3);
    LEDS_turn_off(3);

    TEST_ASSERT_EQUAL(0, virtual_leds);
}

/* 4. Turn multiple LED's on and off. */
void test_turn_on_multiple_leds(void) {
    LEDS_turn_on(3);
    LEDS_turn_on(7);
  
    LEDS_turn_off(7);

    TEST_ASSERT_EQUAL(1u << 2u, virtual_leds);
}

 /* 5. Turn on all LEDs at once. */
void test_turn_on_all_leds_at_once(void) {
    LEDS_turn_on_all();

    TEST_ASSERT_EQUAL(0xffff, virtual_leds);
}

 /* 6. Turn off all LEDs at once. */
void test_turn_off_all_leds_at_once(void) {
    LEDS_turn_on_all();

    LEDS_turn_off_all();

    TEST_ASSERT_EQUAL(0, virtual_leds);
}

 /* 7. Get the status of a LED that is on. */
void test_get_status_led_on(void) {
    LEDS_turn_on(3);

    TEST_ASSERT_EQUAL(true, LEDS_get_status(3));
} 

 /* 8. Get the status of a LED that is off. */
void test_get_status_led_off(void) {
    TEST_ASSERT_EQUAL(false, LEDS_get_status(1));
}
