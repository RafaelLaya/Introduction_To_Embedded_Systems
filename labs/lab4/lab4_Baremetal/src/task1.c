
#include "task1.h"


// The ADC0 handler will request printing temperature
static volatile uint8_t PrintRequested = RESET;
static float temp = 0.0;

/*  
 *  Contains a list of the ports where each LED D<i> is connected
 *  Index <i> corresponds to LED D<i+1>
 */
GPIO_RegDef_t *LedsPortList[] = {GPIO_D1_PORT, 
                                 GPIO_D2_PORT, 
                                 GPIO_D3_PORT, 
                                 GPIO_D4_PORT};
/*  
 *  Contains a list of the pins associated with each LED
 *  Index <i> corresponds to LED D<i+1> 
 */
uint8_t LedsPinList[] = {GPIO_D1_PIN, 
                         GPIO_D2_PIN,
                         GPIO_D3_PIN, 
                         GPIO_D4_PIN};

// Number of LEDs that should be blinking, based on temperature
volatile uint8_t num_leds = 0;

// Status of the buttons
volatile uint8_t SW2_pressed = RESET;
volatile uint8_t SW1_pressed = RESET;

void ADC0Sequence3_Handler(void) {
    // Clear the IT Flag
    ADC_ClearIT(ADC0, ADC_ITReadPos_Sequencer3);

    // take the sample and decode the temperature
    temp = ADC_TO_CELCIUS(ADC_ReadValue(ADC0, ADC_FIFO3));

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

    PrintRequested = SET;
}

