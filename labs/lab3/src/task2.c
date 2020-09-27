
#include "task2.h"

#include <string.h>
#include <stdio.h>

static uint8_t PrintRequested = RESET;
static float temp = 0.0;
static uint8_t buffer[30];

#if (CHOSEN == TASK2_1)
void ADC0Sequence3_Handler(void) {
    // Clear the IT Flag
    ADC_ClearIT(ADC0, ADC_ITReadPos_Sequencer3);

    // take the sample and decode the temperature
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

    PrintRequested = SET;
}
#endif

void UART0_TxRx_Init(void) {
    // Initialize the Tx/Rx pins of the UART for task 2.1
    GPIO_InitDef_t gpio;
    gpio.ConfigIT = DISABLE;
    gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_ALTFN1;
    gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_PULLUP;
    
    // Enable the clock for GPIOA
    SYSCTL_RCGCGPIO_CMD(SYSCTL_RCGCGPIO_PORTA_MASK, ENABLE);

    // Wait until port A is ready
    while(!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_PORTA_POS)));

    // Initialize
    GPIO_Init(GPIOA, GPIO_PinNumber_0, &gpio);
    GPIO_Init(GPIOA, GPIO_PinNumber_1, &gpio);
}

void UART0_Init(void) {
    UART0_TxRx_Init();

    // Configure UART0 with a baud rate of 115200 bits/s, no parity, 1 stop-bit, and 8-bits of data
    UART_Config_t uart;
    uart.BaudRate = UART0_BaudRate;
    uart.BreakCommand = DISABLE;
    uart.ClockFreq = PIOSC_Freq;
    uart.ClockSource = UART_ClockSource_ALTCLK;
    uart.FifoCommand = DISABLE;
    uart.HardwareCTS = DISABLE;
    uart.HardwareRTS = DISABLE;
    uart.HighSpeedCommand = DISABLE;
    uart.LoopbackCommand = DISABLE;
    uart.Parity = UART_Parity_Disabled;
    uart.StopBits = UART_StopLen_1;
    uart.WordLen = UART_WordLen_8;

    // Enable the clock and wait until the peripheral is ready
    SYSCTL_RCGCUART_CMD(SYSCTL_RCGCUART_UART0_MASK, ENABLE);
    while(!(SYSCTL_PRUART_READ(SYSCTL_PRUART_UART0_POS)));

    // Configure and enable
    UART_Config(UART0, &uart);
    UART_Command(UART0, ENABLE);
}

void Task2_1_Init(void) {
    // Initialize the common peripherals and then UART0
    Task_Common_Init();
    UART0_Init();
}

void UART3_TxRx_Init(void) {
    // Initialize the Tx/Rx pins of Task 2.2
    GPIO_InitDef_t gpio;
    gpio.ConfigIT = DISABLE;
    gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_PinMode_ALTFN1;
    gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PinPuPdControl_PULLUP;
    
    // Enable the clock for port A and wait until it is ready to be accessed
    SYSCTL_RCGCGPIO_CMD(SYSCTL_RCGCGPIO_PORTA_MASK, ENABLE);
    while(!(SYSCTL_PRGPIO_READ(SYSCTL_PRGPIO_PORTA_POS)));

    // Initialize
    GPIO_Init(GPIOA, GPIO_PinNumber_4, &gpio);
    GPIO_Init(GPIOA, GPIO_PinNumber_5, &gpio);
}

void UART3_Init(void) {
    // Initialize the Tx/Rx pins
    UART3_TxRx_Init();

    // Configure with baud rate of 9600 bits/s, no parity, 1 stop bit, 8 bits of data
    UART_Config_t uart;
    uart.BaudRate = Bluetooth_Baudrate;
    uart.BreakCommand = DISABLE;
    uart.ClockFreq = PIOSC_Freq;
    uart.ClockSource = UART_ClockSource_ALTCLK;
    uart.FifoCommand = DISABLE;
    uart.HardwareCTS = DISABLE;
    uart.HardwareRTS = DISABLE;
    uart.HighSpeedCommand = DISABLE;
    uart.LoopbackCommand = DISABLE;
    uart.Parity = UART_Parity_Disabled;
    uart.StopBits = UART_StopLen_1;
    uart.WordLen = UART_WordLen_8;

    // Enable the clock and wait until the peripheral is ready
    SYSCTL_RCGCUART_CMD(SYSCTL_RCGCUART_UART3_MASK, ENABLE);
    while(!(SYSCTL_PRUART_READ(SYSCTL_PRUART_UART3_POS)));

    // Configure and enable
    UART_Config(UART3, &uart);
    UART_Command(UART3, ENABLE);
}

void Task2_2_Init(void) {
    // Initialize UART3 connected to the HC-06 module
    UART3_Init();
}

void Task2_1(void) {
    // Initialize peripherals used in task 2.1
    Task2_1_Init();

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
        
        // Wait until a button is pressed or until we have to print
        while (!SW1_pressed && !SW2_pressed && !PrintRequested);
        
        if (PrintRequested) {
            // This should never be a problem since the ADC is triggered once per second
            // But to be safe, make a critical section to avoid corruption of temp
            __asm volatile("cpsid i\t\n");
            sprintf((char *) buffer, "Temperature in celcius: %.2f\r\n", temp);
            __asm volatile("cpsie i\t\n");

            UART_SendData(UART0, buffer, strlen((char *) buffer));
            PrintRequested = RESET;
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

void Task2_2(void) {
    // Initialize the peripherals used in task 2.2 (which is UART3)
    Task2_2_Init();

    // Buffer for a single byte of data (one char)
    uint8_t buffer;
    while(1) {
        // Wait until a char is received
        UART_ReceiveDataSimple(UART3, &buffer, 1);

        // Return that char to the sender
        UART_SendData(UART3, &buffer, 1);
    }
}