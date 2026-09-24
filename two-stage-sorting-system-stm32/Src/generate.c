#include "generate.h"
#include "stm32c031xx.h"

volatile uint32_t lfsr_state = 0xA5A5A5A5;


// Fig. 1 STAGE_0: Sensed RGB LEDs (PA3/PA11/PA8) = sensor_colour
static void set_sensor_rgb(item_colour_t sensor_colour) {
    pin_write(GPIOA, 3, PIN_LOW);
    pin_write(GPIOA, 11, PIN_LOW);
    pin_write(GPIOA, 8, PIN_LOW);

    if (sensor_colour == RED) {
        pin_write(GPIOA, 3, PIN_HIGH);
    } else if (sensor_colour == GREEN) {
        pin_write(GPIOA, 11, PIN_HIGH);
    } else if (sensor_colour == BLUE) {
        pin_write(GPIOA, 8, PIN_HIGH);
    }
}

// Fig. 1 STAGE_0: Real RGB LEDs (PA0/PA1/PA4) = truth_colour
static void set_truth_rgb(item_colour_t truth_colour) {
    pin_write(GPIOA, 0, PIN_LOW);
    pin_write(GPIOA, 1, PIN_LOW);
    pin_write(GPIOA, 4, PIN_LOW);

    if (truth_colour == RED) {
        pin_write(GPIOA, 0, PIN_HIGH);
    } else if (truth_colour == GREEN) {
        pin_write(GPIOA, 1, PIN_HIGH);
    } else if (truth_colour == BLUE) {
        pin_write(GPIOA, 4, PIN_HIGH);
    }
}

void systick_init(void){
    // Part of the Arm Cortex Peripherals. Not found in RM0490
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
// Fig. 1 STAGE_0: provided fault logic inside generate_next_item()
item_colour_t apply_lfsr_fault(item_colour_t true_col) {
    lfsr_state = (lfsr_state >> 1) ^ (-(lfsr_state & 1u) & 0xD0000001u);
    
    if ((lfsr_state % 100) < 20) {
        return (item_colour_t)((true_col % 3) + 1);
    }
    return true_col;
}
// Fig. 1 STAGE_0: call provided generate_next_item(); then return to the FSM
void generate_next_item(void) {
    lfsr_state ^= SysTick->VAL;

    item_colour_t truth_colour = (item_colour_t)((lfsr_state % 3) + 1);
    item_colour_t sensor_colour = apply_lfsr_fault(truth_colour);

    set_truth_rgb(truth_colour);   // Real RGB (PA0/PA1/PA4) = truth_colour
    set_sensor_rgb(sensor_colour); // Sensed RGB (PA3/PA11/PA8) = sensor_colour
}

