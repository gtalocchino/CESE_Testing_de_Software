#include "unity.h"
#include "issi25lp064.h"
#include "mock_issi25lp064_port.h"

#include <stdbool.h>


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

/* The initialization function must send the command 0x9f over SPI. */
void test_ISSI25LP064_init_sent_command(void) {
   ISSI25LP064_init();

   TEST_ASSERT_EQUAL_UINT8(ISSI25LP064_PORT_transmit_spi_fake.arg0_val, 0x9f);
}

/*
 * The initialization function must return "issi25lp064_OK" if the 
 * memory responds with ID 0x9d.
 */
void test_initialization_read_correct_id(void) {
   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_correct_id_ISSI25LP064_PORT_receive_spi;

   TEST_ASSERT_EQUAL(issi25lp064_OK, ISSI25LP064_init());
}

/*
 * The initialization function should must "issi25lp064_ERROR" if the 
 * memory responds with an ID different than 0x9d.
 */
void test_initialization_read_wrong_id(void) {
   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_wrong_id_ISSI25LP064_PORT_receive_spi;

   TEST_ASSERT_EQUAL(issi25lp064_ERROR, ISSI25LP064_init());
}

/**
 * A read to address 0x1234 should send the command <0x03001234> to memory
 * over SPI.
 */
void test_read(void) {
   uint8_t sent_seq[] = {0x03, 0x00, 0x12, 0x34};
   
   uint8_t read_buffer[100];
   ISSI25LP064_read(0x1234, read_buffer, sizeof(read_buffer));

   for (size_t i = 0; i < sizeof(sent_seq); i++) {
      TEST_ASSERT_EQUAL(sent_seq[i], ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i]);
   }
}

/**
 * If 100 bytes are deleted from address 0x1000 (affects one sector) the following 
 * commands must be sent to memory over SPI.
 * 
 * 1) read status                   : 0x05
 * 2) write enable                  : 0x06
 * 3) erase sector at address 0x1000: 0xd7001000
 * 4) read status                   : 0x05
 */
void test_erase_one_sector(void) {
   uint8_t sent_seq[] = {0x05, 0x06, 0xd7, 0x00, 0x10, 0x00, 0x05};

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_zero_ISSI25LP064_PORT_receive_spi;

   ISSI25LP064_erase(0x1000, 100);

   for (size_t i = 0; i < sizeof(sent_seq); i++) {
      TEST_ASSERT_EQUAL(sent_seq[i], ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i]);
   }
}

/** 
 *  Before sending the delete command to the memory, it must be verified that the memory 
 *  is not busy doing another operation. If the memory is busy, its status should be 
 *  asked until it answers that it is not busy.
 * 
 *  1) read status                   : 0x05 (receive busy state)
 *  1) read status                   : 0x05 (receive busy state)
 *  3) read status                   : 0x05
 *  2) write enable                  : 0x06
 *  3) erase sector at address 0x1000: 0xd7001000
 *  1) read status                   : 0x05 (receive busy state)
 *  1) read status                   : 0x05 (receive busy state)
 *  3) read status                   : 0x05
 */
void test_erase_sector_busy(void) {
   uint8_t sent_seq[] = {0x05, 0x05, 0x05, 0x06, 0xd7, 0x00, 0x10, 0x00, 0x05, 0x05, 0x05};

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_busy_one_time_ISSI25LP064_PORT_receive_spi;

   ISSI25LP064_erase(0x1000, 8193);

   for (size_t i = 0; i < sizeof(sent_seq); i++) {
      TEST_ASSERT_EQUAL(sent_seq[i], ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i]);
   }
}

/**
 * If 8193 bytes (affects three sectors) are deleted from address 0x1000 the following 
 * commands must be sent to memory over SPI.
 * 
 *  1) read status                   : 0x05
 *  2) write enable                  : 0x06
 *  3) erase sector at address 0x1000: 0xd7001000
 *  5) read status                   : 0x05
 * 
 *  6) read status                   : 0x05
 *  7) write enable                  : 0x06
 *  8) erase sector at address 0x2000: 0xd7002000
 *  9) read status                   : 0x05
 * 
 * 10) read status                   : 0x05
 * 11) write enable                  : 0x06
 * 12) erase sector at address 0x3000: 0xd7003000
 * 13) read status                   : 0x05
 */
void test_erase_several_sectors(void) {
   uint8_t sent_seq[] = {0x05, 0x06, 0xd7, 0x00, 0x10, 0x00, 0x05,
                         0x05, 0x06, 0xd7, 0x00, 0x20, 0x00, 0x05,
                         0x05, 0x06, 0xd7, 0x00, 0x30, 0x00, 0x05};

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_zero_ISSI25LP064_PORT_receive_spi;

   ISSI25LP064_erase(0x1000, 8193);

   for (size_t i = 0; i < sizeof(sent_seq); i++) {
      TEST_ASSERT_EQUAL(sent_seq[i], ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i]);
   }
}

/**
 *  A write of 10 bytes to address 0x1234 must send the following commands to memory.
 *  
 *  1) read status                 : 0x05
 *  2) write enable                : 0x06
 *  3) write page at address 0x1200: 0x02001200 + data
 *  4) read status                 : 0x05
 */
void test_program_one_page(void) {
   uint8_t sent_seq[] = {0x05, 0x06, 0x02, 0x00, 0x12, 0x00, 0x00, 0x01, 
                         0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x05};

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_zero_ISSI25LP064_PORT_receive_spi;

   /* Array of data to write at address 0x1200. */
   uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   ISSI25LP064_write(0x1234, data, 10);

   for (size_t i = 0; i < sizeof(sent_seq); i++) {
      TEST_ASSERT_EQUAL(sent_seq[i], ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i]);
   }
}

/**
 *  Before sending the write page command to the memory, it must be verified that the memory 
 *  is not busy doing another operation. If the memory is busy, its status should be 
 *  asked until it answers that it is not busy.
 *  
 *  1) read status                 : 0x05 (receive busy state)
 *  1) read status                 : 0x05 (receive busy state)
 *  1) read status                 : 0x05
 *  2) write enable                : 0x06
 *  3) write page at address 0x1200: 0x02001200 + data
 *  1) read status                 : 0x05 (receive busy state)
 *  1) read status                 : 0x05 (receive busy state)
 *  4) read status                 : 0x05
 */
void test_program_page_busy(void) {
   uint8_t sent_seq[] = {0x05, 0x05, 0x05, 0x06, 0x02, 0x00, 0x12, 0x00, 0x00, 
                         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
                         0x05, 0x05, 0x05};

   ISSI25LP064_PORT_receive_spi_fake.custom_fake = fake_receive_busy_one_time_ISSI25LP064_PORT_receive_spi;

   /* Array of data to write. */
   uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   ISSI25LP064_write(0x1234, data, 10);

   for (size_t i = 0; i < sizeof(sent_seq); i++) {
      TEST_ASSERT_EQUAL(sent_seq[i], ISSI25LP064_PORT_transmit_spi_fake.arg0_history[i]);
   }
}
