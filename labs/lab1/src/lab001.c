#include "task1.h"
#include "task2.h"

// Choices for which task to run
#define TASK1_PART2     1U
#define TASK1_PART3     2U
#define TASK2           3U

// Chosen task
#define EXECUTE         TASK2

int main(void) {
    if (EXECUTE == TASK1_PART2) {
        Task1_Part2();
    } else if (EXECUTE == TASK1_PART3) {
        Task1_Part3();
    } else if (EXECUTE == TASK2) {
        Task2();
    }
    while(1);
}
