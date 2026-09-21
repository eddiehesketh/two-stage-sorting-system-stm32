#include "interrupt.h"
#include "stm32c031xx.h"

// EXTICR: 4 pins per register, 8-bit port field (A=0, B=1, ...).
static void configure_pin(gpio_port port, uint8_t pin) {
    uint8_t reg = pin / 4;
    uint8_t index = (pin % 4) * 8;
    EXTI->EXTICR[reg] &= ~(0xFF << index);
    EXTI->EXTICR[reg] |= (port << index);
}

static void enable_nvic_inform(uint8_t pin) {
    EXTI->IMR1 &= ~(1 << pin);
    EXTI->IMR1 |= (1 << pin); // EXTI_IMR1: unmask this line to NVIC
}

static void enable_rising_edge_interrupt(uint8_t pin) {
    EXTI->RTSR1 &= ~(1 << pin);
    EXTI->RTSR1 |= (1 << pin); // EXTI_RTSR1
}

static void enable_falling_edge_interrupt(uint8_t pin) {
    EXTI->FTSR1 &= ~(1 << pin);
    EXTI->FTSR1 |= (1 << pin); // EXTI_FTSR1
}

// RPR1 is rc_w1: write 1 to clear a stale rising pending bit.
static void monitor_rising_edge(uint8_t pin) {
    EXTI->RPR1 &= ~(1 << pin);
    EXTI->RPR1 |= (1 << pin);
}

// FPR1 is rc_w1: write 1 to clear a stale falling pending bit.
static void monitor_falling_edge(uint8_t pin) {
    EXTI->FPR1 &= ~(1 << pin);
    EXTI->FPR1 |= (1 << pin);
}

// App must provide EXTI0_1 / EXTI2_3 / EXTI4_15_IRQHandler.
// priority is 0..3 (lower number preempts).
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

    // STM32C0: pins 0-1, 2-3, and 4-15 share one NVIC vector each.
    IRQn_Type interrupt_type = pin < 2 ? EXTI0_1_IRQn : (pin < 4 ? EXTI2_3_IRQn : EXTI4_15_IRQn);

    NVIC_SetPriority(interrupt_type, priority);
    NVIC_EnableIRQ(interrupt_type);
}

// App must provide EXTI0_1 / EXTI2_3 / EXTI4_15_IRQHandler.
// NVIC priority left at reset (0).
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

    // STM32C0: pins 0-1, 2-3, and 4-15 share one NVIC vector each.
    IRQn_Type interrupt_type = pin < 2 ? EXTI0_1_IRQn : (pin < 4 ? EXTI2_3_IRQn : EXTI4_15_IRQn);

    NVIC_EnableIRQ(interrupt_type);
}
