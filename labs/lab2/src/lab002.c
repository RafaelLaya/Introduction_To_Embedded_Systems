#include "task1.h"
#include "task2.h"

// Choices for which task to run
#define TASK1A          100U
#define TASK1B          101U
#define TASK2A          200U
#define TASK2B          201U
#define TASK2C          202U

// Chosen task
#define EXECUTE         TASK2C

int main(void) {
    switch (EXECUTE) {
        case TASK1A:
            Task1A();
            break;

        case TASK1B:
            Task1B();
            break;

        case TASK2A:
            Task2A();
            break;

        case TASK2B:
            Task2B();
            break;

        case TASK2C:
            Task2C();
            break;
    }
    while(1);
}

