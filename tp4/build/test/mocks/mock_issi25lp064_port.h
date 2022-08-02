#ifndef mock_issi25lp064_port_H
#define mock_issi25lp064_port_H

#include "fff.h"
#include "fff_unity_helper.h"
#include "issi25lp064_port.h"


DECLARE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_init_pins);
DECLARE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_init_spi);
DECLARE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_clear_ce);
DECLARE_FAKE_VOID_FUNC1(ISSI25LP064_PORT_transmit_spi, const uint8_t);
DECLARE_FAKE_VOID_FUNC1(ISSI25LP064_PORT_receive_spi, uint8_t*);
DECLARE_FAKE_VOID_FUNC0(ISSI25LP064_PORT_set_ce);

void mock_issi25lp064_port_Init(void);
void mock_issi25lp064_port_Verify(void);
void mock_issi25lp064_port_Destroy(void);

#endif // mock_issi25lp064_port_H
