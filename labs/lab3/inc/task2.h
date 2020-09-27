#pragma once

#include "common.h"

// Frequencies for UART0 and UART3 (for tasks 2.1 and 2.2, respectively)
#define UART0_BaudRate                  115200UL
#define Bluetooth_Baudrate              9600UL
#define PIOSC_Freq                      16000000UL

/**
  * @brief  Implements the main loop of task 2.1 of Laboratory #3:
  *         - LEDs blink at a rate of 2Hz based on temperature
  *         - The temperature is sent through UART to the ICDI  
  *         - Pressing SW1 sets the system clock to 12MHz
  *         - Pressing SW2 sets the system clock to 120MHz
  *         * Less than 20 celcius, only LED D1 blinks
  *         * Between 20 and 24 celcius, D1 and D2 blink
  *         * Between 24 and 28 celcius, D1, D2, and D3 blink
  *         * Higher than 28 celcius, D1, D2, D3, and D4 blink
  * @retval None
  */
void Task2_1(void);

/**
  * @brief  Implements task 2.2 of laboratory #3:
  *         - UART Return-to-Sender feature, using an HC-06 bluetooth module
  * @retval None
  */
void Task2_2(void);

/**
  * @brief  Reads the temperature sensor and decides on the number
  *         of LEDs that will blink. Also requests the temperature be
  *         sent through UART
  * @retval None
  */
void ADC0Sequence3_Handler(void);

/**
  * @brief  Initializes GPIO pins A0 and A1 as Rx and Tx (respectively) for 
  *         UART communication (UART0)
  * @retval None
  */
void UART0_TxRx_Init(void);

/**
  * @brief  Initializes UART0 for communication (to send temperature)
  * @retval None
  */
void UART0_Init(void);

/**
  * @brief  Initializes peripherals used in Task 2.1:
  *         - GPIOs N1, N0, F4, and F0 as digital outputs
  *           to drive the on-board LEDs D1, D2, D3, and D4,
  *           respectively.
  *         - Timer0 to trigger an ADC conversion every second
  *         - Timer1 to blink LEDs each 0.5 seconds
  *         - ADC0 to get triggered by Timer0 to read the temperature sensor
  *         - GPIOs J0 and J1 as digital inputs to read switches SW1 and SW2
  *         - UART0 for communication (to send temperature)
  *         - GPIOs A0, A1, for UART communication as Rx and Tx pins (respectively)
  * @retval None
  */
void Task2_1_Init(void);

/**
  * @brief  Initializes GPIO pins A4 and A5 as Rx and Tx (respectively) for 
  *         UART communication (UART3)
  * @retval None
  */
void UART3_TxRx_Init(void);

/**
  * @brief  Initializes UART3 for communication (for the Return-to-Sender feature)
  * @retval None
  */
void UART3_Init(void);

/**
  * @brief  Initializes the peripherals for task 2.2:
  *         - UART3 for communication (for the Return-to-Sender feature)
  *         - GPIO Pins A4, A5 as Rx and Tx (respectively) for UART communication
  *           through UART3
  * @retval None
  */
void Task2_2_Init(void);

