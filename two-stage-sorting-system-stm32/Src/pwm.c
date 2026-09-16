#include "pwm.h"
#include "gpio.h"
#include "stm32c031xx.h"
#include "timer.h"

static void set_alt_func(GPIO_TypeDef * port, uint8_t pin) {
    uint8_t shift = 2 * pin;
    port->MODER &= ~(3 << shift);
    port->MODER |= (MODE_ALT << shift);
}

static void set_af_alt_func(GPIO_TypeDef * port, uint8_t pin, uint8_t af) {
    uint8_t idx = pin / 8;
    uint8_t shift = (pin % 8) * 4;

    port->AFR[idx] &= ~(0xF << shift);
    port->AFR[idx] |= (af << shift);
}

static void set_ccmr_register_tim3(uint8_t channel, uint16_t duty_us, cc_mode cc, pwm_mode pwm) {
    switch (channel) {
        case 1:
            TIM3->CCMR1 &= ~(3 << 0 | 7 << 4);
            TIM3->CCMR1 |= (cc << 0 | PWM_MODE_1 << 4);
            TIM3->CCR1 = duty_us;
            TIM3->CCER |= (1 << 0);
            break;
        case 2:
            TIM3->CCMR1 &= ~(3 << 8 | 7 << 12);
            TIM3->CCMR1 |= (cc << 8 | PWM_MODE_1 << 12);
            TIM3->CCR2 = duty_us;
            TIM3->CCER |= (1 << 4);
            break;      
        case 3:
            TIM3->CCMR2 &= ~(3 << 0 | 7 << 4);
            TIM3->CCMR2 |= (cc << 0 | PWM_MODE_1 << 4);
            TIM3->CCR3 = duty_us;
            TIM3->CCER |= (1 << 8);
            break; 
        case 4:
            TIM3->CCMR2 &= ~(3 << 8 | 7 << 12);
            TIM3->CCMR2 |= (cc << 8 | PWM_MODE_1 << 12);
            TIM3->CCR4 = duty_us;
            TIM3->CCER |= (1 << 12);
            break;
        default:
            break;
    }   
}

// static void enable_timer_cap_com(uint8_t channel) {
//     TIM3->DIER |= (1 << channel);
//     TIM3->SR |= (1 << channel);
// }

void enable_cap_com(GPIO_TypeDef * port, uint8_t pin, uint8_t channel, uint8_t af, uint16_t duty_us) {
    // enable_timer(TIM3, arr, psc, 2);
    set_alt_func(port, pin);
    set_af_alt_func(port, pin, af);
    set_ccmr_register_tim3(channel, duty_us, CCM_OUTPUT, PWM_MODE_1);
    // enable_timer_cap_com(channel);
    TIM3->EGR |= (1 << 0);
}