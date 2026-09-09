#pragma once

#include "stm32c0xx.h"
#include "reset_and_control_clock.h"

typedef enum {
    FALLING,
    RISING,
    BOTH
} edge_trigger_mode;

void enable_interrupt(gpio_port port, edge_trigger_mode mode, uint8_t pin, uint8_t priority);


