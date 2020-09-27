
#pragma once

#include "tm4c1294ncpdt.h"

// All possibles states of the FSM in Task 1B and Task 2C
typedef enum {
    IDLE,
    STOP,
    GO,
    WARN
} TL_states_e;

// Number of LEDs used in Task 1A and 2A
#define NUM_LEDS                    4U

// Macro for converting secons to count
#define SECONDS_TO_COUNT(SECONDS)       ((SECONDS) * OSCILLATOR_FREQ)

// Location of LEDs D1, D2, D3, and D4 in our TM4C1294NCPDT board for Task 1A, 2A, and 2B
#define GPIO_D1_D2_PORT              GPION
#define GPIO_D1_PORT                 GPIO_D1_D2_PORT
#define GPIO_D2_PORT                 GPIO_D1_D2_PORT
#define GPIO_D3_D4_PORT              GPIOF
#define GPIO_D3_PORT                 GPIO_D3_D4_PORT
#define GPIO_D4_PORT                 GPIO_D3_D4_PORT
#define GPIO_D1_PIN                  GPIO_PinNumber_1
#define GPIO_D2_PIN                  GPIO_PinNumber_0
#define GPIO_D3_PIN                  GPIO_PinNumber_4
#define GPIO_D4_PIN                  GPIO_PinNumber_0
#define SYSCTL_PRGPIO_D1_D2_POS      SYSCTL_PRGPIO_PORTN_POS  
#define SYSCTL_PRGPIO_D3_D4_POS      SYSCTL_PRGPIO_PORTF_POS

// Definitions for the Ports and Pins used in Task 1B and 1C
#define TL_Port                   GPIOL
#define SYSCTL_TL_PORT_POS        SYSCTL_PRGPIO_PORTL_POS
#define SYSCTL_TL_PORT_MASK       SYSCTL_RCGCGPIO_PORTL_MASK
#define START_STOP_PIN            GPIO_PinNumber_0
#define PEDESTRIAN_PIN            GPIO_PinNumber_1
#define RED_PIN                   GPIO_PinNumber_2
#define YELLOW_PIN                GPIO_PinNumber_3
#define GREEN_PIN                 GPIO_PinNumber_4

// Timers used in Task 1B and 1C
#define PEDESTRIAN_BUTTON_TIMER         TIM1
#define PEDESTRIAN_BUTTON_TIMER_MASK    SYSCTL_RCGCTIMER_TIM1_MASK   
#define PEDESTRIAN_BUTTON_TIMER_POS     SYSCTL_PRTIMER_TIM1_POS 
#define PEDESTRIAN_BUTTON_IRQ           TIM1A_IRQNumber   
#define START_STOP_BUTTON_TIMER         TIM2
#define START_STOP_BUTTON_TIMER_MASK    SYSCTL_RCGCTIMER_TIM2_MASK  
#define START_STOP_BUTTON_TIMER_POS     SYSCTL_PRTIMER_TIM2_POS 
#define START_STOP_BUTTON_IRQ           TIM2A_IRQNumber
#define TRANSITION_TIMER                TIM3
#define TRANSITION_TIMER_MASK           SYSCTL_RCGCTIMER_TIM3_MASK  
#define TRANSITION_TIMER_POS            SYSCTL_PRTIMER_TIM3_POS 
#define TRANSITION_TIMER_IRQ            TIM3A_IRQNumber

// Frequency of the oscillator used in this laboratory
#define OSCILLATOR_FREQ             16000000UL
