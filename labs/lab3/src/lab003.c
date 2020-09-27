
#include "choose_task.h"
#include "task1.h"
#include "task2.h"

int main(void) {

    #if (CHOSEN == TASK1)
    Task1();
    #elif (CHOSEN == TASK2_1)
    Task2_1();
    #elif (CHOSEN == TASK2_2)
    Task2_2();
    #endif

    while(1);
}