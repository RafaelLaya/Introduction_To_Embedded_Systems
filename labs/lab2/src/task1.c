#include "task1.h"

/*  Used in Task 1A
 *  Contains a list of the ports where each LED D<i> is connected
 *  Index <i> corresponds to LED D<i+1>
 */
static GPIO_RegDef_t *LedsPortList[] = {GPIO_D1_PORT, 
                                        GPIO_D2_PORT, 
                                        GPIO_D3_PORT, 
                                        GPIO_D4_PORT};

/*  Used in Task 1A
 *  Contains a list of the pins associated with each LED
 *  Index <i> corresponds to LED D<i+1> 
 */
static uint8_t LedsPinList[] = {GPIO_D1_PIN, 
                                GPIO_D2_PIN,
                                GPIO_D3_PIN, 
                                GPIO_D4_PIN};

void Task1A_GPIO_Init(void) {
    // Enable the clock for the ports associated to the LEDs. 
    GPIO_PeriphClockCtrl(GPIO_D1_D2_PORT, ENABLE);
    GPIO_PeriphClockCtrl(GPIO_D3_D4_PORT, ENABLE);

    // GPIOs will be configured as digital outputs to drive the LEDs
    GPIO_InitDef_t leds;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    // wait until the ports are ready to be accessed
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D1_D2_POS)));
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D3_D4_POS)));

    // Initialize each LED and make sure each LED is initially off 
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        GPIO_Init(LedsPortList[i], LedsPinList[i], &leds);
        GPIO_PinWrite(LedsPortList[i], LedsPinList[i], RESET);
    }


}

void Task1A_TIM_Init(void) {
    // Enable the clock for TIM0
    TIM_PeriphClockCtrlByMask(SYSCTL_RCGCTIMER_TIM0_MASK, ENABLE);

    // Configure as Periodic Down-counter
    TIM_Init_t timer;
    timer.PeriodicModeCommand = ENABLE;
    timer.TIM_CompareAction = TIM_CompareAction_Disable;
    timer.TIM_Direction = TIM_Direction_DOWN;
    timer.TIM_SnapshotCommand = DISABLE;
    timer.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until the timer is ready to be accessed
    while(!(SYSCTL_PRTIMER_READ(SYSCTL_PRTIMER_TIM0_POS)));
    TIM_Init(TIM0, &timer, TIM_Port_Concatenated);

    // Configure Timer to trigger with a frequency of 1Hz
    TIM_ConfigIntervalConc(TIM0, SECONDS_TO_COUNT(1UL));

    // Clear previous events
    TIM_ClearITAll(TIM0);

}

void Task1A_Init(void) {
    Task1A_GPIO_Init();
    Task1A_TIM_Init();
}

void Task1A(void) {
    Task1A_Init();

    // start the timer
    TIM_Command(TIM0, ENABLE, TIM_Port_Concatenated);
    
    while (1) {
        // Turn each LED ON in order (D1, D2, D3, D4) with some delay in-between
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            GPIO_PinWrite(LedsPortList[i], LedsPinList[i], SET);
            while(!(TIM_ReadRawITStatus(TIM0, TIM_ITReadPos_TimeoutA)));
            TIM_ClearIT(TIM0, TIM_ITReadPos_TimeoutA);
        }    

        // Then turn each LED OFF in reverse order, again with some delay in-between
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            GPIO_PinWrite(LedsPortList[NUM_LEDS - 1 - i], 
                          LedsPinList[NUM_LEDS - 1 - i], 
                          RESET);
            while(!(TIM_ReadRawITStatus(TIM0, TIM_ITReadPos_TimeoutA)));
            TIM_ClearIT(TIM0, TIM_ITReadPos_TimeoutA);
        }
    }
}

void Task1B_Leds_Init(void) {
    // Configure as digital outputs to drive the LEDs
    GPIO_InitDef_t leds;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    GPIO_Init(TL_Port, RED_PIN, &leds);
    GPIO_Init(TL_Port, YELLOW_PIN, &leds);
    GPIO_Init(TL_Port, GREEN_PIN, &leds); 
}

void Task1B_Buttons_Init(void) {
    // Configure as digital inputs to read the buttons
    GPIO_InitDef_t buttons;
    buttons.ConfigIT = DISABLE;
    buttons.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_IN;
    buttons.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;
    GPIO_Init(TL_Port, START_STOP_PIN, &buttons);
    GPIO_Init(TL_Port, PEDESTRIAN_PIN, &buttons); 

    // Make sure all LEDs are off initially
    GPIO_PinWrite(TL_Port, RED_PIN, RESET);
    GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
    GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
}

