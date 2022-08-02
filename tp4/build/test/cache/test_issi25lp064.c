#include "build/test/mocks/mock_issi25lp064_port.h"
#include "src/issi25lp064.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"






void fake_correct_id_ISSI25LP064_PORT_receive_spi(uint8_t *data) {

   *data = 0x9d;

}



void fake_wrong_id_ISSI25LP064_PORT_receive_spi(uint8_t *data) {

   *data = 0x00;

}



void fake_receive_zero_ISSI25LP064_PORT_receive_spi(uint8_t *data) {

   *data = 0;

}



void fake_receive_busy_one_time_ISSI25LP064_PORT_receive_spi(uint8_t *data) {

   static uint8_t busy_state = 0;



   switch (busy_state) {

      case 0:

         busy_state++;

         *data = 0b10;

         break;

      case 1:

         busy_state++;

         *data = 0b10;

         break;

      case 2:

      default:

         busy_state = 0;

         *data = 0;

         break;

   }

}





void test_ISSI25LP064_init_sent_command(void) {

   ISSI25LP064_init();



   UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((ISSI25LP064_PORT_transmit_spi_fake.arg0_val)), (UNITY_INT)(UNITY_UINT8 )((0x9f)), (

  ((void *)0)

  ), (UNITY_UINT)(44), UNITY_DISPLAY_STYLE_UINT8);

}











void test_initialization_read_correct_id(void) {

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_correct_id_ISSI25LP064_PORT_receive_spi;



   UnityAssertEqualNumber((UNITY_INT)((issi25lp064_OK)), (UNITY_INT)((ISSI25LP064_init())), (

  ((void *)0)

  ), (UNITY_UINT)(54), UNITY_DISPLAY_STYLE_INT);

}











void test_initialization_read_wrong_id(void) {

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_wrong_id_ISSI25LP064_PORT_receive_spi;



   UnityAssertEqualNumber((UNITY_INT)((issi25lp064_ERROR)), (UNITY_INT)((ISSI25LP064_init())), (

  ((void *)0)

  ), (UNITY_UINT)(64), UNITY_DISPLAY_STYLE_INT);

}











void test_read(void) {

   uint8_t sent_seq[] = {0x03, 0x00, 0x12, 0x34};



   uint8_t read_buffer[100];

   ISSI25LP064_read(0x1234, read_buffer, sizeof(read_buffer));



   for (size_t i = 0; i < sizeof(sent_seq); i++) {

      UnityAssertEqualNumber((UNITY_INT)((sent_seq[i])), (UNITY_INT)((ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i])), (

     ((void *)0)

     ), (UNITY_UINT)(78), UNITY_DISPLAY_STYLE_INT);

   }

}

void test_erase_one_sector(void) {

   uint8_t sent_seq[] = {0x05, 0x06, 0xd7, 0x00, 0x10, 0x00, 0x05};



   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_zero_ISSI25LP064_PORT_receive_spi;



   ISSI25LP064_erase(0x1000, 100);



   for (size_t i = 0; i < sizeof(sent_seq); i++) {

      UnityAssertEqualNumber((UNITY_INT)((sent_seq[i])), (UNITY_INT)((ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i])), (

     ((void *)0)

     ), (UNITY_UINT)(99), UNITY_DISPLAY_STYLE_INT);

   }

}

void test_erase_sector_busy(void) {

   uint8_t sent_seq[] = {0x05, 0x05, 0x05, 0x06, 0xd7, 0x00, 0x10, 0x00, 0x05, 0x05, 0x05};



   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_busy_one_time_ISSI25LP064_PORT_receive_spi;



   ISSI25LP064_erase(0x1000, 8193);



   for (size_t i = 0; i < sizeof(sent_seq); i++) {

      UnityAssertEqualNumber((UNITY_INT)((sent_seq[i])), (UNITY_INT)((ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i])), (

     ((void *)0)

     ), (UNITY_UINT)(125), UNITY_DISPLAY_STYLE_INT);

   }

}

void test_erase_several_sectors(void) {

   uint8_t sent_seq[] = {0x05, 0x06, 0xd7, 0x00, 0x10, 0x00, 0x05,

                         0x05, 0x06, 0xd7, 0x00, 0x20, 0x00, 0x05,

                         0x05, 0x06, 0xd7, 0x00, 0x30, 0x00, 0x05};



   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_zero_ISSI25LP064_PORT_receive_spi;



   ISSI25LP064_erase(0x1000, 8193);



   for (size_t i = 0; i < sizeof(sent_seq); i++) {

      UnityAssertEqualNumber((UNITY_INT)((sent_seq[i])), (UNITY_INT)((ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i])), (

     ((void *)0)

     ), (UNITY_UINT)(158), UNITY_DISPLAY_STYLE_INT);

   }

}

void test_program_one_page(void) {

   uint8_t sent_seq[] = {0x05, 0x06, 0x02, 0x00, 0x12, 0x00, 0x00, 0x01,

                         0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x05};



   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_zero_ISSI25LP064_PORT_receive_spi;





   uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   ISSI25LP064_write(0x1234, data, 10);



   for (size_t i = 0; i < sizeof(sent_seq); i++) {

      UnityAssertEqualNumber((UNITY_INT)((sent_seq[i])), (UNITY_INT)((ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i])), (

     ((void *)0)

     ), (UNITY_UINT)(181), UNITY_DISPLAY_STYLE_INT);

   }

}

void test_program_page_busy(void) {

   uint8_t sent_seq[] = {0x05, 0x05, 0x05, 0x06, 0x02, 0x00, 0x12, 0x00, 0x00,

                         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,

                         0x05, 0x05, 0x05};



   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_busy_one_time_ISSI25LP064_PORT_receive_spi;





   uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   ISSI25LP064_write(0x1234, data, 10);



   for (size_t i = 0; i < sizeof(sent_seq); i++) {

      UnityAssertEqualNumber((UNITY_INT)((sent_seq[i])), (UNITY_INT)((ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i])), (

     ((void *)0)

     ), (UNITY_UINT)(211), UNITY_DISPLAY_STYLE_INT);

   }

}
