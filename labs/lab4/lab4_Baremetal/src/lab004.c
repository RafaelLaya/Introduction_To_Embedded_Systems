
#include "task1.h"
#include "task2.h"

#define TASK1B              2U
#define TASK1C              3U
#define TASK2A              4U
// Task 2B is not compiled here. It is run within the FreeRTOS demo project

#define CHOSEN              TASK2A  

int main(void) {

    switch(CHOSEN) {
        case TASK1B: 
            Task1B();
            break;
        
        case TASK1C:
            Task1C();
            break;
        
        case TASK2A:
            Task2A();
            break;
    }

    while(1);
}