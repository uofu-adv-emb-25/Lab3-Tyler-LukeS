#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <stdint.h>
#include "unity_config.h"
#include <unity.h>

#include "routine.h"

// The top test runner thread should have highest priority.
#define SUPERVISOR_THREAD_PRIORITY ( tskIDLE_PRIORITY + 5UL )
#define TASK_1_PRIORITY ( SUPERVISOR_THREAD_PRIORITY - 1UL )
#define TASK_2_PRIORITY ( SUPERVISOR_THREAD_PRIORITY - 1UL )

void setUp(void) {}

void tearDown(void) {}


/**************** Activity 0-2 ****************/
void test_run_routine(void)
{
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;
    xSemaphoreTake(semaphore, portMAX_DELAY);

    int result = run_routine(semaphore, &counter, "test", 10);

    TEST_ASSERT_EQUAL_INT(pdFALSE, result);
    TEST_ASSERT_EQUAL_INT(0, counter);
}

void test_run_routine_positive(void)
{
    int counter = 0;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int result = run_routine(semaphore, &counter, "test", 10);

    TEST_ASSERT_EQUAL_INT(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, counter);
}

void test_deadlock(void) {
    int counter = 0;
    TaskHandle_t thread1, thread2;
    SemaphoreHandle_t semaphore1 = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t semaphore2 = xSemaphoreCreateCounting(1, 1);

    struct DeadlockArgs da1 = {semaphore1, semaphore2, 0};
    struct DeadlockArgs da2 = {semaphore2, semaphore1, 0};

    xTaskCreate(deadlock_routine, "1", configMINIMAL_STACK_SIZE, (void*) &da1, TASK_1_PRIORITY, &thread1);
    xTaskCreate(deadlock_routine, "2", configMINIMAL_STACK_SIZE, (void*) &da2, TASK_2_PRIORITY, &thread2);

    printf("Created threads\n");
    // Once this times out, this thread will preempt the child threads
    // so we can check their state.
    vTaskDelay(1000);
    printf("Waited 1000 ticks\n");
    
    // Here we have paused everything and are inspecting it frozen in place.
    vTaskSuspend(thread1);
    vTaskSuspend(thread2);

    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore1), 0);
    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore2), 0);
    // Each counter should only be incremented once
    TEST_ASSERT_EQUAL_INT(1, da1.counter);
    TEST_ASSERT_EQUAL_INT(1, da2.counter);
    printf("Killing threads\n"); // memory management
    vTaskDelete(thread1);
    vTaskDelete(thread2);
    printf("Killed threads\n");
    
}



void supervisor_thread(__unused void *args)
{
    while(1) {
        printf("Starting tests...\n");
        UNITY_BEGIN();
        RUN_TEST(test_run_routine);
        RUN_TEST(test_run_routine_positive);
        RUN_TEST(test_deadlock);
        // RUN_TEST(test_orphaned);
        // RUN_TEST(test_unorphaned);

        UNITY_END();
        sleep_ms(10000);
    }
}

int main (void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    xTaskCreate(supervisor_thread, "TestSupervisor",
                configMINIMAL_STACK_SIZE, NULL, SUPERVISOR_THREAD_PRIORITY, NULL);
    vTaskStartScheduler();
	return 0;
}
