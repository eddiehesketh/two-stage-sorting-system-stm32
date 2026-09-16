#include "interrupt.h"
#include "stm32c031xx.h"

static void configure_pin(gpio_port port, uint8_t pin) {
    uint8_t reg = pin / 4;
    uint8_t index = (pin % 4) * 8;
    EXTI->EXTICR[reg] &= ~(0xFF << index);
    EXTI->EXTICR[reg] |= (port << index);
}

static void enable_nvic_inform(uint8_t pin) {
    EXTI->IMR1 &= ~(1 << pin);
    EXTI->IMR1 |= (1 << pin);
}

static void enable_rising_edge_interrupt(uint8_t pin) {
    EXTI->RTSR1 &= ~(1 << pin);
    EXTI->RTSR1 |= (1 << pin);
}

static void enable_falling_edge_interrupt(uint8_t pin) {
    EXTI->FTSR1 &= ~(1 << pin);
    EXTI->FTSR1 |= (1 << pin);
}

static void monitor_rising_edge(uint8_t pin) {
    EXTI->RPR1 &= ~(1 << pin);
    EXTI->RPR1 |= (1 << pin);
}
static void monitor_falling_edge(uint8_t pin) {
    EXTI->FPR1 &= ~(1 << pin);
    EXTI->FPR1 |= (1 << pin);
}

// up to main.c to define the handler
void enable_interrupt_priority(gpio_port port, edge_trigger_mode mode, uint8_t pin, uint8_t priority) {
    configure_pin(port, pin);
    enable_nvic_inform(pin);
    
    if (mode == FALLING || mode == BOTH) {
        enable_falling_edge_interrupt(pin);
        monitor_falling_edge(pin);
    }

    if (mode == RISING || mode == BOTH) {
        enable_rising_edge_interrupt(pin);
        monitor_rising_edge(pin);
    }

    IRQn_Type interrupt_type = pin < 2 ? EXTI0_1_IRQn : (pin < 4 ? EXTI2_3_IRQn : EXTI4_15_IRQn);

    NVIC_SetPriority(interrupt_type, priority);
    NVIC_EnableIRQ(interrupt_type);
}
void enable_interrupt(gpio_port port, edge_trigger_mode mode, uint8_t pin) {
    configure_pin(port, pin);
    enable_nvic_inform(pin);
    
    if (mode == FALLING || mode == BOTH) {
        enable_falling_edge_interrupt(pin);
        monitor_falling_edge(pin);
    }

    if (mode == RISING || mode == BOTH) {
        enable_rising_edge_interrupt(pin);
        monitor_rising_edge(pin);
    }

    IRQn_Type interrupt_type = pin < 2 ? EXTI0_1_IRQn : (pin < 4 ? EXTI2_3_IRQn : EXTI4_15_IRQn);

    NVIC_EnableIRQ(interrupt_type);
}

