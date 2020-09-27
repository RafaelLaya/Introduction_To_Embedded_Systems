#pragma once

#include "choose_task.h"
#include "tm4c1294ncpdt.h"
#include "clock.h"

/*
 * Extern global variables used in Task 1 and 2.1 
 * These are initialized in common.c
 */
extern volatile uint8_t num_leds; 
extern GPIO_RegDef_t *LedsPortList[];
extern uint8_t LedsPinList[];
extern volatile uint8_t SW2_pressed;
extern volatile uint8_t SW1_pressed;

// Possible settings in task 1 and 2.1
typedef enum {
    TM_SLOW,
    TM_FAST
} TM_states_e;

// Constants related to frequency or period for Task 1 and 2.1
#define PIOSC_FREQ                              16000000UL
#define SECONDS_TO_COUNT(SECONDS, FREQ)         ((SECONDS) * (FREQ))
#define BLINKING_PERIOD_IN_COUNTS               8000000UL
#define ADC_TRIGGER_PERIOD                      1UL
#define LED_BLINK_PERIOD                        2UL    
#define DEFAULT_PRIO                            5U

// Register definition for alternate clock configuration for task 1 and 2.1
#define SYSCTL_ALTCLKCFG_OFFSET     0x138U
#define SYSCTL_ALTCLKCFG            *((volatile uint32_t *) (SYSCTL_BASE + SYSCTL_ALTCLKCFG_OFFSET))
#define SYSCTL_ALTCLKCFG_MASK       0xFU

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
  * @brief  Initializes the peripherals required in task 1 and 2.1:
  *         - GPIOs N1, N0, F4, and F0 as digital outputs
  *           to drive the on-board LEDs D1, D2, D3, and D4,
  *           respectively.
  *         - Timer0 to trigger an ADC conversion every second
  *         - Timer1 to blink LEDs each 0.5 seconds
  *         - ADC0 to get triggered by Timer0 to read the temperature sensor
  *         - GPIOs J0 and J1 as digital inputs to read switches SW1 and SW2
  * @retval None
  */
void Task_Common_Init(void);

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

#pragma once
