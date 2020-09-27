#include "task2.h"

void Task2(void) {
    Task2_Init();

    TL_states_e present_state;
    TL_states_e next_state;
    uint32_t ticks = 0; 

    // Software will keep track of the previous and new values of the
    // buttons in order to detect rising edges. If there is a rising
    // edge, the values of the buttons will be latched until the next transition.
    uint8_t startStop_prev = 0;
    uint8_t startSTop_new = 0;
    uint8_t startStop_pressed = 0;

    uint8_t pedestrian_prev = 0;
    uint8_t pedestrian_new = 0;
    uint8_t pedestrian_pressed = 0;
    while (1) {
        
        // Read the buttons
        startSTop_new = GPIO_PinRead(TL_Port, START_STOP_PIN);
        pedestrian_new = GPIO_PinRead(TL_Port, PEDESTRIAN_PIN);

        // Looking for a rising edge to latch the start/stop button
        if (!startStop_pressed) {
            if (!startStop_prev && startSTop_new) {
                startStop_pressed = SET;
            }
        } 

        // Looking for a rising edge to latch the pedestrian button
        if (!pedestrian_pressed) {
            if (!pedestrian_prev && pedestrian_new) {
                pedestrian_pressed = SET; 
            }
        }

        // next-state logic
        switch (present_state) {
            case IDLE:
                if (startStop_pressed) {
                    next_state = STOP;
                } else {
                    next_state = IDLE;
                }
                break;
            case STOP:
                if (startStop_pressed) {
                    next_state = IDLE;
                } else {
                    next_state = GO;
                }
                break;
            case GO:
                if (startStop_pressed) {
                    next_state = IDLE;
                } else if (pedestrian_pressed) {
                    next_state = WARN;
                } else {
                    next_state = STOP;
                }
                break;
            case WARN:
                if (startStop_pressed) {
                    next_state = IDLE;
                } else {
                    next_state = STOP;
                }
                break;
            default:
                next_state = IDLE;
                break;
        }

        // output logic (Moore Type)
        switch (present_state) {
            case IDLE:
                GPIO_PinWrite(TL_Port, RED_PIN, RESET);
                GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
                GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
                break;
            case STOP:
                GPIO_PinWrite(TL_Port, RED_PIN, SET);
                GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
                GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
                break;
            case GO:
                GPIO_PinWrite(TL_Port, RED_PIN, RESET);
                GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
                GPIO_PinWrite(TL_Port, GREEN_PIN, SET);
                break;
            case WARN:
                GPIO_PinWrite(TL_Port, RED_PIN, RESET);
                GPIO_PinWrite(TL_Port, YELLOW_PIN, SET);
                GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
                break;
            default:
                GPIO_PinWrite(TL_Port, RED_PIN, RESET);
                GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
                GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
                break;
        }

        // state-transition
        if (ticks >= STATE_LENGTH_IN_TICKS) {
            present_state = next_state;
            pedestrian_pressed = RESET; 
            startStop_pressed = RESET; 
            ticks = 0;
        }

        // update data
        startStop_prev = startSTop_new;
        pedestrian_prev = pedestrian_new;

        ticks++;
    }
}

void Task2_Init(void) {
    GPIO_PeriphClockCtrlByMask(SYSCTL_RCGCGPIO_PORTL_MASK, ENABLE);

    GPIO_InitDef_t leds;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;
    
    GPIO_InitDef_t buttons;
    buttons.ConfigIT = DISABLE;
    buttons.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_IN;
    buttons.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    // Make sure the GPIO port is ready to be accessed before initialization
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_PORTL_POS)));

    GPIO_Init(TL_Port, RED_PIN, &leds);
    GPIO_Init(TL_Port, YELLOW_PIN, &leds);
    GPIO_Init(TL_Port, GREEN_PIN, &leds);  
    GPIO_Init(TL_Port, START_STOP_PIN, &buttons);
    GPIO_Init(TL_Port, PEDESTRIAN_PIN, &buttons); 

    GPIO_PinWrite(TL_Port, RED_PIN, RESET);
    GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
    GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
}
