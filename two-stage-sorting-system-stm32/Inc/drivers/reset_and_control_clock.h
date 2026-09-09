#pragma once
#include "stm32c0xx.h"

typedef enum {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    F = 5
} gpio_port;

void enable_port_clock(gpio_port port); // RCC_IOPENR


