#include <string.h>
#include "fff.h"
#include "mock_issi25lp064_port.h"

DEFINE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_init_pins);
DEFINE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_init_spi);
DEFINE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_clear_ce);
DEFINE_FAKE_VOID_FUNC1(ISSI25LP064_PORT_transmit_spi, const uint8_t);
DEFINE_FAKE_VOID_FUNC1(ISSI25LP064_PORT_receive_spi, uint8_t*);
DEFINE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_set_ce);

void mock_issi25lp064_port_Init(void)
{
    FFF_RESET_HISTORY();
    RESET_FAKE(ISSI25LP064_PORT_init_pins)
    RESET_FAKE(ISSI25LP064_PORT_init_spi)
    RESET_FAKE(ISSI25LP064_PORT_clear_ce)
    RESET_FAKE(ISSI25LP064_PORT_transmit_spi)
    RESET_FAKE(ISSI25LP064_PORT_receive_spi)
    RESET_FAKE(ISSI25LP064_PORT_set_ce)
}
void mock_issi25lp064_port_Verify(void)
{
}
void mock_issi25lp064_port_Destroy(void)
{
}
