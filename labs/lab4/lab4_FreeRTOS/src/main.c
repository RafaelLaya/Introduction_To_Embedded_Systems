
#include "FreeRTOS.h"
#include "task.h"

// SSD2119 Display and Touch Drivers
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"

// header file specific to task 2
#include "task2.h"

#define SET         1U 
#define RESET       0U

// Flags for the buttons and the timer
int pedestrian_pressed = RESET;
int onoff_pressed = RESET;
int time_expired = RESET;

// light_tick is global because FSM() needs to reset the timer when an input
// results in a transition 
static int curr_light_tick_time = 0;
static int prev_light_tick_time = 0;

// ped_tick global because FSM() needs to reset this timer to ensure that
// the pedestrian button be pressed for two seconds during GO before going to WARN
static int curr_ped_tick_time = 0;
static int prev_ped_tick_time = 0;

// Since drawing circles on the screen is time-consuming, we'll keep track of the light states
// so that we don't draw on an already drawn spot
uint8_t GreenLightOn = RESET;
uint8_t YellowLightOn = RESET;
uint8_t RedLightOn = RESET;

// Task function that checks the state of the virtual pedestrian button.
// Keeps track of how many seconds the pedestrian button has been pressed.
// Once the user has pressed the virtual pedestrian button for 2 seconds,
// it will set the global flag indicating the virtual pedestrian button
// has been pressed.
void Pedestrian(void *p);

// Task function that checks the state of the virtual onoff button.
// Keeps track of how many seconds the onoff button has been pressed.
// Once the user has pressed the onoff button for 2 seconds,
// it will set the global flag indicating the onoff button has been
// pressed
void StartStop(void *p);

// Task function that represents your Finite State Machine.
// Keeps track of how many seconds the virtual traffic light has been
// lighting on. It will update the state of the traffic light system
// every 5 seconds or once there's virtual button input.
void Control(void *p);

// Helper function that represents the traffic light FSM
// Handles the traffic light state transition.
void FSM(void);

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  while (1) {}
}

int main(void) {
  // Initialize the display and touch
  // functionalities of the SSD2119 touch display assembly.
  LCD_Init();
  Touch_Init();

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

  xTaskCreate(StartStop, (const char *)"StartStopButton", 1024, NULL, 0U, NULL);
  xTaskCreate(Pedestrian, (const char *)"PedestrianButton", 1024, NULL, 0U, NULL);
  xTaskCreate(Control, (const char *)"Control FSM", 1024, NULL, 0U, NULL);

  vTaskStartScheduler();

  while(1);
}

void StartStop(void *p) {
  // keep track of time
  static int curr_onoff_tick_time = 0;
  static int prev_onoff_tick_time = 0;
  curr_onoff_tick_time = xTaskGetTickCount();
  prev_onoff_tick_time = curr_onoff_tick_time;

  // coordinates of a touch
  unsigned long x = 0;
  unsigned long y = 0;
  while (1) {
    curr_onoff_tick_time = xTaskGetTickCount();

    // Check whether the virtual button is pressed
    x = Touch_ReadX();
    y = Touch_ReadY();
    if ((START_STOP_LEFT_LIMIT < x && x < START_STOP_RIGHT_LIMIT && 
         START_STOP_BOTTOM_LIMIT < y && y < START_STOP_TOP_LIMIT)) { 
      if (curr_onoff_tick_time - prev_onoff_tick_time >= SEC_TO_MS(BUTTON_PRESS_IN_S)) {
        // register the input
        onoff_pressed = SET;
        prev_onoff_tick_time = curr_onoff_tick_time;
      }
      else {
        onoff_pressed = RESET;
      }
    } else {
      onoff_pressed = RESET;
      prev_onoff_tick_time = curr_onoff_tick_time;
    }

    vTaskDelay(1);
  }
}

void Pedestrian(void *p) {
  curr_ped_tick_time = xTaskGetTickCount();
  prev_ped_tick_time = curr_ped_tick_time;

  // coordinates of a touch
  unsigned long x = 0;
  unsigned long y = 0;

  while (1) {
    curr_ped_tick_time = xTaskGetTickCount();

    // check if button has been pressed
    x = Touch_ReadX();
    y = Touch_ReadY();

    if (PED_LEFT_LIMIT < x && x < PED_RIGHT_LIMIT && 
        PED_BOT_LIMIT < y && y < PED_TOP_LIMIT) { 
      if (curr_ped_tick_time - prev_ped_tick_time >= SEC_TO_MS(BUTTON_PRESS_IN_S)) {
        // register the input
        pedestrian_pressed = SET;
        prev_ped_tick_time = curr_ped_tick_time;
      }
      else {
        pedestrian_pressed = RESET;
      }
    } else {
      pedestrian_pressed = RESET;
      prev_ped_tick_time = curr_ped_tick_time;
    }

    vTaskDelay(1);
  }
}

void Control(void *p) {
  // keep track of time
  time_expired = RESET;
  curr_light_tick_time = xTaskGetTickCount();
  prev_light_tick_time = curr_light_tick_time;

  while (1) {
    curr_light_tick_time = xTaskGetTickCount();

    // check if time to make a transition or if there's been virtual input
    if (curr_light_tick_time - prev_light_tick_time >= SEC_TO_MS(TRANSITION_TIMEOUT_IN_S) || onoff_pressed || pedestrian_pressed) { 
      
      // register that timer has expired
      if (curr_light_tick_time - prev_light_tick_time >= SEC_TO_MS(TRANSITION_TIMEOUT_IN_S)) {
        time_expired = SET;
        prev_light_tick_time = curr_light_tick_time;
      } 
    
      FSM();
    } else {
      time_expired = RESET;
    }

    vTaskDelay(1);
  }
}

void FSM(void) {
  static TL_states_e present_state = IDLE;
  static TL_states_e next_state = IDLE;

  // choose next state
  switch (present_state) {
    case IDLE:
      if (onoff_pressed) {
        next_state = STOP;
      } else {
        next_state = IDLE;
      }
      break;

    case STOP:
      if (onoff_pressed) {
        next_state = IDLE;
      } else if(time_expired) {
        next_state = GO;
      }
      break;

    case GO:
      if (onoff_pressed) {
        next_state = IDLE;
      } else if(pedestrian_pressed) {
        next_state = WARN;
      } else if(time_expired) {
        next_state = STOP;
      }
      break;

    case WARN:
      if (onoff_pressed) {
        next_state = IDLE;
      } else if(time_expired) {
        next_state = STOP;
      }
      break;
    
    default:
        next_state = IDLE;
      break;
  } 

  if (next_state != present_state) {
    present_state = next_state;
    time_expired = RESET;
    prev_light_tick_time = curr_light_tick_time;
    pedestrian_pressed = RESET;
    prev_ped_tick_time = curr_ped_tick_time;
  }

  // produce the output
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
}
