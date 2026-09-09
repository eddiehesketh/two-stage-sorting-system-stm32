#include "timer.h"
#include "stm32c031xx.h"


static IRQn_Type enable_rcc_and_get_type(TIM_TypeDef * tim) {
    if (tim == TIM1) {
        RCC->APBENR2 &= ~(RCC_APBENR2_TIM1EN_Msk);
        RCC->APBENR2 |= (RCC_APBENR2_TIM1EN);
        return TIM1_CC_IRQn;
    } else if (tim == TIM3) {
        RCC->APBENR1 &= ~(RCC_APBENR1_TIM3EN_Msk);
        RCC->APBENR1 |= (RCC_APBENR1_TIM3EN);
        return TIM3_IRQn;
    } else if (tim == TIM14) {
        RCC->APBENR2 &= ~(RCC_APBENR2_TIM14EN_Msk);
        RCC->APBENR2 |= (RCC_APBENR2_TIM14EN);
        return TIM14_IRQn;
    } else if (tim == TIM16) {
        RCC->APBENR2 &= ~(RCC_APBENR2_TIM16EN_Msk);
        RCC->APBENR2 |= (RCC_APBENR2_TIM16EN);
        return TIM16_IRQn;
    } else if (tim == TIM17) {
        RCC->APBENR2 &= ~(RCC_APBENR2_TIM17EN_Msk);
        RCC->APBENR2 |= (RCC_APBENR2_TIM17EN);
        return TIM17_IRQn;
    } else {
        return TIM1_CC_IRQn; // base case shouldnt happen
    }
}



void enable_timer(TIM_TypeDef * tim, uint16_t arr, uint16_t psc) {
    IRQn_Type nvic_type = enable_rcc_and_get_type(tim);

    tim->PSC = psc;
    tim->ARR = arr;
    tim->DIER |= (1 << 0);

    NVIC_EnableIRQ(nvic_type);

    tim->CR1 |= (1 << 0);
}
