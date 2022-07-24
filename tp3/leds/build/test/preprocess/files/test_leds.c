#include "src/leds.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"
static uint16_t virtual_leds;





void setUp(void) {

    LEDS_init(&virtual_leds);

}



void tearDown(void) {



}





void test_leds_off_at_startup(void) {

    uint16_t virtual_leds = 0xffff;



    LEDS_init(&virtual_leds);



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((virtual_leds)), (

   ((void *)0)

   ), (UNITY_UINT)(38), UNITY_DISPLAY_STYLE_INT);

}





void test_turn_on_single_led(void) {

    LEDS_turn_on(3);



    UnityAssertEqualNumber((UNITY_INT)((1u << 2u)), (UNITY_INT)((virtual_leds)), (

   ((void *)0)

   ), (UNITY_UINT)(45), UNITY_DISPLAY_STYLE_INT);

}





void test_turn_off_single_led(void) {

    LEDS_turn_on(3);

    LEDS_turn_off(3);



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((virtual_leds)), (

   ((void *)0)

   ), (UNITY_UINT)(53), UNITY_DISPLAY_STYLE_INT);

}





void test_turn_on_multiple_leds(void) {

    LEDS_turn_on(3);

    LEDS_turn_on(7);



    LEDS_turn_off(7);



    UnityAssertEqualNumber((UNITY_INT)((1u << 2u)), (UNITY_INT)((virtual_leds)), (

   ((void *)0)

   ), (UNITY_UINT)(63), UNITY_DISPLAY_STYLE_INT);

}





void test_turn_on_all_leds_at_once(void) {

    LEDS_turn_on_all();



    UnityAssertEqualNumber((UNITY_INT)((0xffff)), (UNITY_INT)((virtual_leds)), (

   ((void *)0)

   ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_INT);

}





void test_turn_off_all_leds_at_once(void) {

    LEDS_turn_on_all();



    LEDS_turn_off_all();



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((virtual_leds)), (

   ((void *)0)

   ), (UNITY_UINT)(79), UNITY_DISPLAY_STYLE_INT);

}





void test_get_status_led_on(void) {

    LEDS_turn_on(3);



    UnityAssertEqualNumber((UNITY_INT)((

   1

   )), (UNITY_INT)((LEDS_get_status(3))), (

   ((void *)0)

   ), (UNITY_UINT)(86), UNITY_DISPLAY_STYLE_INT);

}





void test_get_status_led_off(void) {

    UnityAssertEqualNumber((UNITY_INT)((

   0

   )), (UNITY_INT)((LEDS_get_status(1))), (

   ((void *)0)

   ), (UNITY_UINT)(91), UNITY_DISPLAY_STYLE_INT);

}
