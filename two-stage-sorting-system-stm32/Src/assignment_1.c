#include "assignment_1.h"
#include "stm32c031xx.h"

volatile uint32_t lfsr_state = 0xA5A5A5A5;


static void Set_Sensor_RGB(ItemColour_t sensor_colour) {
    pin_write(GPIOA, 3, PIN_LOW);
    pin_write(GPIOB, 11, PIN_LOW);
    pin_write(GPIOA, 8, PIN_LOW);

    if (sensor_colour == RED) {
        pin_write(GPIOA, 3, PIN_HIGH);
    } else if (sensor_colour == GREEN) {
        pin_write(GPIOB, 11, PIN_HIGH);
    } else if (sensor_colour == BLUE) {
        pin_write(GPIOA, 8, PIN_HIGH);
    }
}

static void Set_Turth_RGB(ItemColour_t truth_colour) {
    pin_write(GPIOA, 0, PIN_LOW);
    pin_write(GPIOB, 1, PIN_LOW);
    pin_write(GPIOA, 4, PIN_LOW);

    if (truth_colour == RED) {
        pin_write(GPIOA, 0, PIN_HIGH);
    } else if (truth_colour == GREEN) {
        pin_write(GPIOB, 1, PIN_HIGH);
    } else if (truth_colour == BLUE) {
        pin_write(GPIOA, 4, PIN_HIGH);
    }
}

void SysTick_Init(void){
    // Part of the Arm Cortex Peripherals. Not found in RM0490
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
// Subroutine to add faulty detections
ItemColour_t Apply_LFSR_Fault(ItemColour_t true_col) {
    lfsr_state = (lfsr_state >> 1) ^ (-(lfsr_state & 1u) & 0xD0000001u);
    
    if ((lfsr_state % 100) < 20) {
        return (ItemColour_t)((true_col % 3) + 1);
    }
    return true_col;
}
// Subroutine to generate item and sensor detection 
void Generate_Next_Item(void) {
    lfsr_state ^= SysTick->VAL;
    ItemColour_t truth_colour = (ItemColour_t)((lfsr_state % 3) + 1);
    ItemColour_t sensor_colour = Apply_LFSR_Fault(truth_colour);
    Set_Turth_RGB(truth_colour);
    Set_Sensor_RGB(sensor_colour);
    if (truth_colour != sensor_colour) {
        pin_write(GPIOA, 9, PIN_HIGH);
    } else {
        pin_write(GPIOA, 9, PIN_LOW);
    }
}

