#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <stdio.h>

int run_routine(SemaphoreHandle_t semaphore, int *counter, const char *from, TickType_t timeout);
