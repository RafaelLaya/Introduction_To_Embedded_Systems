#include "task1.h"

// Contains a list of the ports where each LED D<i> is connected
// Index <i> corresponds to LED D<i+1>
static GPIO_RegDef_t *LedsPortList[] = {GPIO_D1_PORT, 
                                        GPIO_D2_PORT, 
                                        GPIO_D3_PORT, 
                                        GPIO_D4_PORT};

// Contains a list of the pins associated with each LED
// Index <i> corresponds to LED D<i+1> 
static uint8_t LedsPinList[] = {GPIO_D1_PIN, 
                                GPIO_D2_PIN,
                                GPIO_D3_PIN, 
                                GPIO_D4_PIN};

// Contains a list of the pins associated with each Switch
// Index <i> corresponds to Switch SW<i+1>
static uint8_t SwPinList[] = {GPIO_SW1_PIN, 
                              GPIO_SW2_PIN};

void Delay(void) {
   for (volatile uint32_t i = 0; i < DELAY_IN_LOOPS; i++);
}

void Task1_Part2_Init(void) {
    // Enable the clock for the ports associated to the LEDs. 
    GPIO_PeriphClockCtrl(GPIO_D1_D2_PORT, ENABLE);
    GPIO_PeriphClockCtrl(GPIO_D3_D4_PORT, ENABLE);

    // Pins associated to LEDs will be initialized as Digital Outputs
    GPIO_InitDef_t leds;
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.ConfigIT = DISABLE;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;

    // Make sure the clock has been enabled for each peripheral before
    // attempting to initialize
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D1_D2_POS)));
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D3_D4_POS)));

    // Initialize each LED and make sure each one is OFF 
    for (uint8_t i = 0; i < NUM_LEDS_PART2; i++) {
        GPIO_Init(LedsPortList[i], LedsPinList[i], &leds);
        GPIO_PinWrite(LedsPortList[i], LedsPinList[i], RESET);
    }
}

void Task1_Part2(void) {
    Task1_Part2_Init();
    while (1) {
        // Turn each LED ON in order (D1, D2, D3, D4) with some delay in-between
        for (uint8_t i = 0; i < NUM_LEDS_PART2; i++) {
            GPIO_PinWrite(LedsPortList[i], LedsPinList[i], SET);
            Delay();
        }    

        // Then turn each LED OFF in reverse order, again with some delay in-between
        for (uint8_t i = 0; i < NUM_LEDS_PART2; i++) {
            GPIO_PinWrite(LedsPortList[NUM_LEDS_PART2 - 1 - i], 
                          LedsPinList[NUM_LEDS_PART2 - 1 - i], 
                          RESET);
            Delay();
        }
    }
}

void Task1_Part3_Init(void) {
    // First provide the clock to the ports associated to the LEDs D1 and D2
    // and to the switches SW1 and SW2
    GPIO_PeriphClockCtrl(GPIO_D1_D2_PORT, ENABLE);
    GPIO_PeriphClockCtrl(GPIO_SW_PORT, ENABLE);

    GPIO_InitDef_t leds;
    GPIO_InitDef_t buttons;
  
    // LEDs will be initialized as Digital Outputs
    leds.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_OUT;
    leds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_NOPULL;
    leds.ConfigIT = DISABLE;
  
    // Buttons as Digital Inputs with internal Pull-up resistors
    buttons.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_IN;
    buttons.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_PULLUP;
    buttons.ConfigIT = DISABLE;
    
    // Make sure the clock has been enabled for each peripheral before
    // attempting to initialize
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_D1_D2_POS)));
    while (!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_SW_POS)));

    // Initialize D1 and D2 
    for (uint8_t i = 0; i < NUM_LEDS_PART3; i++) {
        GPIO_Init(LedsPortList[i], LedsPinList[i], &leds);
    }

    // Initialize SW1 and SW2
    for (uint8_t i = 0; i < NUM_SW_PART3; i++) {
        GPIO_Init(GPIO_SW_PORT, SwPinList[i], &buttons);
    }
}

void Task1_Part3(void) {
    Task1_Part3_Init();

    while (1) {
        // Check SW<i> and turn LED<i> ON if the switch is pressed, otherwise OFF
        for (uint8_t i = 0; i < 2; i++) {
            if (!GPIO_PinRead(GPIO_SW_PORT, SwPinList[i])) {
                GPIO_PinWrite(LedsPortList[i], LedsPinList[i], SET);
            } else {
            GPIO_PinWrite(LedsPortList[i], LedsPinList[i], RESET);
            }
        }
    }
}
