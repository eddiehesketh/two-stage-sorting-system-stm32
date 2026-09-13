#include "pwm_logic.h"

#define SERVO_MIN_US 500 // 1.0 ms = ~0 degrees
#define SERVO_MID_US 1500 // 1.5 ms = ~90 degrees
#define SERVO_MAX_US 2500

void object_detected_blue() {
    TIM3->CCR2 = SERVO_MID_US;
}

void object_detected_not_blue() {
    TIM3->CCR2 = SERVO_MAX_US;
}
