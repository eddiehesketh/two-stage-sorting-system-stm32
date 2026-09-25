#include "fsm.h"

// Servo position macros - Initialisation block of the main flow chart
#define SERVO_45_DEG_ACW (uint16_t)1000 // 1.0 ms, ~45 degrees
#define SERVO_90_DEG (uint16_t)1500 // 1.5 ms, ~90 degrees
#define SERVO_45_DEG_CW (uint16_t)2000 // 2.0 ms, ~135 degrees

// Init globals — Initialisation block of the main flow chart
sorting_stage current_stage = STAGE_0;
uint16_t servo_1_pos = SERVO_90_DEG; // TIM3 CH2 pulse, microseconds
uint16_t servo_2_pos = SERVO_90_DEG; // TIM3 CH3 pulse, microseconds

// Fig. 1: sorting_stage_fsm() — one stage per EXTI call (PA2 / PA10 / PB5).
// Gate CCR values are applied later in TIM3_IRQHandler (main flow chart).
void sorting_stage_fsm() {
    switch (current_stage) {
        case STAGE_0: // Fig. 1: current_stage == STAGE_0?
            generate_next_item(); // call provided generate_next_item(); sets Real + Sensed RGB
            current_stage = STAGE_1;
            break;
        case STAGE_1: // Fig. 1: current_stage == STAGE_1?
            // Read sensed-colour LEDs: PA8, PA11, PA3
            if (pin_read(GPIOA, 8)) { // PA8 high? (sensed blue)
                servo_1_pos = SERVO_45_DEG_ACW;
            } else if (pin_read(GPIOA, 11) || pin_read(GPIOA, 3)) { // PA11 or PA3 high? (sensed green/red)
                servo_1_pos = SERVO_45_DEG_CW;
            } else {
                servo_1_pos = SERVO_90_DEG;
            }
            current_stage = STAGE_2;
            break;
        case STAGE_2: // Fig. 1: current_stage == STAGE_2?
            // Read sensed-colour LEDs: PA11, PA3
            if (pin_read(GPIOA, 11)) { // PA11 high? (sensed green)
                servo_2_pos = SERVO_45_DEG_ACW;
            } else if (pin_read(GPIOA, 3)) { // PA3 high? (sensed red)
                servo_2_pos = SERVO_45_DEG_CW;
            } else {
                servo_2_pos = SERVO_90_DEG;
            }
            current_stage = STAGE_3;
            break;
        case STAGE_3: // Fig. 1: current_stage == STAGE_3?
            servo_1_pos = SERVO_90_DEG; // park gates
            servo_2_pos = SERVO_90_DEG;
            pin_write(GPIOA, 9, PIN_LOW);  // clear LEDs: PA9 Fault LED
            pin_write(GPIOA, 3, PIN_LOW);  // PA3 sensed red
            pin_write(GPIOA, 11, PIN_LOW); // PA11 sensed green
            pin_write(GPIOA, 8, PIN_LOW);  // PA8 sensed blue
            pin_write(GPIOA, 0, PIN_LOW);  // PA0 truth red
            pin_write(GPIOA, 1, PIN_LOW);  // PA1 truth green
            pin_write(GPIOA, 4, PIN_LOW);  // PA4 truth blue
            current_stage = STAGE_0;
            break;
        case STAGE_4: // Fig. 1: current_stage == STAGE_4?
            servo_1_pos = SERVO_90_DEG; // park gates
            servo_2_pos = SERVO_90_DEG;
            // do not change current_stage (locked in STAGE_4)
            break;
        default:
            break;
    }
}
