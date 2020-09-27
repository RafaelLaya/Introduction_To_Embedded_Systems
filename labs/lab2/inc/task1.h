
#pragma once

#include "tm4c1294ncpdt.h"
#include "common.h"

/**
  * @brief  Initializes Peripherals utilized in Task 1A
  * 1) GPIOs N1, N0, F4, and F0 as Digital Outputs to drive
  * LEDs D1, D2, D3, and D4, respectively.
  * 2) TIM0 as a periodic down-counter with a period of 1Hz
  * if the timer is driven by a 16MHz clock source. TIM0 is
  * configured in 32-bit mode. 
  * @retval None
  */
void Task1A_Init(void);

/**
  * @brief  Initializes GPIOs used in task 1A
  * GPIOs N1, N0, F4, and F0 as Digital Outputs to drive
  * LEDs D1, D2, D3, and D4, respectively.
  * @retval None
  */
void Task1A_GPIO_Init(void);

/**
  * @brief  Initializes the timer in task 1A
  * TIM0 as a periodic down-counter with a period of 1Hz
  * if the timer is driven by a 16MHz clock source. TIM0 is
  * configured in 32-bit mode. 
  * @retval None
  */
void Task1A_TIM_Init(void);

/**
  * @brief Sequentially turns LEDs D1, D2, D3, D4 with 
  * a 1 second delay in between. Then subsequently turns
  * the same LEDs off in reverse order.
  * @retval None
  */
void Task1A(void);

/**
  * @brief  FSM for the Traffic Light Controller
  * @retval None
  */
void Task1B(void);

/**
  * @brief  Initializes Peripherals utilized in task 1B:
  * 1) GPIOL2, L3, and L4 as Digital Outputs to drive the
  * Red, Yellow, and Green LEDs, respectively.
  * 2) GPIOL0 and L1 as Digital Inputs to read the
  * START/STOP and PEDESTRIAN Buttons, respectively.
  * 3) Timers TIM1, TIM2, and TIM3 that are associated
  * to the pedestrian button, the start/stop button, and
  * the transition timer, respectively 
  * @retval None
  */
void Task1B_Init(void);

/**
  * @brief  Initializes the GPIOs that drive the LEDs in task 1B:
  * GPIOL2, L3, and L4 as Digital Outputs to drive the
  * Red, Yellow, and Green LEDs, respectively.
  * @retval None
  */
void Task1B_Leds_Init(void);

/**
  * @brief  Initializes the GPIOs that read the buttons in task 1B:
  * GPIOL0 and L1 as Digital Inputs to read the
  * START/STOP and PEDESTRIAN Buttons, respectively.
  * @retval None
  */
void Task1B_Buttons_Init(void);

/**
  * @brief  Initializes the Timers used in task 1B:
  * Timers TIM1, TIM2, and TIM3 that are associated
  * to the pedestrian button, the start/stop button, and
  * the transition timer, respectively 
  * @retval None
  */
void Task1B_Timers_Init(void);

