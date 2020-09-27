#include "task2.h"

// Access to the ISRs to the state of the buttons in task 2C, and to request transitions
static uint8_t startStop_pressed = RESET;
static uint8_t pedestrian_pressed = RESET;
static uint8_t transition_requested = RESET;

/*  Used in Task 2A
 *  Contains a list of the ports where each LED D<i> is connected
 *  Index <i> corresponds to LED D<i+1>
 */
static GPIO_RegDef_t *LedsPortList[] = {GPIO_D1_PORT, 
                                        GPIO_D2_PORT, 
                                        GPIO_D3_PORT, 
                                        GPIO_D4_PORT};


/*  Used in Task 2A
 *  Contains a list of the pins associated with each LED
 *  Index <i> corresponds to LED D<i+1> 
 */
static uint8_t LedsPinList[] = {GPIO_D1_PIN, 
                                GPIO_D2_PIN,
                                GPIO_D3_PIN, 
                                GPIO_D4_PIN};

void Timer4A_Handler(void) {
    static uint8_t LedNum = 0;
    static uint8_t Dir = 1;

    TIM_ClearIT(TIM4, TIM_ITReadPos_TimeoutA);
    GPIO_PinToggle(LedsPortList[LedNum], LedsPinList[LedNum]);

    if(Dir && LedNum == (NUM_LEDS - 1)) {
        Dir = 0;
    } else if (Dir && LedNum < (NUM_LEDS - 1)) {
        LedNum++;
    } else if (!Dir && LedNum == 0) {
        Dir = 1;
    } else if (!Dir && LedNum > 0) {
        LedNum--;
    } else {
        // Software shall never reach here 
        LedNum = 0;
        Dir = 1; 
    }
}

void Task2A_Timer_Init(void) {
    // Enable the clock for TIM4
    TIM_PeriphClockCtrlByMask(SYSCTL_RCGCTIMER_TIM4_MASK, ENABLE);

    // Configure as Periodic Down-counter
    TIM_Init_t timer;
    timer.PeriodicModeCommand = ENABLE;
    timer.TIM_CompareAction = TIM_CompareAction_Disable;
    timer.TIM_Direction = TIM_Direction_DOWN;
    timer.TIM_SnapshotCommand = DISABLE;
    timer.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until the timer is ready to be accessed
    while(!(SYSCTL_PRTIMER_READ(SYSCTL_PRTIMER_TIM4_POS)));

    // Configure Timer to trigger with a frequency of 1Hz
    TIM_Init(TIM4, &timer, TIM_Port_Concatenated);
    TIM_ConfigIntervalConc(TIM4, SECONDS_TO_COUNT(1UL));

    // Configure interrupts for the Timer
    TIM_ConfigTimeoutIT(TIM4, ENABLE, TIM_Port_Concatenated);
    TIM_IRQConfig(TIM4, DISABLE, TIM_IRQ_ALL);
    TIM_IRQConfig(TIM4, ENABLE, TIM_IRQ_TimeoutA);

    // Clear previous interrupt signals
    TIM_ClearITAll(TIM4);
    NVIC_ClearPendingIRQ(TIM4A_IRQNumber);

    NVIC_EnableIRQ(TIM4A_IRQNumber);
}

void Task2A_Leds_Init(void) {
    // Enable the clock for the ports associated to the LEDs. 
    GPIO_PeriphClockCtrl(GPIO_D1_D2_PORT, ENABLE);
    GPIO_PeriphClockCtrl(GPIO_D3_D4_PORT, ENABLE);

    // Configure as digital outputs to drive the LEDs
    GPIO_InitDef_t leds;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    // wait until the ports are ready to be accessed
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D1_D2_POS)));
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D3_D4_POS)));

    // Initialize each LED and make sure each one is OFF 
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        GPIO_Init(LedsPortList[i], LedsPinList[i], &leds);
        GPIO_PinWrite(LedsPortList[i], LedsPinList[i], RESET);
    }
}

void Task2A_Init(void) {
    Task2A_Leds_Init();
    Task2A_Timer_Init();
}