void Task1B_Timers_Init(void) {
    // Enable the clock of the timers utilized in this task
    TIM_PeriphClockCtrlByMask(PEDESTRIAN_BUTTON_TIMER_MASK, ENABLE);
    TIM_PeriphClockCtrlByMask(START_STOP_BUTTON_TIMER_MASK, ENABLE);
    TIM_PeriphClockCtrlByMask(TRANSITION_TIMER_MASK, ENABLE);

    // Configure as periodic down counters
    TIM_Init_t timers;
    timers.PeriodicModeCommand = ENABLE;
    timers.TIM_CompareAction = TIM_CompareAction_Disable;
    timers.TIM_Direction = TIM_Direction_DOWN;
    timers.TIM_SnapshotCommand = DISABLE;
    timers.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until these are ready to be accessed
    while(!(SYSCTL_PRTIMER_READ(PEDESTRIAN_BUTTON_TIMER_POS)));
    while(!(SYSCTL_PRTIMER_READ(START_STOP_BUTTON_TIMER_POS)));
    while(!(SYSCTL_PRTIMER_READ(TRANSITION_TIMER_POS)));

    TIM_Init(PEDESTRIAN_BUTTON_TIMER, &timers, TIM_Port_Concatenated);
    TIM_Init(TRANSITION_TIMER, &timers, TIM_Port_Concatenated);
    TIM_Init(START_STOP_BUTTON_TIMER, &timers, TIM_Port_Concatenated);

    // the buttons must be pressed for two seconds for the system to register 
    // the press
    TIM_ConfigIntervalConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
    TIM_ConfigIntervalConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));

    // A transition normally occurs every 5 seconds
    TIM_ConfigIntervalConc(TRANSITION_TIMER, SECONDS_TO_COUNT(5UL));

    // Clear any previous events
    TIM_ClearITAll(PEDESTRIAN_BUTTON_TIMER);
    TIM_ClearITAll(START_STOP_BUTTON_TIMER);
    TIM_ClearITAll(TRANSITION_TIMER);
}

void Task1B_Init(void) {
    // Turn on the clock where the GPIOs related to the LEDs and buttons are
    GPIO_PeriphClockCtrlByMask(SYSCTL_RCGCGPIO_PORTL_MASK, ENABLE);

    // wait until the port is ready to be accessed
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_PORTL_POS)));

    Task1B_Leds_Init();
    Task1B_Buttons_Init();
    Task1B_Timers_Init();
}

void Task1B(void) {
    Task1B_Init();

    TL_states_e present_state;
    TL_states_e next_state; 

    // Software will keep track of the previous and new values to detect rising edges
    // But must only register inputs after a 2-second press, thus the 'tmp' suffix-ed variables
    // represent a button press that hasn't been held for 2 seconds yet
    uint8_t startStop_prev = RESET;
    uint8_t startSTop_new = RESET;
    uint8_t startStop_tmp = RESET;
    uint8_t startStop_pressed = RESET;

    uint8_t pedestrian_prev = RESET;
    uint8_t pedestrian_new = RESET;
    uint8_t pedestrian_tmp = RESET;
    uint8_t pedestrian_pressed = RESET;

    uint8_t transition_requested = RESET;

    // Start the timer
    TIM_Command(TRANSITION_TIMER, ENABLE, TIM_Port_Concatenated);

    while (1) {
        
        // Read the buttons
        startSTop_new = GPIO_PinRead(TL_Port, START_STOP_PIN);
        pedestrian_new = GPIO_PinRead(TL_Port, PEDESTRIAN_PIN);

        // Check for rising edges
        if (!startStop_prev && startSTop_new) {
            startStop_tmp = SET;
        } else if(startStop_prev && !startSTop_new) {
            startStop_tmp = RESET;
        }

        if (!pedestrian_prev && pedestrian_new) {
            pedestrian_tmp = SET; 
        } else if (pedestrian_prev && !pedestrian_new) {
            pedestrian_tmp = RESET;
        }

        // start counting if the button gets pressed, and restart the count if the button is not pressed
        // for 2-seconds straight
        if (pedestrian_tmp && !TIM_ReadCounterStatus(PEDESTRIAN_BUTTON_TIMER, TIM_Port_Concatenated)) {
            TIM_Command(PEDESTRIAN_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
        } else if (!pedestrian_tmp) {
            TIM_Command(PEDESTRIAN_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
        }

        // Exactly as above
        if (startStop_tmp && !TIM_ReadCounterStatus(START_STOP_BUTTON_TIMER, TIM_Port_Concatenated)) {
            TIM_Command(START_STOP_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
        } else if (!startStop_tmp) {
            TIM_Command(START_STOP_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_LoadCountConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
        }

        // If a timer has expired, register the input
        if (TIM_ReadRawITStatus(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA)) {
            pedestrian_pressed = SET;
            TIM_Command(PEDESTRIAN_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_ClearIT(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
        }

        if (TIM_ReadRawITStatus(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA)) {
            startStop_pressed = SET;
            TIM_Command(START_STOP_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_ClearIT(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_LoadCountConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
        }

        // next-state logic
        switch (present_state) {
            case IDLE:
                if (startStop_pressed) {
                    next_state = STOP;
                    transition_requested = SET;
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
                    transition_requested = SET;
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

        // trigger a transition if the transition timer has expired
        transition_requested = TIM_ReadRawITStatus(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA) ? SET : transition_requested;
 
        if (transition_requested) {
            present_state = next_state;
            transition_requested = RESET;
            pedestrian_pressed = RESET;
            startStop_pressed = RESET;

            TIM_ClearIT(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_ClearIT(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_ClearIT(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);

            // restart the pedestrian count too, or else input would be ambigous
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
            TIM_LoadCountConc(TRANSITION_TIMER, SECONDS_TO_COUNT(5UL));
        }

        // update data
        startStop_prev = startSTop_new;
        pedestrian_prev = pedestrian_new;
    }
}

