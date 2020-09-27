
#include "common.h"

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

void Task_Common_Init(void) {
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



