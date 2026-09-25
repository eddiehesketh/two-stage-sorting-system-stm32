#pragma once
#include "gpio.h"
#include "generate.h"
#include "stm32c0xx.h"

typedef enum {
    STAGE_0, // new item: roll colour onto the RGB LEDs
    STAGE_1, // first gate from sensed blue vs green/red
    STAGE_2, // second gate from sensed green vs red
    STAGE_3, // park servos, clear LEDs, return to STAGE_0
    STAGE_4  // fault LED on, park servos, cant leave this state
} sorting_stage;

extern volatile sorting_stage current_stage; // E-Stop EXTI may force STAGE_4
extern volatile uint16_t servo_1_pos; // TIM3 CH2 pulse, microseconds
extern volatile uint16_t servo_2_pos; // TIM3 CH3 pulse, microseconds

// One stage per call from EXTI (PA2 new item, PA10 step gate, PB5 E-stop)
// Servo CCR values are applied in TIM3_IRQHandler, not here
void sorting_stage_fsm();
