#pragma once

#include "stm32c0xx.h"

// NVIC priority left at reset (0). Must provide TIMx_IRQHandler
// Counter clock is fCK / (psc + 1); arr is the period (PSC/ARR).
void enable_timer(TIM_TypeDef * tim, uint16_t arr, uint16_t psc);
