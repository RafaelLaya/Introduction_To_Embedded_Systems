
#pragma once

// All possibles states of the FSM 
typedef enum {
    IDLE,
    STOP,
    GO,
    WARN
} TL_states_e;

// Macro for converting seconds to count and seconds to ms
#define OSCILLATOR_FREQ                         16000000UL
#define SECONDS_TO_COUNT(SECONDS, FREQ)         ((SECONDS) * (FREQ))
#define SEC_TO_MS(SECONDS)			((SECONDS) * 1000)

// Register definition for alternate clock configuration 
#define SYSCTL_ALTCLKCFG_OFFSET     0x138U
#define SYSCTL_ALTCLKCFG            *((volatile uint32_t *) (SYSCTL_BASE + SYSCTL_ALTCLKCFG_OFFSET))
#define SYSCTL_ALTCLKCFG_MASK       0xFU

// Timers used 
#define PEDESTRIAN_BUTTON_TIMER         TIM1
#define PEDESTRIAN_BUTTON_TIMER_MASK    SYSCTL_RCGCTIMER_TIM1_MASK   
#define PEDESTRIAN_BUTTON_TIMER_POS     SYSCTL_PRTIMER_TIM1_POS  
#define START_STOP_BUTTON_TIMER         TIM2
#define START_STOP_BUTTON_TIMER_MASK    SYSCTL_RCGCTIMER_TIM2_MASK  
#define START_STOP_BUTTON_TIMER_POS     SYSCTL_PRTIMER_TIM2_POS 
#define TRANSITION_TIMER                TIM3
#define TRANSITION_TIMER_MASK           SYSCTL_RCGCTIMER_TIM3_MASK  
#define TRANSITION_TIMER_POS            SYSCTL_PRTIMER_TIM3_POS 

// Boundary for the buttons
#define START_STOP_LEFT_LIMIT           1000
#define START_STOP_RIGHT_LIMIT          1400
#define START_STOP_BOTTOM_LIMIT         1200
#define START_STOP_TOP_LIMIT            1500
#define PED_LEFT_LIMIT                  1550
#define PED_RIGHT_LIMIT                 1840
#define PED_BOT_LIMIT                   1200
#define PED_TOP_LIMIT                   1500

// Position of the buttons
#define START_STOP_X                    100
#define START_STOP_Y                    120
#define PED_X                           225
#define PED_Y                           120

// Position of the lights
#define RED_X                           80
#define RED_Y                           175
#define GREEN_X                         163
#define GREEN_Y                         175
#define YELLOW_X                        245
#define YELLOW_Y                        175

// Colors
#define START_STOP_COLOR                Color4[0]
#define PED_COLOR                       Color4[6]
#define BACKGROUND_COLOR                Color4[3]
#define RED_COLOR                       convertColor(255, 0, 0)
#define GREEN_COLOR                     convertColor(0, 255, 0)
#define YELLOW_COLOR                    convertColor(255, 255, 0)

// Labels
#define START_STOP_LABEL_X              75
#define START_STOP_LABEL_Y              90
#define PED_LABEL_X                     195
#define PED_LABEL_Y                     90

#define BUTTON_PRESS_IN_S               2UL
#define TRANSITION_TIMEOUT_IN_S         5UL


// Radius of buttons and lights
#define RADIUS                          20

/**
  * @brief  FSM for the Traffic Light Controller
  * @retval None
  */
void Task2A(void);

/**
  * @brief  Initializes Peripherals utilized in task 2A:
  * 1) The LCD screen and touch functions using ADC1 and the following GPIOs:
  *        * Data:    D2, K5, M7, P0-1, Q0, Q2-3
  *        * Control: N4-5, P3-4
  * 2) Timers TIM1, TIM2, and TIM3 that are associated
  * to the pedestrian button, the start/stop button, and
  * the transition timer, respectively 
  * @retval None
  */
void Task2A_Init(void);

/**
  * @brief  Initializes the Timers used in task 2A:
  * Timers TIM1, TIM2, and TIM3 that are associated
  * to the pedestrian button, the start/stop button, and
  * the transition timer, respectively 
  * @retval None
  */
void Task2A_Timers_Init(void);


