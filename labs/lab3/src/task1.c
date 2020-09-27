
#include "task1.h"

#if (CHOSEN == TASK1)
void ADC0Sequence3_Handler(void) {
    // Clear the IT Flag
    ADC_ClearIT(ADC0, ADC_ITReadPos_Sequencer3);

    // take the sample and decode the temperature
    float temp;
    temp = ADC_ReadValue(ADC0, ADC_FIFO3) * 247.5 / 4096.0;
    temp = -temp;
    temp = temp + 147.5;

    // Decide on number of blinking LEDs value based on temperature
    if (temp < 20.0) {
        num_leds = 1U;
    } else if(temp < 24.0) {
        num_leds = 2U;
    } else if(temp < 28.0) {
        num_leds = 3U;
    } else {
        num_leds = 4U;
    }
}
#endif

void Task1(void) {
    // Initialize peripherals common for task 1 and 2.1
    Task_Common_Init();

    // Start in slow mode (12MHz)
    TM_states_e present_state = TM_SLOW;
    TM_states_e next_state = TM_SLOW;
    
    // Start the timers
    TIM_Command(TIM0, ENABLE, TIM_Port_Concatenated);
    TIM_Command(TIM1, ENABLE, TIM_Port_Concatenated);

    // Let the CPU start taking interrupts
    NVIC_EnableIRQ(ADC0_Sequence3_IRQNumber);
    NVIC_EnableIRQ(TIM1A_IRQNumber);
    NVIC_EnableIRQ(GPIOJ_IRQNumber);

    while(1) {
        
        // Wait until a button is pressed
        while (!SW1_pressed && !SW2_pressed);

        // decide on what will be the next state
        switch (present_state) {
            case TM_SLOW:
                if (SW2_pressed) {
                    next_state = TM_FAST;
                } 
                break;
            
            case TM_FAST:
                if(SW1_pressed) {
                    next_state = TM_SLOW;
                }
                break;
        }

        // If there is a change, make the change
        if(present_state != next_state) {
            present_state = next_state;

            // But remember to make the change actually happen by configuring the clock
            if(present_state == TM_SLOW) {
                PLL_Init(PRESET3);
            } else if(present_state == TM_FAST) {
                PLL_Init(PRESET1);
            } 
        } 

        SW1_pressed = RESET;
        SW2_pressed = RESET;
    }
}