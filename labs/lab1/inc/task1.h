#pragma once

#include "tm4c1294ncpdt.h"

// Number of LEDs used in Parts 2 and Part 3
#define NUM_LEDS                4U
#define NUM_LEDS_PART2          NUM_LEDS
#define NUM_LEDS_PART3          2U

// Number of switches used in Part 3
#define NUM_SW                  2U
#define NUM_SW_PART3            NUM_SW

// Delay in units of 'loops'
#define DELAY_IN_LOOPS          1000000UL

#define SYSCTL_PRGPIO_D1_D2_POS           SYSCTL_PRGPIO_PORTN_POS
#define SYSCTL_PRGPIO_D3_D4_POS           SYSCTL_PRGPIO_PORTF_POS
#define SYSCTL_PRGPIO_SW_POS              SYSCTL_PRGPIO_PORTJ_POS

/**
  * @brief  Initializes Peripherals utilized in Part 2 of Task1:
  * GPIOs N1, N0, F4, and F0 as Digital Outputs to drive
  * LEDs D1, D2, D3, and D4, respectively.
  * @retval None
  */
void Task1_Part2_Init(void);

/**
  * @brief Sequentially turns LEDs D1, D2, D3, D4 with 
  * a small delay in between. Then subsequently turns
  * the same LEDs off in reverse order.
  * @retval None
  */
void Task1_Part2(void);

/**
  * @brief  Initializes peripherals utilized in Part 2 of Task 1: 
  * - GPIOs N1, N0 as Digital Outputs to drive 
  *   LEDs D1, and D2, respectively.
  * - GPIOs J0, J1 as Digital Inputs with internal Pull-up resistros
  *   to read switches SW1 and SW2, respectively.
  * @retval None
  */
void Task1_Part3_Init(void);

/*
 *  Turns ON LED D1 whenever switch SW1 is pressed.
 *  Turns ON LED D2 whenever switch SW2 is pressed.
 */
/**
  * @brief  Turns ON LED D1 only when SW1 is pressed.
  *         Turns ON LED D2 only when SW2 is pressed. 
  * @retval None
  */
void Task1_Part3(void);

/**
  * @brief  Keeps the CPU busy to provide a small delay.
  * @retval None
  */
void Delay(void);
