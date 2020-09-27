
#include "tm4c1294ncpdt.h"
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "task2.h"


void Task2A_Timers_Init(void) {
    // Enable the clock of the timers utilized in this task
    TIM_PeriphClockCtrlByMask(PEDESTRIAN_BUTTON_TIMER_MASK, ENABLE);
    TIM_PeriphClockCtrlByMask(START_STOP_BUTTON_TIMER_MASK, ENABLE);
    TIM_PeriphClockCtrlByMask(TRANSITION_TIMER_MASK, ENABLE);

    // Configure as periodic down counters
    TIM_Init_t timers;
    timers.PeriodicModeCommand = ENABLE;
    timers.TIM_CompareAction = TIM_CompareAction_Disable;
    timers.TIM_Direction = TIM_Direction_DOWN;
    timers.TIM_SnapshotCommand = DISABLE;
    timers.TIM_WaitOnTriggerCommand = DISABLE;

    // wait until these are ready to be accessed
    while(!(SYSCTL_PRTIMER_READ(PEDESTRIAN_BUTTON_TIMER_POS)));
    while(!(SYSCTL_PRTIMER_READ(START_STOP_BUTTON_TIMER_POS)));
    while(!(SYSCTL_PRTIMER_READ(TRANSITION_TIMER_POS)));

    TIM_ConfigAltClk(PEDESTRIAN_BUTTON_TIMER, ENABLE);
    TIM_ConfigAltClk(START_STOP_BUTTON_TIMER, ENABLE);
    TIM_ConfigAltClk(TRANSITION_TIMER, ENABLE);

    TIM_Init(PEDESTRIAN_BUTTON_TIMER, &timers, TIM_Port_Concatenated);
    TIM_Init(TRANSITION_TIMER, &timers, TIM_Port_Concatenated);
    TIM_Init(START_STOP_BUTTON_TIMER, &timers, TIM_Port_Concatenated);

    // the buttons must be pressed for two seconds for the system to register 
    // the press
    TIM_ConfigIntervalConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));
    TIM_ConfigIntervalConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));

    // A transition normally occurs every 5 seconds
    TIM_ConfigIntervalConc(TRANSITION_TIMER, SECONDS_TO_COUNT(TRANSITION_TIMEOUT_IN_S, OSCILLATOR_FREQ));

    // Clear any previous events
    TIM_ClearITAll(PEDESTRIAN_BUTTON_TIMER);
    TIM_ClearITAll(START_STOP_BUTTON_TIMER);
    TIM_ClearITAll(TRANSITION_TIMER);
}

void Task2A_Init(void) {
    SYSCTL_ALTCLKCFG &= ~(SYSCTL_ALTCLKCFG_MASK);
    Task2A_Timers_Init();

    LCD_Init();
    Touch_Init();
}

