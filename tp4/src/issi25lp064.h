#include <stdint.h>
#include <stddef.h>


typedef enum {
   issi25lp064_OK,
   issi25lp064_ERROR
} issi25lp064_status_t;


issi25lp064_status_t ISSI25LP064_init(void);

issi25lp064_status_t ISSI25LP064_read(uint32_t address, void *buffer, size_t length);

issi25lp064_status_t ISSI25LP064_erase(uint32_t address, size_t length);

issi25lp064_status_t ISSI25LP064_write(uint32_t address, const void *buffer, size_t length);
