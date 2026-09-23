#include "init.h"
#include "interrupt.h"

#define SERVO_MID_US (uint16_t)1500 // 1.5 ms = ~90 degrees


static void initialise_clocks() {
    enable_port_clock(A);
    enable_port_clock(B);
    enable_port_clock(C); 
}

static void initialise_inputs() {
    init_pin(GPIOA, 2, MODE_INPUT, PULL_DOWN); // new item PA2
    init_pin(GPIOA, 10, MODE_INPUT, PULL_DOWN); // step gate PA10
    init_pin(GPIOB, 5, MODE_INPUT, PULL_DOWN); // E-stop PB5
}

static void initialise_pwm() {
    init_pin(GPIOC, 7, MODE_ALT, NONE); // servo motor 1 -> PC7
    init_pin(GPIOB, 0, MODE_ALT, NONE); // servo motor 2 -> PB0
    enable_cap_com(GPIOC, 7, 2, 1, SERVO_MID_US); // enable cap com -> PC7
    enable_cap_com(GPIOB, 0, 3, 1, SERVO_MID_US); // enable cap com -> PB0
}

static void initialise_outputs() {
    init_pin(GPIOA, 9, MODE_OUTPUT, NONE); // status LED

    init_pin(GPIOA, 0, MODE_OUTPUT, NONE); // real colour: RGB LED -> red
    init_pin(GPIOA, 1, MODE_OUTPUT, NONE); // real colour: RGB LED -> green
    init_pin(GPIOA, 4, MODE_OUTPUT, NONE); // real colour: RGB LED -> blue

    init_pin(GPIOA, 3, MODE_OUTPUT, NONE); // sensed colour: RGB LED -> red 
    init_pin(GPIOA, 11, MODE_OUTPUT, NONE); // sensed colour: RGB LED -> green 
    init_pin(GPIOA, 8, MODE_OUTPUT, NONE); // sensed colour: RGB LED -> blue 
}

static void initialise_interrupts() {
    enable_interrupt(A, RISING, 2);
    enable_interrupt(A, RISING, 10);
    enable_interrupt(B, RISING, 5);
}


static void initialise_timer() {
    enable_timer(TIM3, 19999, 47);
}


void initialise_registers() {
    initialise_clocks();
    initialise_inputs();
    initialise_pwm();
    initialise_outputs();
    initialise_timer();
    initialise_interrupts();
}