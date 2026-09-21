#pragma once

#include "stm32c0xx.h"
#include "timer.h"


typedef enum {
    CCM_OUTPUT = 0,
    CCM_INPUT_STRAIGHT = 1,
    CCM_INPUT_CROSS = 2
} cc_mode;

typedef enum {
    PWM_MODE_1 = 0x6,
    PWM_MODE_2 = 0x7
} pwm_mode;

void enable_cap_com(GPIO_TypeDef * port, uint8_t pin, uint8_t channel, uint8_t af, uint16_t duty_us);