void Task2A(void) {
    Task2A_Init();

    // start the timer
    TIM_Command(TIM4, ENABLE, TIM_Port_Concatenated); 

    while(1) {
        __asm volatile("WFI\t\n");
    }
}

 void Task2B_Leds_Init(void) {
    // Enable the clock for the port associated to the LEDs. 
    GPIO_PeriphClockCtrl(GPIO_D1_D2_PORT, ENABLE);

    // Configure as digital outputs to drive the LEDs
    GPIO_InitDef_t leds;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    // wait until the port is ready to be accessed
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D1_D2_POS)));

    // Initialize each LED and make sure each one is OFF 
    GPIO_Init(GPIO_D1_PORT, GPIO_D1_PIN, &leds);
    GPIO_Init(GPIO_D2_PORT, GPIO_D2_PIN, &leds);

    GPIO_PinWrite(GPIO_D1_PORT, GPIO_D1_PIN, RESET);
    GPIO_PinWrite(GPIO_D2_PORT, GPIO_D2_PIN, RESET);
 }

void Task2B_Timer_Init(void) {
    // enable the clock to this timer
    TIM_PeriphClockCtrlByMask(SYSCTL_RCGCTIMER_TIM5_MASK, ENABLE);


    // Configure as periodic downcounter
    TIM_Init_t timer;
    timer.PeriodicModeCommand = ENABLE;
    timer.TIM_CompareAction = TIM_CompareAction_Disable;
    timer.TIM_Direction = TIM_Direction_DOWN;
    timer.TIM_SnapshotCommand = DISABLE;
    timer.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until it is ready to be accessed
    while(!(SYSCTL_PRTIMER_READ(SYSCTL_PRTIMER_TIM5_POS)));
    TIM_Init(TIM5, &timer, TIM_Port_Concatenated);

    // Need a period of 1Hz
    TIM_ConfigIntervalConc(TIM5, SECONDS_TO_COUNT(1UL));

    // Configure interrupts for when the timer expires and clear previous events
    TIM_ConfigTimeoutIT(TIM5, ENABLE, TIM_Port_Concatenated);
    TIM_ClearITAll(TIM5);
    NVIC_ClearPendingIRQ(TIM5A_IRQNumber);
    TIM_IRQConfig(TIM5, ENABLE, TIM_IRQ_TimeoutA);
}

void Task2B_Buttons_Init(void) {
    // Provide clock to this port
    GPIO_PeriphClockCtrl(GPIO_SW1_SW2_PORT, ENABLE);

    // Configure as digital input with a pull-up resistor
    // Capture falling edges
    GPIO_InitDef_t buttons;
    buttons.ConfigIT = ENABLE;
    buttons.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_IN;
    buttons.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_PULLUP;
    buttons.GPIO_PinITConfig.GPIO_PinEdge = GPIO_PinEdge_FALLING;
    buttons.GPIO_PinITConfig.GPIO_PinSense = GPIO_PinSense_EDGE;

    // wait until the port is ready to be accessed
    while(!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_SW1_SW2_POS)));

    GPIO_Init(GPIO_SW1_PORT, GPIO_SW1_PIN, &buttons);
    GPIO_Init(GPIO_SW2_PORT, GPIO_SW2_PIN, &buttons);
    
    // clear previous events
    GPIO_PortClearIT(GPIO_SW1_SW2_PORT);
    NVIC_ClearPendingIRQ(GPIO_SW1_SW2_IRQNUMBER);
    GPIO_IRQConfig(GPIO_SW1_PORT, GPIO_SW1_PIN, ENABLE); 
    GPIO_IRQConfig(GPIO_SW2_PORT, GPIO_SW2_PIN, ENABLE); 
}

void Task2B_Init(void) {
    Task2B_Leds_Init();
    Task2B_Timer_Init();
    Task2B_Buttons_Init();

    // Set the same priority to avoid nesting    
    NVIC_SetPriority(TIM5A_IRQNumber, 5U);
    NVIC_SetPriority(GPIO_SW1_SW2_IRQNUMBER, 5U);

    // Enable interrupts from the processor's side
    NVIC_EnableIRQ(TIM5A_IRQNumber);
    NVIC_EnableIRQ(GPIO_SW1_SW2_IRQNUMBER);

    // Start counting
    TIM_Command(TIM5, ENABLE, TIM_Port_Concatenated);
}

void Task2B(void) {
    Task2B_Init();

    while(1);
}

void Timer5A_Handler(void) {
    // Toggles the LED
    if (TIM_ReadITStatus(TIM5, TIM_ITReadPos_TimeoutA)) {
        TIM_ClearIT(TIM5, TIM_ITReadPos_TimeoutA);
        GPIO_PinToggle(GPIO_D1_PORT, GPIO_D1_PIN);
    } else {
        // Software shall not reach here
        TIM_ClearITAll(TIM5);
    }
}

