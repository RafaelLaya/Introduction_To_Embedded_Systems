
#pragma once

#include "tm4c1294ncpdt.h"
#include "common.h"

// Location and IRQ Number of Switches SW1 and SW2 in our TM4C1294NCPDT board for Task 2B
#define GPIO_SW1_SW2_PORT           GPIOJ
#define GPIO_SW1_PORT               GPIO_SW1_SW2_PORT
#define GPIO_SW2_PORT               GPIO_SW1_SW2_PORT
#define GPIO_SW1_PIN                GPIO_PinNumber_0
#define GPIO_SW2_PIN                GPIO_PinNumber_1
#define SYSCTL_PRGPIO_SW1_SW2_POS   SYSCTL_PRGPIO_PORTJ_POS
#define SYSCTL_PRGPIO_SW1_POS       SYSCTL_PRGPIO_SW1_SW2_POS
#define SYSCTL_PRGPIO_SW2_POS       
#define SYSCTL_RCGCGPIO_SW1_SW2_POS   SYSCTL_PRGPIO_PORTJ_POS
#define SYSCTL_RCGCGPIO_SW1_POS       SYSCTL_PRGPIO_SW1_SW2_POS
#define SYSCTL_RCGCGPIO_SW2_POS       SYSCTL_PRGPIO_SW1_SW2_POS
#define GPIO_SW1_SW2_IRQNUMBER        GPIOJ_IRQNumber

/**
  * @brief  Initializes GPIOs that drive the LEDs in task 2A
  * GPIOs N1, N0, F4, and F0 as Digital Outputs to drive
  * LEDs D1, D2, D3, and D4, respectively.
  * @retval None
  */
void Task2A_Leds_Init(void);

/**
  * @brief  Initializes the timer in task 2A that produces
  * the delay
  * TIM4 as a periodic down-counter with a period of 1Hz
  * if the timer is driven by a 16MHz clock source. TIM4 is
  * configured in 32-bit mode. 
  * @retval None
  */
void Task2A_Timer_Init(void);

/**
  * @brief  Initializes Peripherals utilized in Task 2A
  * 1) GPIOs N1, N0, F4, and F0 as Digital Outputs to drive
  * LEDs D1, D2, D3, and D4, respectively.
  * 2) TIM4 as a periodic down-counter with a period of 1Hz
  * if the timer is driven by a 16MHz clock source. TIM4 is
  * configured in 32-bit mode. This timer produces the animation delay
  * @retval None
  */
void Task2A_Init(void);

/**
  * @brief Sequentially turns LEDs D1, D2, D3, D4 with 
  * a 1 second delay in between. Then subsequently turns
  * the same LEDs off in reverse order.
  * @retval None
  */
void Task2A(void);

/**
  * @brief  Initializes GPIOs that drive the LEDs in task 2B
  * GPIOs N1, and N0 to drive LEDs D1 and D2, respectively.
  * @retval None
  */
 void Task2B_Leds_Init(void);

/**
  * @brief Initializes TIM5 to produce a frequency of 1Hz for blinking LED D1
  * TIM5 is a 32-bit downcounter, used in Task 2B 
  * @retval None
  */
void Task2B_Timer_Init(void);

/**
  * @brief Initializes SW1 and SW2 which will be used in Task 2B
  * for controlling the behaviour of the timer and the LEDs 
  * @retval None
  */
void Task2B_Buttons_Init(void);

/**
  * @brief Initializes the peripherals used in Task 2B
  * 1) TIM5 as a 32-bit downcounter to produce a frequency of 
  * 1Hz for the blinking of LED D1
  * 2) SW1 and SW2 to manipulate the behaviour of the LEDs and
  * the timer
  * @retval None
  */
void Task2B_Init(void);

/**
  * @brief Controls two LEDs based on two switches
  * 1) When SW1 is pressed, the blinking on LED D1 stops and
  * so does the counter. LED D2 becomes ON
  * 2) When SW2 is pressed, the blinking behaviour of D1 
  * returns, the counter starts counting again, and D2 is
  * turned off
  * @retval None
  */
void Task2B(void);

/**
  * @brief  Responsible for turning ON/OFF each LED in task 2A
  * @retval None
  */
void Timer4A_Handler(void);

/**
  * @brief Produces a blinking animation on LED D1 of 1Hz in task 2B
  * @retval None
  */
void Timer5A_Handler(void);

/**
  * @brief Manages the button on Task 2B
  * - When SW1 is pressed, the blinking on LED D1 stops and
  * so does the counter. LED D2 becomes ON
  * - When SW2 is pressed, the blinking behaviour of D1 
  * returns, the counter starts counting again, and D2 is
  * turned off 
  * @retval None
  */
void GPIOJ_Handler(void);

/**
  * @brief Initializes the peripherals used in Task 2C
  * 1) GPIOs L2, L3, L4 to drive the LEDs for the red,
  * yellow and green light, respectively
  * 2) GPIOs L0, and L1 to read start/stop and pedestrian
  * buttons, respectively
  * 3) Timers TIM1, TIM2, of a press on buttons pedestrian, start/stop, respectively.
  * Also Timer TIM3, to measure length of a each state in the FSM
  * @retval None
  */
void Task2C_Init(void);

/**
  * @brief An FSM that uses timer interrupts and buttons to
  * control a traffic-light (task 2C)
  * - Pressing start/stop for two seconds turns the machine ON/OFF.
  * If ON and uninterrupted, it will cycle between GO (green light) and
  * STOP (red light), changing every 5 seconds.
  * - Pressing pedestrian for two seconds during GO (green light), it 
  * will make the machine transition immediately to the WARN state (yellow light), 
  * where it will stay for 5 seconds and then move to STOP state (red light), and 
  * continue with the regular cycle.  
  * @retval None
  */
void Task2C(void);

/**
  * @brief Registers a press when the pedestrian button is held for 2 seconds in task 2C
  * @retval None
  */
void Timer1A_Handler(void);

/**
  * @brief Registers a press of the start/stop button in task 2C when it is held for 2 seconds
  * @retval None
  */
void Timer2A_Handler(void);

/**
  * @brief Requests a state transition in task 2C every 5 seconds after a transition
  * @retval None
  */
void Timer3A_Handler(void);

/**
  * @brief Initializes the GPIOs that drive the LEDs in task 2C
  * - GPIOs L2, L3, L4 to drive the LEDs for the red,
  * yellow and green light, respectively
  * @retval None
  */
void Task2C_Leds_Init(void);

/**
  * @brief Initializes the GPIOs that read the buttons in task 2C
  * - GPIOs L0, and L1 to read start/stop and pedestrian
  * buttons, respectively
  * @retval None
  */
void Task2C_Buttons_Init(void);

/**
  * @brief Initializes the timers used in task 2C
  * - Timers TIM1, TIM2, of a press on buttons pedestrian, start/stop, respectively.
  * Also Timer TIM3, to measure length of a each state in the FSM
  * @retval None
  */
void Task2C_Timers_Init(void);



