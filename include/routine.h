#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

struct DeadlockArgs {
    SemaphoreHandle_t a;
    SemaphoreHandle_t b;
    int counter;
};

int run_routine(SemaphoreHandle_t semaphore, int *counter, const char *from, TickType_t timeout);

void deadlock_routine(void* args);

void orphaned_lock(void* args);

void unorphaned_lock(void* args);

