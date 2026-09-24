#pragma once

#include "reset_and_control_clock.h"
#include "interrupt.h"
#include "gpio.h"
#include "stm32c031xx.h"
#include "generate.h"
#include "timer.h"
#include "pwm.h"

// Initialise all registers.
void initialise_registers();