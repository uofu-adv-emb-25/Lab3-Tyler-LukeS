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

// Create a deadlock routine that takes in a semaphore and locks it
void deadlock_routine(void* args) {
    struct DeadlockArgs *dargs = (struct DeadlockArgs *) args;
    
    xSemaphoreTake(dargs->a, portMAX_DELAY); // attempt to take lock a
    {
        dargs->counter++;
        vTaskDelay(100); // Delay so that the other deadlock thread can take lock b
        xSemaphoreTake(dargs->b, portMAX_DELAY); // attempt to take lock b
        {
            dargs->counter++; // This should never run
        }
        xSemaphoreGive(dargs->b);
    }
    xSemaphoreGive(dargs->a);
    vTaskSuspend(NULL);
}

void orphaned_lock(void* args) {
    struct DeadlockArgs *dargs = (struct DeadlockArgs *) args;
    while (1) {
        xSemaphoreTake(dargs->a, portMAX_DELAY);
        dargs->counter++;
        if (dargs->counter % 2) { // True if count is odd
            continue;
        }
        printf("Count %d\n", dargs->counter);
        xSemaphoreGive(dargs->a);
    }
}

void unorphaned_lock(void* args) {
    struct DeadlockArgs *dargs = (struct DeadlockArgs *) args;
    while (1) {
        xSemaphoreTake(dargs->a, portMAX_DELAY);
        {

        dargs->counter++;
        if ( !(dargs->counter % 2) ) { // True if count is Even
            printf("Count %d\n", dargs->counter);
        }
        
        }
        xSemaphoreGive(dargs->a);
        vTaskDelay(100);
    }
}