#pragma once
#include "gpio.h"
#include "stm32c031xx.h"

typedef enum {
    OFF = 0,
    RED,
    GREEN,
    BLUE
} item_colour_t;

item_colour_t apply_lfsr_fault(item_colour_t true_col); // Fig. 1 STAGE_0 fault logic
void generate_next_item(void); // Fig. 1 STAGE_0: truth + sensed colour onto RGB LEDs
void systick_init(void);