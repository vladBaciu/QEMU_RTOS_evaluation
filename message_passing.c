//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//#include "Timer.h"
#include "BlinkLed.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (configTICK_RATE_HZ * 3 / 4)
#define BLINK_OFF_TICKS (configTICK_RATE_HZ - BLINK_ON_TICKS)
TaskHandle_t taskAHandle, taskBHandle;
uint32_t it1, it2, delta;      // start and stop flag
SemaphoreHandle_t xSem = 0;
QueueHandle_t xQueue1, xQueue2;

void taskA(void *parms __attribute__((unused))) {
	char buffer[10];

	// Infinite loop
	while (1) {
		trace_printf("Send message\n");

		SysTick->LOAD = 0x00FFFFFF;
		SysTick->CTRL |= 0x00000001;
		it1 = SysTick->VAL;
		xQueueSend(xQueue1, (void* ) &buffer, (TickType_t ) portMAX_DELAY );





	}

}

void taskB(void *parms __attribute__((unused))) {
	char buffer[10];

	// Infinite loop
	while (1) {
		trace_printf("Wait message\n");
		blink_led_on();
		xQueueReceive(xQueue1, (void* ) &buffer, (TickType_t ) portMAX_DELAY );
		it2 = SysTick->VAL;
		SysTick->CTRL &= ~0x00000001;
		blink_led_off();
		trace_printf("Time: %u\t%u\n", xTaskGetTickCount(), 0x00FFFFFF & (it1 - it2));

	}

}
// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void SysTick_Init(void) {
	SysTick->CTRL = 0x00000005;
}

int main(int argc, char *argv[]) {
	SysTick_Init();
	blink_led_init();
	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello ARM World!");
	xQueue1 = xQueueCreate(10, sizeof(unsigned long));
	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	xTaskCreate(taskA, "taskA", 256, NULL, configMAX_PRIORITIES - 2,
			&taskAHandle);
	xTaskCreate(taskB, "taskB", 256, NULL, configMAX_PRIORITIES - 1,
			&taskBHandle);

	vTaskStartScheduler();
	vTaskResume(taskBHandle);

	vTaskResume(taskAHandle);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