void Task2A(void) {
    Task2A_Init();

    TL_states_e present_state = IDLE;
    TL_states_e next_state = IDLE;  

    // Software will keep track of the previous and new values to detect rising edges
    // But must only register inputs after a 2-second press, thus the 'tmp' suffix-ed variables
    // represent a button press that hasn't been held for 2 seconds yet
    uint8_t startStop_prev = RESET;
    uint8_t startSTop_new = RESET;
    uint8_t startStop_tmp = RESET;
    uint8_t startStop_pressed = RESET;

    uint8_t pedestrian_prev = RESET;
    uint8_t pedestrian_new = RESET;
    uint8_t pedestrian_tmp = RESET;
    uint8_t pedestrian_pressed = RESET;

    uint8_t transition_requested = RESET;

    // Since drawing a filled circle on the screen is time-consuming, we'll keep track of 
    // which light is On/Off, so that we don't draw an already drawn circle
    uint8_t RedLightOn, GreenLightOn, YellowLightOn;
    RedLightOn = RESET;
    GreenLightOn = RESET;
    YellowLightOn = RESET;

    // x and y coordinates of a touch
    static unsigned long x = 0;
    static unsigned long y = 0;

    // Draw initial state of the screen
    LCD_ColorFill(BACKGROUND_COLOR);
    LCD_SetTextColor(255, 255, 255);
    LCD_SetCursor(0, 0);

    // Draw buttons
    LCD_DrawFilledCircle(START_STOP_X, START_STOP_Y, RADIUS, START_STOP_COLOR);
    LCD_DrawFilledCircle(PED_X, PED_Y, RADIUS, PED_COLOR);

    // Label buttons
    LCD_SetCursor(START_STOP_LABEL_X, START_STOP_LABEL_Y);
    LCD_PrintString("Start/Stop");
    LCD_SetCursor(PED_LABEL_X, PED_LABEL_Y);
    LCD_PrintString("Pedestrian");
    LCD_SetCursor(0, 0);

    // Start the timer
    TIM_Command(TRANSITION_TIMER, ENABLE, TIM_Port_Concatenated);

    while (1) {
        // Read the buttons
        x = Touch_ReadX();
        y = Touch_ReadY();

        // Check if any of the buttons is being pressed
        if (START_STOP_LEFT_LIMIT < x  && x < START_STOP_RIGHT_LIMIT && 
           START_STOP_BOTTOM_LIMIT < y && y < START_STOP_TOP_LIMIT) {
            startSTop_new = SET;
        } else {
            startSTop_new = RESET;
        }

        if (PED_LEFT_LIMIT < x && x < PED_RIGHT_LIMIT && 
            PED_BOT_LIMIT < y && y < PED_TOP_LIMIT)     {
            pedestrian_new = SET;
        } else {
            pedestrian_new = RESET;
        }

        // Check for rising edges
        if (!startStop_prev && startSTop_new) {
            startStop_tmp = SET;
        } else if(startStop_prev && !startSTop_new) {
            startStop_tmp = RESET;
        }

        if (!pedestrian_prev && pedestrian_new) {
            pedestrian_tmp = SET; 
        } else if (pedestrian_prev && !pedestrian_new) {
            pedestrian_tmp = RESET;
        }

        // start counting if the button gets pressed, and restart the count if the button is not pressed
        // for 2-seconds straight
        if (pedestrian_tmp && !TIM_ReadCounterStatus(PEDESTRIAN_BUTTON_TIMER, TIM_Port_Concatenated)) {
            TIM_Command(PEDESTRIAN_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
        } else if (!pedestrian_tmp) {
            TIM_Command(PEDESTRIAN_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));
        }

        // Exactly as above
        if (startStop_tmp && !TIM_ReadCounterStatus(START_STOP_BUTTON_TIMER, TIM_Port_Concatenated)) {
            TIM_Command(START_STOP_BUTTON_TIMER, ENABLE, TIM_Port_Concatenated);
        } else if (!startStop_tmp) {
            TIM_Command(START_STOP_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_LoadCountConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));
        }

        // If a timer has expired, register the input
        if (TIM_ReadRawITStatus(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA)) {
            pedestrian_pressed = SET;
            TIM_Command(PEDESTRIAN_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_ClearIT(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));
        }

        if (TIM_ReadRawITStatus(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA)) {
            startStop_pressed = SET;
            TIM_Command(START_STOP_BUTTON_TIMER, DISABLE, TIM_Port_Concatenated);
            TIM_ClearIT(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_LoadCountConc(START_STOP_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));
        }

        // next-state logic
        switch (present_state) {
            case IDLE:
                if (startStop_pressed) {
                    next_state = STOP;
                    transition_requested = SET;
                } else {
                    next_state = IDLE;
                }
                break;
            case STOP:
                if (startStop_pressed) {
                    next_state = IDLE;
                    transition_requested = SET;
                } else {
                    next_state = GO;
                }
                break;
            case GO:
                if (startStop_pressed) {
                    next_state = IDLE;
                    transition_requested = SET;
                } else if (pedestrian_pressed) {
                    transition_requested = SET;
                    next_state = WARN;
                } else {
                    next_state = STOP;
                }
                break;
            case WARN:
                if (startStop_pressed) {
                    next_state = IDLE;
                    transition_requested = SET;
                } else {
                    next_state = STOP;
                }
                break;
            default:
                next_state = IDLE;
                break;
        }

        // output logic (Moore Type)
        switch (present_state) {
            case IDLE:
                if(RedLightOn) {
                    LCD_DrawFilledCircle(RED_X, RED_Y, RADIUS, BACKGROUND_COLOR);
                    RedLightOn = RESET;
                } 
                if(GreenLightOn) {
                    LCD_DrawFilledCircle(GREEN_X, GREEN_Y, RADIUS, BACKGROUND_COLOR);
                    GreenLightOn = RESET;
                }
                if(YellowLightOn) {
                    LCD_DrawFilledCircle(YELLOW_X, YELLOW_Y, RADIUS, BACKGROUND_COLOR);
                    YellowLightOn = RESET;
                }
                break;
            case STOP:
                if(!RedLightOn) {
                    LCD_DrawFilledCircle(RED_X, RED_Y, RADIUS, RED_COLOR);
                    RedLightOn = SET;
                }
                if(GreenLightOn) {
                    LCD_DrawFilledCircle(GREEN_X, GREEN_Y, RADIUS, BACKGROUND_COLOR);
                    GreenLightOn = RESET;
                }
                if(YellowLightOn) {
                    LCD_DrawFilledCircle(YELLOW_X, YELLOW_Y, RADIUS, BACKGROUND_COLOR);
                    YellowLightOn = RESET;
                }
                break;
            case GO:
                if(RedLightOn) {
                    LCD_DrawFilledCircle(RED_X, RED_Y, RADIUS, BACKGROUND_COLOR);
                    RedLightOn = RESET;
                } 
                if(!GreenLightOn) {
                    LCD_DrawFilledCircle(GREEN_X, GREEN_Y, RADIUS, GREEN_COLOR);
                    GreenLightOn = SET;
                }
                if(YellowLightOn) {
                    LCD_DrawFilledCircle(YELLOW_X, YELLOW_Y, RADIUS, BACKGROUND_COLOR);
                    YellowLightOn = RESET;
                }
                break;
            case WARN:
                if(RedLightOn) {
                    LCD_DrawFilledCircle(RED_X, RED_Y, RADIUS, BACKGROUND_COLOR);
                    RedLightOn = RESET;
                } 
                if(GreenLightOn) {
                    LCD_DrawFilledCircle(GREEN_X, GREEN_Y, RADIUS, BACKGROUND_COLOR);
                    GreenLightOn = RESET;
                }
                if(!YellowLightOn) {
                    LCD_DrawFilledCircle(YELLOW_X, YELLOW_Y, RADIUS, YELLOW_COLOR);
                    YellowLightOn = SET;
                }
                break;
            default:
                if(RedLightOn) {
                    LCD_DrawFilledCircle(RED_X, RED_Y, RADIUS, BACKGROUND_COLOR);
                    RedLightOn = RESET;
                } 
                if(GreenLightOn) {
                    LCD_DrawFilledCircle(GREEN_X, GREEN_Y, RADIUS, BACKGROUND_COLOR);
                    GreenLightOn = RESET;
                }
                if(YellowLightOn) {
                    LCD_DrawFilledCircle(YELLOW_X, YELLOW_Y, RADIUS, BACKGROUND_COLOR);
                    YellowLightOn = RESET;
                }
                break;
        }

        // trigger a transition if the transition timer has expired
        transition_requested = TIM_ReadRawITStatus(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA) ? SET : transition_requested;
        if (transition_requested) {
            present_state = next_state;
            transition_requested = RESET;
            pedestrian_pressed = RESET;
            startStop_pressed = RESET;

            TIM_ClearIT(TRANSITION_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_ClearIT(PEDESTRIAN_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);
            TIM_ClearIT(START_STOP_BUTTON_TIMER, TIM_ITReadPos_TimeoutA);

            // restart the pedestrian count too, or else input would be ambigous
            TIM_LoadCountConc(PEDESTRIAN_BUTTON_TIMER, SECONDS_TO_COUNT(BUTTON_PRESS_IN_S, OSCILLATOR_FREQ));
            TIM_LoadCountConc(TRANSITION_TIMER, SECONDS_TO_COUNT(TRANSITION_TIMEOUT_IN_S, OSCILLATOR_FREQ));
        }

        // update data
        startStop_prev = startSTop_new;
        pedestrian_prev = pedestrian_new;
    }
}