void Task1B(void) {
    Task1B_Init();

    // Start in slow mode (12MHz)
    TM_states_e present_state = TM_SLOW;
    TM_states_e next_state = TM_SLOW;
    
    // Start the screen on cyan, text is white
    LCD_ColorFill(Color4[3]);
    LCD_SetCursor(0, 0);
    LCD_SetTextColor(255, 255, 255);

    // Start the timers
    TIM_Command(TIM0, ENABLE, TIM_Port_Concatenated);
    TIM_Command(TIM1, ENABLE, TIM_Port_Concatenated);

    // Let the CPU start taking interrupts
    NVIC_EnableIRQ(ADC0_Sequence3_IRQNumber);
    NVIC_EnableIRQ(TIM1A_IRQNumber);
    NVIC_EnableIRQ(GPIOJ_IRQNumber);

    while(1) {
        
        // Wait until a button is pressed or until we have to print
        while (!SW1_pressed && !SW2_pressed && !PrintRequested);
        
        if (PrintRequested) {
            PrintRequested = RESET;

            // This should never be a problem since the ADC is triggered once per second
            // But to be safe, make a critical section to avoid corruption of temp
            LCD_SetCursor(0, 0);
            LCD_Printf("The current temperature is ");
            __asm volatile("cpsid i\t\n");
            LCD_PrintFloat(temp);
            LCD_Printf(" C, ");
            LCD_PrintFloat(CELCIUS_TO_FARENH(temp));
            __asm volatile("cpsie i\t\n");

            if (present_state == TM_SLOW) {
                // Extra space before 12 to normalize length of the string
                LCD_PrintString(" F\r\nThe current clock frequency is  12 MHz");
            } else {
                LCD_PrintString(" F\r\nThe current clock frequency is 120 MHz");
            }
        }

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

void Task1C(void) {
    Task1C_Init();

    // Start in slow mode (12MHz)
    TM_states_e present_state = TM_SLOW;
    TM_states_e next_state = TM_SLOW;
    
    // set the background to cyan, text is white
    LCD_ColorFill(Color4[3]);
    LCD_SetTextColor(255, 255, 255);

    // Draw the buttons
    LCD_DrawFilledCircle(FAST_X, FAST_Y, RADIUS, FAST_COLOR);
    LCD_DrawFilledCircle(SLOW_X, SLOW_Y, RADIUS, SLOW_COLOR);

    // Label the buttons
    LCD_SetCursor(FAST_LABEL_X, FAST_LABEL_Y);
    LCD_PrintString("FAST");
    LCD_SetCursor(SLOW_LABEL_X, SLOW_LABEL_Y);
    LCD_PrintString("SLOW");
    LCD_SetCursor(0, 0);

    // Start the timers
    TIM_Command(TIM0, ENABLE, TIM_Port_Concatenated);
    TIM_Command(TIM1, ENABLE, TIM_Port_Concatenated);

    // Let the CPU start taking interrupts
    NVIC_EnableIRQ(ADC0_Sequence3_IRQNumber);
    NVIC_EnableIRQ(TIM1A_IRQNumber);

    while(1) {
        
        // Wait until a button is pressed or until we have to print
        while (!SW1_pressed && !SW2_pressed && !PrintRequested) {
            unsigned long x = Touch_ReadX();
            unsigned long y = Touch_ReadY();

            if(FAST_LEFT_LIMIT < x && x < FAST_RIGHT_LIMIT && 
                FAST_BOT_LIMIT < y && y < FAST_TOP_LIMIT) {
                SW2_pressed = SET;
            }

            if(SLOW_LEFT_LIMIT < x && x < SLOW_RIGHT_LIMIT && 
                SLOW_BOT_LIMIT < y && y < SLOW_TOP_LIMIT) {
                SW1_pressed = SET;
            }
        }
        
        if (PrintRequested) {
            PrintRequested = RESET;

            // This should never be a problem since the ADC is triggered once per second
            // But to be safe, make a critical section to avoid corruption of temp
            LCD_SetCursor(0, 0);
            LCD_Printf("The current temperature is ");
            __asm volatile("cpsid i\t\n");
            LCD_PrintFloat(temp);
            LCD_Printf(" C, ");
            LCD_PrintFloat(CELCIUS_TO_FARENH(temp));
            __asm volatile("cpsie i\t\n");

            if (present_state == TM_SLOW) {
                // Extra space before 12 to normalize length of the string
                LCD_PrintString(" F\r\nThe current clock frequency is  12 MHz");
            } else {
                LCD_PrintString(" F\r\nThe current clock frequency is 120 MHz");
            }
        }

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

void Timer0_Init(void) {
    // Enable the clock for Timer0
    SYSCTL_RCGCTIMER_CMD(SYSCTL_RCGCTIMER_TIM0_MASK, ENABLE);
    
    // configure as a periodic down-counter
    TIM_Init_t adctrigger;
    adctrigger.PeriodicModeCommand = ENABLE;
    adctrigger.TIM_CompareAction = TIM_CompareAction_Disable;
    adctrigger.TIM_Direction = TIM_Direction_DOWN;
    adctrigger.TIM_SnapshotCommand = DISABLE;
    adctrigger.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until peripheral is ready
    while(!(SYSCTL_PRTIMER_READ(SYSCTL_PRTIMER_TIM0_POS)));

    // Make sure alternate clock (PIOSC) is selected
    TIM_ConfigAltClk(TIM0, ENABLE);

    // Initialize
    TIM_Init(TIM0, &adctrigger, TIM_Port_Concatenated);
    TIM_ConfigIntervalConc(TIM0, SECONDS_TO_COUNT(ADC_TRIGGER_PERIOD, PIOSC_FREQ));
    TIM_ConfigADCTrigger(TIM0, ENABLE, TIM_ADCEvent_TimeoutA);
    TIM_EnableADCTrigger(TIM0, ENABLE, TIM_Port_Concatenated);
}

void Timer1_Init(void) {
    // Enable the clock for Timer 1
    SYSCTL_RCGCTIMER_CMD(SYSCTL_RCGCTIMER_TIM1_MASK, ENABLE);
    
    // Configure as periodic down-counter
    TIM_Init_t blinker;
    blinker.PeriodicModeCommand = ENABLE;
    blinker.TIM_CompareAction = TIM_CompareAction_Disable;
    blinker.TIM_Direction = TIM_Direction_DOWN;
    blinker.TIM_SnapshotCommand = DISABLE;
    blinker.TIM_WaitOnTriggerCommand = DISABLE;

    // Wait until peripheral is ready
    while(!(SYSCTL_PRTIMER_READ(SYSCTL_PRTIMER_TIM1_POS)));

    // Use PIOSC
    TIM_ConfigAltClk(TIM1, ENABLE);

    // Initialize
    TIM_Init(TIM1, &blinker, TIM_Port_Concatenated);
    
    TIM_ConfigIntervalConc(TIM1, BLINKING_PERIOD_IN_COUNTS);

    // Clear previous events and enable interrupt generation
    TIM_ClearITAll(TIM1);
    NVIC_ClearPendingIRQ(TIM1A_IRQNumber);
    NVIC_ClearPendingIRQ(TIM1B_IRQNumber);
    TIM_IRQConfig(TIM1, ENABLE, TIM_IRQ_TimeoutA);

    NVIC_SetPriority(TIM1A_IRQNumber, DEFAULT_PRIO);
}

void LED_Init(void) {
    // Enable the clock for the ports where the LEDs are
    SYSCTL_RCGCGPIO_CMD(SYSCTL_RCGCGPIO_D1_D2_MASK, ENABLE);
    SYSCTL_RCGCGPIO_CMD(SYSCTL_RCGCGPIO_D3_D4_MASK, ENABLE);

    // Configure as Digital Outputs with no Pull-ups
    GPIO_InitDef_t leds;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;
    
    // Wait until these ports are ready
    while(!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D1_D2_POS)));
    while(!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D3_D4_POS)));

    // Initialize and make sure these are initially OFF
    for(uint8_t i = 0; i < 4; i++) {
        GPIO_Init(LedsPortList[i], LedsPinList[i], &leds);
        GPIO_PinWrite(LedsPortList[i], LedsPinList[i], RESET);
    }
}

