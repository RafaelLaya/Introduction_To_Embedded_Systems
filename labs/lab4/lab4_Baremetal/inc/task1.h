
#pragma once

#include "tm4c1294ncpdt.h"
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "clock.h"

// Possible settings in task 1B
typedef enum {
    TM_SLOW,
    TM_FAST
} TM_states_e;

// Constants related to frequency or period for tasks 1B and 1C
#define PIOSC_FREQ                              16000000UL
#define SECONDS_TO_COUNT(SECONDS, FREQ)         ((SECONDS) * (FREQ))
#define BLINKING_PERIOD_IN_COUNTS               8000000UL
#define ADC_TRIGGER_PERIOD                      1UL   
#define DEFAULT_PRIO                            5U

// Position of buttons
#define FAST_X                      100
#define FAST_Y                      120
#define SLOW_X                      225
#define SLOW_Y                      120
#define RADIUS                      20 

// Position of button labels
#define FAST_LABEL_X                90
#define FAST_LABEL_Y                115
#define SLOW_LABEL_X                215
#define SLOW_LABEL_Y                115

// Button limits
#define FAST_LEFT_LIMIT             1000
#define FAST_RIGHT_LIMIT            1400
#define FAST_BOT_LIMIT              1200
#define FAST_TOP_LIMIT              1500
#define SLOW_LEFT_LIMIT             1550
#define SLOW_RIGHT_LIMIT            1840
#define SLOW_BOT_LIMIT              1200
#define SLOW_TOP_LIMIT              1500

// Colors
#define BACKGROUND_COLOR            Color4[3]
#define FAST_COLOR                  convertColor(255, 0, 0)
#define SLOW_COLOR                  convertColor(0, 255, 0)

// Register definition for alternate clock configuration for task 1B and 1C
#define SYSCTL_ALTCLKCFG_OFFSET     0x138U
#define SYSCTL_ALTCLKCFG            *((volatile uint32_t *) (SYSCTL_BASE + SYSCTL_ALTCLKCFG_OFFSET))
#define SYSCTL_ALTCLKCFG_MASK       0xFU

// Celcius to Farenheit Conversion
#define CELCIUS_TO_FARENH(C)        ((C) * 1.8 + 32.0)

// ADC value to Celcius
#define ADC_TO_CELCIUS(A)           (147.5 - ((A) * 247.5 / 4096.0))

/**
  * @brief  Initializes Timer0 as a periodic downcounter
  *         which triggers an ADC conversion every second
  * @retval None
  */
void Timer0_Init(void);

/**
  * @brief  Initializes Timer1, which is a periodic downcounter
  *         each half a second to blink the LEDs
  * @retval None
  */
void Timer1_Init(void);

/**
  * @brief  Initializes GPIOs N1, N0, F4, F0 as digital outputs
  *         to drive the on-board LEDs D1, D2, D3, and D4, 
  *         respectively
  * @retval None
  */
void LED_Init(void);

/**
  * @brief  Initializes GPIOs J0 and J1 as digital inputs 
  *         with pull-up resistors to read switches SW1 and SW2
  * @retval None
  */
void SW_Init(void);

/**
  * @brief  Initializes ADC0 to get triggered by Timer0 
  *         to read the on-board temperature sensor
  * @retval None
  */
void ADC_Init(void);

/**
  * @brief  Initializes the peripherals required in task 1B
  *         - GPIOs N1, N0, F4, and F0 as digital outputs
  *           to drive the on-board LEDs D1, D2, D3, and D4,
  *           respectively.
  *         - Timer0 to trigger an ADC conversion every second
  *         - Timer1 to blink LEDs each 0.5 seconds
  *         - ADC0 to get triggered by Timer0 to read the temperature sensor
  *         - GPIOs J0 and J1 as digital inputs to read switches SW1 and SW2
  *         - The LCD screen with the following GPIOs:
  *             * Data:    D2, K5, M7, P0-1, Q0, Q2-3
  *             * Control: N4-5, P3-4
  * @retval None
  */
void Task1B_Init(void);

/**
  * @brief  Initializes the peripherals required in task 1C
  *         - GPIOs N1, N0, F4, and F0 as digital outputs
  *           to drive the on-board LEDs D1, D2, D3, and D4,
  *           respectively.
  *         - Timer0 to trigger an ADC conversion every second
  *         - Timer1 to blink LEDs each 0.5 seconds
  *         - ADC0 to get triggered by Timer0 to read the temperature sensor
  *         - The LCD screen and touch functions using ADC1 and the following GPIOs:
  *             * Data:    D2, K5, M7, P0-1, Q0, Q2-3
  *             * Control: N4-5, P3-4
  * @retval None
  */
void Task1C_Init(void);

/**
  * @brief  Toggles LEDs D0, ..., D[num_leds - 1] and turns
  *         the rest OFF
  * @retval None
  */
void Timer1A_Handler(void);

/**
  * @brief  Registers a button press on SW1 or SW2
  * @retval None
  */
void GPIOJ_Handler(void);

/**
  * @brief  Implements the main loop of task 1B:
  *         - LEDs blink at a rate of 2Hz based on temperature
  *         - Pressing SW1 sets the system clock to 12MHz
  *         - Pressing SW2 sets the system clock to 120MHz
  *         * Less than 20 celcius, only LED D1 blinks
  *         * Between 20 and 24 celcius, D1 and D2 blink
  *         * Between 24 and 28 celcius, D1, D2, and D3 blink
  *         * Higher than 28 celcius, D1, D2, D3, and D4 blink
  * @retval None
  */
void Task1B(void);

/**
  * @brief  Implements the main loop of task 1C:
  *         - LEDs blink at a rate of 2Hz based on temperature
  *         - Pressing the green button sets the system clock to 12MHz
  *         - Pressing the red button sets the system clock to 120MHz
  *         * Less than 20 celcius, only LED D1 blinks
  *         * Between 20 and 24 celcius, D1 and D2 blink
  *         * Between 24 and 28 celcius, D1, D2, and D3 blink
  *         * Higher than 28 celcius, D1, D2, D3, and D4 blink
  * @retval None
  */
void Task1C(void);

/**
  * @brief  Reads the temperature sensor and decides on the number
  *         of LEDs that will blink. Also requests the temperature be
  *         printed to the display
  * @retval None
  */
void ADC0Sequence3_Handler(void);

