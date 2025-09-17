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



void supervisor_thread(__unused void *args)
{
    while(1) {
        printf("Starting tests...\n");
        UNITY_BEGIN();
        RUN_TEST(test_run_routine);
        RUN_TEST(test_run_routine_positive);
        // RUN_TEST(test_deadlock);
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
