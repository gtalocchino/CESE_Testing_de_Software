#include "issi25lp064.h"
#include "issi25lp064_port.h"

#include <stdbool.h>

#define FLASH_PAGE_SIZE       (256u)
#define FLASH_SECTOR_SIZE     (4096u)
#define GET_SECTOR_ADDRESS(x) (((x) / FLASH_SECTOR_SIZE) * FLASH_SECTOR_SIZE)
#define GET_PAGE_ADDRESS(x)   (((x) / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE)
#define BUSY_OFFSET           (1u)

#define ISSI_ID               (0x9d)

#define CMD_READ_STATUS       (0x05)
#define CMD_READ_NORMAL       (0x03)
#define CMD_ERASE_SECTOR      (0xd7)
#define CMD_PAGE_PROGRAM      (0x02)
#define CMD_WRITE_ENABLE      (0x06)
#define CMD_READ_ID           (0x9f)


issi25lp064_status_t erase_sector(uint32_t address);
issi25lp064_status_t page_program(uint32_t address, const uint8_t *data, size_t length);
void write_enable(void);
void wait_busy(void);


issi25lp064_status_t ISSI25LP064_init(void) {
   ISSI25LP064_PORT_init_pins();
   ISSI25LP064_PORT_init_spi();

   uint8_t instruction = CMD_READ_ID;
   uint8_t read_value = 0;

   ISSI25LP064_PORT_clear_ce();
   ISSI25LP064_PORT_transmit_spi(instruction);
   ISSI25LP064_PORT_receive_spi(&read_value);
   ISSI25LP064_PORT_set_ce();

   if (read_value != ISSI_ID) {
      return issi25lp064_ERROR;
   }

   return issi25lp064_OK;
}

issi25lp064_status_t ISSI25LP064_read(uint32_t address, void *buffer, size_t length) {
   uint8_t *data = (uint8_t *) data;
   uint8_t instruction[4] = {
      CMD_READ_NORMAL, 
      (address & 0xff0000) >> 16u,
      (address & 0xff00  ) >> 8u,
      (address & 0xff    ) >> 0u,
   };
   
   ISSI25LP064_PORT_clear_ce();

   for (size_t i = 0; i < sizeof(instruction); i++) {
      ISSI25LP064_PORT_transmit_spi(instruction[i]);
   }

   for (size_t i = 0; i < length; i++) {
      ISSI25LP064_PORT_receive_spi(data++);
   }

   ISSI25LP064_PORT_set_ce();

   return issi25lp064_OK;
}

issi25lp064_status_t ISSI25LP064_erase(uint32_t address, size_t length) {
	const uint32_t sector_address_start = GET_SECTOR_ADDRESS(address);
	const uint32_t sector_address_end = GET_SECTOR_ADDRESS(address + length - 1u);
	const uint32_t sector_count = 1u + (sector_address_end - sector_address_start) / FLASH_SECTOR_SIZE;

	uint32_t start_address = sector_address_start;

	for (uint32_t i = 0; i < sector_count; i++) {
		issi25lp064_status_t status = erase_sector(start_address);

      if (status != issi25lp064_OK) {
         return status;
      }

		start_address += FLASH_SECTOR_SIZE;
	}

   return issi25lp064_OK;
}

issi25lp064_status_t ISSI25LP064_write(uint32_t address, const void *buffer, size_t length) {
   const uint32_t page_address_start = GET_PAGE_ADDRESS(address);
	const uint32_t page_address_end = GET_PAGE_ADDRESS(address + length - 1u);
	const uint32_t page_count = 1u + (page_address_end - page_address_start) / FLASH_PAGE_SIZE;

	uint32_t start_address = page_address_start;
   const uint8_t *data = (const uint8_t *) buffer;

	for (uint32_t i = 0; i < page_count; i++) {
		issi25lp064_status_t status = page_program(start_address, data, length);

      if (status != issi25lp064_OK) {
         return status;
      }

		buffer += FLASH_PAGE_SIZE;
		start_address += FLASH_PAGE_SIZE;
      length  = (length < FLASH_PAGE_SIZE) ? 0 : length - FLASH_PAGE_SIZE;
	}

   return issi25lp064_OK;
}

void wait_busy(void) {
   uint8_t instruction = CMD_READ_STATUS;
   uint8_t read_value = 0;
   bool is_busy = true;

   do {
      ISSI25LP064_PORT_clear_ce();
      ISSI25LP064_PORT_transmit_spi(instruction);
      ISSI25LP064_PORT_receive_spi(&read_value);
      ISSI25LP064_PORT_set_ce();

      if (read_value & (1u << BUSY_OFFSET)) {
			is_busy = true;
		} else {
         is_busy = false;
		}
   } while (is_busy);
}

void write_enable() {
   uint8_t instruction = CMD_WRITE_ENABLE;

   ISSI25LP064_PORT_clear_ce();
   ISSI25LP064_PORT_transmit_spi(instruction);
   ISSI25LP064_PORT_set_ce();
}

issi25lp064_status_t page_program(uint32_t address, const uint8_t *data, size_t length) {
   wait_busy();

   write_enable();

   uint8_t instruction[4] = {
      CMD_PAGE_PROGRAM, 
      (address & 0xff0000) >> 16u,
      (address & 0xff00  ) >> 8u,
      (address & 0xff    ) >> 0u,
   };

   ISSI25LP064_PORT_clear_ce();

   for (size_t i = 0; i < sizeof(instruction); i++) {
      ISSI25LP064_PORT_transmit_spi(instruction[i]);
   }

   for (size_t i = 0; i < length; i++) {
      ISSI25LP064_PORT_transmit_spi(data[i]);
   }

   ISSI25LP064_PORT_set_ce();

   wait_busy();

   return issi25lp064_OK;
}

issi25lp064_status_t erase_sector(uint32_t address) {
   wait_busy();

   write_enable();

   uint8_t instruction[4] = {
      CMD_ERASE_SECTOR, 
      (address & 0xff0000) >> 16u,
      (address & 0xff00  ) >> 8u,
      (address & 0xff    ) >> 0u,
   };

   ISSI25LP064_PORT_clear_ce();
   
   for (size_t i = 0; i < sizeof(instruction); i++) {
      ISSI25LP064_PORT_transmit_spi(instruction[i]);
   }

   ISSI25LP064_PORT_set_ce();

   wait_busy();

   return issi25lp064_OK;
}
