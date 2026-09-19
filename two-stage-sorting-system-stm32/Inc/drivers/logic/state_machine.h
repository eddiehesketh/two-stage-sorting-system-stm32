#pragma once

#include "gpio.h"

typedef enum {
    STAGE_0,
    STAGE_1,
    STAGE_2
} sorting_stage;

void sorting_stage_fsm();