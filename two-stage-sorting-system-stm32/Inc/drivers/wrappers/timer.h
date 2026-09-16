#pragma once

#include "stm32c0xx.h"

void enable_timer(TIM_TypeDef * tim, uint16_t arr, uint16_t psc);