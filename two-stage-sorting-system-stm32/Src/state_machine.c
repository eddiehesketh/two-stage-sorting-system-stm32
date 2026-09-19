#include "state_machine.h"
#include "stm32c031xx.h"

#define SERVO_MIN_US 500 // 1.0 ms = ~0 degrees
#define SERVO_45_DEG_ACW 1000
#define SERVO_MID_US 1500 // 1.5 ms = ~90 degrees
#define SERVO_45_DEG_CW 1500
#define SERVO_MAX_US 2500

static sorting_stage current_stage = NONE;
static sorting_stage next_stage = NONE;

void sorting_stage_fsm() {
    current_stage = next_stage;
    switch (current_stage) {
        case STAGE_0:
            next_stage = STAGE_1;
        case STAGE_1:
            if (pin_read(GPIOA, 8)) {
                TIM3->CCR2 = SERVO_45_DEG_ACW;
            } else if (pin_read(GPIOA, 11) || pin_read(GPIOA, 3)) {
                TIM3->CCR2 = SERVO_45_DEG_CW;
            } else {
                TIM3->CCR2 = SERVO_MID_US;
            }
            next_stage = STAGE_2;
        case STAGE_2:
            if (pin_read(GPIOA, 11)) {
                TIM3->CCR3 = SERVO_45_DEG_ACW;
            } else if (pin_read(GPIOA, 3)) {
                TIM3->CCR3 = SERVO_45_DEG_CW;
            } else {
                TIM3->CCR3 = SERVO_MID_US;
            }
            next_stage = STAGE_0;
    }
}