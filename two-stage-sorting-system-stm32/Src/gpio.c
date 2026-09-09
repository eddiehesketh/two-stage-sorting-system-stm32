#include "gpio.h"
#include <stdint.h>


static void set_pin_mode(GPIO_TypeDef * port, uint8_t pin, pin_mode mode) {
    uint8_t shift = pin * 2;
    port->MODER &= ~(3 << shift);
    port->MODER |= (mode << shift);
}

void pin_write(GPIO_TypeDef * port, uint8_t pin, pin_state state) {
    if (state == PIN_HIGH) {
        port->ODR |= (1 << pin);
    } else {
        port->ODR &= ~(1 << pin);
    }
}

void toggle_output(GPIO_TypeDef * port, uint8_t pin) {
    port->ODR ^= (1 << pin);
}


uint8_t pin_read(GPIO_TypeDef * port, uint8_t pin) {
    return (uint8_t)((port->IDR >> pin) & 1);
}

static void set_pupdr_mode(GPIO_TypeDef * port, uint8_t pin, pupdr_mode mode) {
    uint8_t shift = 2 * pin;
    port->PUPDR &= ~(3 << shift);
    port->PUPDR |= (mode << shift);
}

void init_pin(GPIO_TypeDef * port, uint8_t pin, pin_mode mode, pupdr_mode p_mode) {
    set_pin_mode(port, pin, mode);
    if (p_mode != NONE) {
        set_pupdr_mode(port, pin, p_mode);
    }
}