void GPIOJ_Handler(void) {
    if(GPIO_PinReadITStatus(GPIO_SW1_PORT, GPIO_SW1_PIN)) {
        // stop blinking and turn on LED D2
        GPIO_PinClearIT(GPIO_SW1_PORT, GPIO_SW1_PIN);
        TIM_Command(TIM5, DISABLE, TIM_Port_Concatenated);
        TIM_LoadCountConc(TIM5, SECONDS_TO_COUNT(1UL));
        GPIO_PinWrite(GPIO_D1_PORT, GPIO_D1_PIN, RESET);
        GPIO_PinWrite(GPIO_D2_PORT, GPIO_D2_PIN, SET);
    } else if(GPIO_PinReadITStatus(GPIO_SW2_PORT, GPIO_SW2_PIN)) {
        // start/keep counting and make sure D2 is off
        GPIO_PinClearIT(GPIO_SW2_PORT, GPIO_SW2_PIN);
        TIM_Command(TIM5, ENABLE, TIM_Port_Concatenated);
        GPIO_PinWrite(GPIO_D2_PORT, GPIO_D2_PIN, RESET);
    } else {
        // Software shall not reach here
        GPIO_PortClearIT(GPIO_SW1_SW2_PORT);
    }
}


void Task2C_Leds_Init(void) {
    // Configure as digital outputs to drive the LEDs
    GPIO_InitDef_t leds;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    GPIO_Init(TL_Port, RED_PIN, &leds);
    GPIO_Init(TL_Port, YELLOW_PIN, &leds);
    GPIO_Init(TL_Port, GREEN_PIN, &leds); 

    // make sure these are initially off
    GPIO_PinWrite(TL_Port, RED_PIN, RESET);
    GPIO_PinWrite(TL_Port, YELLOW_PIN, RESET);
    GPIO_PinWrite(TL_Port, GREEN_PIN, RESET);
}

void Task2C_Buttons_Init(void) {
    // Configure as digital inputs to read the state of the buttons
    GPIO_InitDef_t buttons;
    buttons.ConfigIT = DISABLE;
    buttons.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_IN;
    buttons.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    GPIO_Init(TL_Port, START_STOP_PIN, &buttons);
    GPIO_Init(TL_Port, PEDESTRIAN_PIN, &buttons); 
}

