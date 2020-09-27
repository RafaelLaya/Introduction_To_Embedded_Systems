
#pragma once
#include "tm4c1294ncpdt.h"

// Contains all possible states of this FSM
typedef enum {
    IDLE,
    STOP,
    GO,
    WARN
} TL_states_e;

// Definitions for the Ports and Pins used in task 2
#define TL_Port                   GPIOL
#define SYSCTL_TL_PORT_MASK       SYSCTL_RCGCGPIO_PORTL_MASK
#define START_STOP_PIN            GPIO_PinNumber_0
#define PEDESTRIAN_PIN            GPIO_PinNumber_1
#define RED_PIN                   GPIO_PinNumber_2
#define YELLOW_PIN                GPIO_PinNumber_3
#define GREEN_PIN                 GPIO_PinNumber_4

// Defines how many 'ticks' a state should last 
#define STATE_LENGTH_IN_TICKS       100000UL

/**
  * @brief  FSM for the Traffic Light Controller
  * @retval None
  */
void Task2(void);

/**
  * @brief  Initializes Peripherals utilized in task 2:
  * GPIOL2, L3, and L4 as Digital Outputs to drive the
  * Red, Yellow, and Green LEDs, respectively.
  * GPIOL0 and L1 as Digital Inputs to read the
  * START/STOP and PEDESTRIAN Buttons, respectively. 
  * @retval None
  */
void Task2_Init(void);
