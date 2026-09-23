#include "fsm.h"

#define SERVO_45_DEG_ACW (uint16_t)1000 // 1.0 ms, ~45 degrees
#define SERVO_90_DEG (uint16_t)1500 // 1.5 ms, ~90 degrees
#define SERVO_45_DEG_CW (uint16_t)2000 // 2.0 ms, ~135 degrees

sorting_stage current_stage = STAGE_0; // E-stop EXTI may force STAGE_3
uint16_t servo_1_pos = SERVO_90_DEG; // TIM3 CH2 pulse, microseconds
uint16_t servo_2_pos = SERVO_90_DEG; // TIM3 CH3 pulse, microseconds

// One stage per call from EXTI (PA2 new item, PA10 step gate, PB5 E-stop).
void sorting_stage_fsm() {
    switch (current_stage) {
        case STAGE_0:
            generate_next_item();
            current_stage = STAGE_1;
            break;
        case STAGE_1:
            if (pin_read(GPIOA, 8)) { // sensed blue
                servo_1_pos = SERVO_45_DEG_ACW;
            } else if (pin_read(GPIOA, 11) || pin_read(GPIOA, 3)) { // sensed green or red
                servo_1_pos = SERVO_45_DEG_CW;
            } else {
                servo_1_pos = SERVO_90_DEG;
            }
            current_stage = STAGE_2;
            break;
        case STAGE_2:
            if (pin_read(GPIOA, 11)) { // sensed green
                servo_2_pos = SERVO_45_DEG_ACW;
            } else if (pin_read(GPIOA, 3)) { // sensed red
                servo_2_pos = SERVO_45_DEG_CW;
            } else {
                servo_2_pos = SERVO_90_DEG;
            }
            current_stage = STAGE_3;
            break;
        case STAGE_3:
            servo_1_pos = SERVO_90_DEG;
            servo_2_pos = SERVO_90_DEG;
            pin_write(GPIOA, 9, PIN_LOW); // status LED
            pin_write(GPIOA, 3, PIN_LOW); // sensed red
            pin_write(GPIOA, 11, PIN_LOW); // sensed green
            pin_write(GPIOA, 8, PIN_LOW); // sensed blue
            pin_write(GPIOA, 0, PIN_LOW); // truth red
            pin_write(GPIOA, 1, PIN_LOW); // truth green
            pin_write(GPIOA, 4, PIN_LOW); // truth blue
            current_stage = STAGE_0;
            break;
        default:
            break;
    }
}
