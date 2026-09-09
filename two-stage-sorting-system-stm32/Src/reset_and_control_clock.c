#include "reset_and_control_clock.h"

void enable_port_clock(gpio_port port) {
    RCC->IOPENR |= (1 << port);
}
