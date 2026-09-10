#pragma once
#include "gpio.h"
#include "stm32c031xx.h"

typedef enum {
    OFF = 0,
    RED,
    GREEN,
    BLUE
} item_colour_t;

item_colour_t apply_lfsr_fault(item_colour_t true_col);
void generate_next_item(void);
void systick_init(void);