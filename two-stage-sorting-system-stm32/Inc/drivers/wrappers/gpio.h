#pragma once

#include "stm32c031xx.h"
#include "stm32c0xx.h"
#include <stdint.h>


typedef enum {
    PIN_LOW = 0,
    PIN_HIGH = 1
} pin_state;

typedef enum {
    MODE_INPUT = 0,
    MODE_OUTPUT = 1,
    MODE_ALT = 2,
    MODE_ANALOG = 3
} pin_mode;

typedef enum {
    NONE,
    PULL_UP,
    PULL_DOWN,
    RESERVED
} pupdr_mode;

void pin_write(GPIO_TypeDef * port, uint8_t pin, pin_state state);
void toggle_output(GPIO_TypeDef * port, uint8_t pin);
uint8_t pin_read(GPIO_TypeDef * port, uint8_t pin);
void init_pin(GPIO_TypeDef * port, uint8_t pin, pin_mode mode, pupdr_mode p_mode);