void Task2C_Timers_Init(void) {
    // provide clock to the traffic-light timers
    TIM_PeriphClockCtrlByMask(PEDESTRIAN_BUTTON_TIMER_MASK, ENABLE);
    TIM_PeriphClockCtrlByMask(START_STOP_BUTTON_TIMER_MASK, ENABLE);
    TIM_PeriphClockCtrlByMask(TRANSITION_TIMER_MASK, ENABLE);
   

    // Configure as periodic downcounters
    TIM_Init_t timers;
    timers.PeriodicModeCommand = ENABLE;
    timers.TIM_CompareAction = TIM_CompareAction_Disable;
    timers.TIM_Direction = TIM_Direction_DOWN;
    timers.TIM_SnapshotCommand = DISABLE;
    timers.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until times can be accessed
    while(!(SYSCTL_PRTIMER_READ(PEDESTRIAN_BUTTON_TIMER_POS)));
    while(!(SYSCTL_PRTIMER_READ(START_STOP_BUTTON_TIMER_POS)));
    while(!(SYSCTL_PRTIMER_READ(TRANSITION_TIMER_POS)));

    TIM_Init(PEDESTRIAN_BUTTON_TIMER, &timers, TIM_Port_Concatenated);
    TIM_Init(TRANSITION_TIMER, &timers, TIM_Port_Concatenated);
    TIM_Init(START_STOP_BUTTON_TIMER, &timers, TIM_Port_Concatenated);

    // Buttons are registered as inputs only after being held for 2 seconds
    TIM_ConfigIntervalConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
    TIM_ConfigIntervalConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));

    // Transitions normally occur every 5 seconds
    TIM_ConfigIntervalConc(TRANSITION_TIMER, SECONDS_TO_COUNT(5UL));

    // Clear previous events
    TIM_ClearITAll(PEDESTRIAN_BUTTON_TIMER);
    TIM_ClearITAll(START_STOP_BUTTON_TIMER);
    TIM_ClearITAll(TRANSITION_TIMER);
    NVIC_ClearPendingIRQ(PEDESTRIAN_BUTTON_IRQ);
    NVIC_ClearPendingIRQ(START_STOP_BUTTON_IRQ);
    NVIC_ClearPendingIRQ(TRANSITION_TIMER_IRQ);

    TIM_ConfigTimeoutIT(PEDESTRIAN_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
    TIM_ConfigTimeoutIT(START_STOP_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
    TIM_ConfigTimeoutIT(TRANSITION_TIMER, ENABLE, TIM_Port_Concatenated);

    TIM_IRQConfig(PEDESTRIAN_BUTTON_TIMER, ENABLE, TIM_IRQ_TimeoutA);
    TIM_IRQConfig(START_STOP_BUTTON_TIMER, ENABLE, TIM_IRQ_TimeoutA);
    TIM_IRQConfig(TRANSITION_TIMER, ENABLE, TIM_IRQ_TimeoutA);
}

void Task2C_Init(void) {
    // provide clock to the port where LEDs and buttons are connected to
    GPIO_PeriphClockCtrlByMask(SYSCTL_TL_PORT_MASK, ENABLE);

    // wait until the port is ready to be accessed
    while(!SYSCTL_PRGPIO_READ(SYSCTL_TL_PORT_POS));

    Task2C_Leds_Init();
    Task2C_Buttons_Init();
    Task2C_Timers_Init();
}

void Timer1A_Handler(void) {
    if (TIM_ReadITStatus(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA)) {
        pedestrian_pressed = SET;
        TIM_Command(PEDESTRIAN_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
        TIM_ClearIT(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
        TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
    } else {
        // Software shall not reach here
        TIM_ClearITAll(TIM1);
    }
}

void Timer2A_Handler(void) {
    if (TIM_ReadITStatus(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA)) {
        startStop_pressed = SET;
        TIM_Command(START_STOP_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
        TIM_ClearIT(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
        TIM_LoadCountConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
    } else {
        // Software shall not reach here
        TIM_ClearITAll(TIM2);
    }
}

void Timer3A_Handler(void) {
    if (TIM_ReadITStatus(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA)) {
        TIM_ClearIT(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA);   
        transition_requested = SET;
    } else {
        // Software shall not reach here
        TIM_ClearITAll(TIM3);
    }
}

void Task2C(void) {
    Task2C_Init();

    static TL_states_e present_state = IDLE;
    static TL_states_e next_state = IDLE;

    // Software will keep track of the previous and new values to detect rising edges
    // But must only register inputs after a 2-second press, thus the 'tmp' suffix-ed variables
    // represent a button press that hasn't been held for 2 seconds yet
    uint8_t startStop_prev = RESET;
    uint8_t startSTop_new = RESET;
    uint8_t startStop_tmp = RESET;
    startStop_pressed = RESET;

    uint8_t pedestrian_prev = RESET;
    uint8_t pedestrian_new = RESET;
    uint8_t pedestrian_tmp = RESET;
    pedestrian_pressed = RESET;

    NVIC_SetPriority(PEDESTRIAN_BUTTON_IRQ, 5U);
    NVIC_SetPriority(START_STOP_BUTTON_IRQ, 5U);
    NVIC_SetPriority(TRANSITION_TIMER_IRQ, 5U);

    NVIC_EnableIRQ(PEDESTRIAN_BUTTON_IRQ);
    NVIC_EnableIRQ(START_STOP_BUTTON_IRQ);
    NVIC_EnableIRQ(TRANSITION_TIMER_IRQ);

    // Start counting
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
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, 2UL * 16000000UL);
        }

        // same as above for start/stop button
        if (startStop_tmp && !TIM_ReadCounterStatus(START_STOP_BUTTON_TIMER, TIM_Port_Concatenated)) {
            TIM_Command(START_STOP_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
        } else if (!startStop_tmp) {
            TIM_Command(START_STOP_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_LoadCountConc(START_STOP_BUTTON_TIMER, 2UL * 16000000UL);
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
                    next_state = WARN;
                    transition_requested = SET;
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
        if (transition_requested) {
            present_state = next_state;
            pedestrian_pressed = RESET;
            startStop_pressed = RESET;

            TIM_ClearIT(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_ClearIT(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_ClearIT(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);

            // restart the pedestrian count too, or else input would be ambigous
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(2UL));
            TIM_LoadCountConc(TRANSITION_TIMER, SECONDS_TO_COUNT(5UL));
            
            transition_requested = RESET;
        }

        // update data
        startStop_prev = startSTop_new;
        pedestrian_prev = pedestrian_new;
    }
}