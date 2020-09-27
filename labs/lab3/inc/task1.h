
#pragma once

#include "common.h"

/**
  * @brief  Reads the temperature sensor and decides on the number
  *         of LEDs that will blink
  * @retval None
  */
void ADC0Sequence3_Handler(void);

/**
  * @brief  Implements the main loop of task 1 of Laboratory #3:
  *         - LEDs blink at a rate of 2Hz based on temperature 
  *         - Pressing SW1 sets the system clock to 12MHz
  *         - Pressing SW2 sets the system clock to 120MHz
  *         * Less than 20 celcius, only LED D1 blinks
  *         * Between 20 and 24 celcius, D1 and D2 blink
  *         * Between 24 and 28 celcius, D1, D2, and D3 blink
  *         * Higher than 28 celcius, D1, D2, D3, and D4 blink
  * @retval None
  */
void Task1(void);