void SW_Init(void) {
    // Enable the clock for the ports where the buttons are
    SYSCTL_RCGCGPIO_CMD(SYSCTL_RCGCGPIO_SW_MASK, ENABLE);

    // Configure as digital inputs with a pull-up resistor and interrupt on the falling edge
    GPIO_InitDef_t buttons;
    buttons.ConfigIT = ENABLE;
    buttons.GPIO_PinITConfig.GPIO_PinEdge = GPIO_PinEdge_FALLING;
    buttons.GPIO_PinITConfig.GPIO_PinSense = GPIO_PinSense_EDGE;
    buttons.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_IN;
    buttons.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_PULLUP;

    // Wait until these GPIO ports are ready to be accessed
    while(!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_SW_POS)));
    GPIO_Init(GPIO_SW_PORT, GPIO_SW1_PIN, &buttons);
    GPIO_Init(GPIO_SW_PORT, GPIO_SW2_PIN, &buttons);

    // Clear previous events and enable interrupt generation
    GPIO_PortClearIT(GPIO_SW_PORT);
    NVIC_ClearPendingIRQ(GPIOJ_IRQNumber);
    GPIO_IRQConfig(GPIO_SW_PORT, GPIO_SW1_PIN, ENABLE);
    GPIO_IRQConfig(GPIO_SW_PORT, GPIO_SW2_PIN, ENABLE);

    NVIC_SetPriority(GPIO_SW_IRQNUMBER, DEFAULT_PRIO);
}

void ADC_Init(void) {
    // Enable the clock for ADC0
    SYSCTL_RCGCADC_CMD(SYSCTL_RCGCADC_ADC0_MASK, ENABLE);
    
    // Configure to read the internal temperature sensor, and enable interrupts
    ADC_Sample_t adc;
    adc.Comparator = ADC_Sample_Comparator_Disabled;
    adc.EndOfSequenceCommand = ENABLE;
    adc.HoldWidth = ADC_Sample_HoldWidth16;
    adc.Input = ADC_Sample_Input_Temp;
    adc.ITCommand = ENABLE;
    adc.Sample = ADC_Sample0;
    adc.Sequencer = ADC_Sequencer3;

    // Wait until ADC0 is ready to be accessed
    while(!(SYSCTL_PRADC_READ(SYSCTL_PRADC_ADC0_POS)));

    // Initialize and set triggers based on the timer
    ADC_ConfigSample(ADC0, &adc);
    ADC_ConfigOversampling(ADC0, ADC_Oversampling_4);
    ADC_ConfigTriggerEvent(ADC0, ADC_Sequencer3, ADC_TriggerEvent_Timer);
    ADC_ConfigClockSource(ADC0, ADC_ClockSource_AltClk);
    
    // Start sequencer 0
    ADC_SequencerCommand(ADC0, ENABLE, ADC_Sequencer3);

    // Clear previous events and enable interrupt generation
    ADC_ClearITByMask(ADC0, ADC_ITMask_All);
    NVIC_ClearPendingIRQ(ADC0_Sequence3_IRQNumber);
    ADC_IRQConfig(ADC0, ENABLE, ADC_ITReadPos_Sequencer3);

    NVIC_SetPriority(ADC0_Sequence3_IRQNumber, DEFAULT_PRIO);
}

void Task1B_Init(void) {
    // Start at 12MHz
    PLL_Init(PRESET3);

    // Timers will Use PIOSC to avoid having to change frequency when a switch is pressed
    SYSCTL_ALTCLKCFG &= ~(SYSCTL_ALTCLKCFG_MASK);

    // Configure each peripheral
    Timer0_Init();
    Timer1_Init();
    LED_Init();
    SW_Init();
    ADC_Init();
    LCD_Init();
}

void Task1C_Init(void) {
    // Start at 12MHz
    PLL_Init(PRESET3);

    // Timers will Use PIOSC to avoid having to change frequency when a switch is pressed
    SYSCTL_ALTCLKCFG &= ~(SYSCTL_ALTCLKCFG_MASK);

    // Configure each peripheral
    Timer0_Init();
    Timer1_Init();
    LED_Init();
    ADC_Init();
    LCD_Init();
    Touch_Init();
}

void Timer1A_Handler(void) {
    static uint8_t value = RESET;

    // Clear the IT flag
    TIM_ClearIT(TIM1, TIM_ITReadPos_TimeoutA);

    // Uniformly toggle the first num_leds LEDs
    for(uint8_t i = 0; i < num_leds; i++) {
        GPIO_PinWrite(LedsPortList[i], LedsPinList[i], value);
    }

    // Turn the rest off
    for(uint8_t i = num_leds; i < 4; i++) {
        GPIO_PinWrite(LedsPortList[i], LedsPinList[i], RESET);
    }

    // This is what produces the toggle behavior
    if(value == SET) {
        value = RESET;
    } else {
        value = SET;
    }
}

void GPIOJ_Handler(void) {
    // Check which button triggered the interrupt
    if(GPIO_PinReadITStatus(GPIO_SW_PORT, GPIO_SW1_PIN)) {
        GPIO_PinClearIT(GPIO_SW_PORT, GPIO_SW1_PIN);
        SW1_pressed = SET;
    } else if(GPIO_PinReadITStatus(GPIO_SW_PORT, GPIO_SW2_PIN)) {
        GPIO_PinClearIT(GPIO_SW_PORT, GPIO_SW2_PIN);
        SW2_pressed = SET;
    } else {
        GPIO_PortClearIT(GPIO_SW_PORT);
    }
}



