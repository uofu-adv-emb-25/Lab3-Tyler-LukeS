#include "routine.h"

#define SLEEPTIME 5

// Run the main thread body (routine) of the task
int run_routine(SemaphoreHandle_t semaphore,
            int *counter,
            const char *src,
            TickType_t timeout)
{
    if (xSemaphoreTake(semaphore, timeout) == pdFALSE)
        return pdFALSE;
    {
        (*counter)++;
        printf("hello world from %s! Count %d\n", src, *counter);
    }
    xSemaphoreGive(semaphore);
    return pdTRUE;
}