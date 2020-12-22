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
#include <string.h>

#define	DUMMY_STR_LEN		30
#define DUMMY_NUM 128
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
TaskHandle_t taskAHandle, taskBHandle, taskDummyHandle;
uint32_t it1, it2, delta;      // start and stop flag
SemaphoreHandle_t xSem = 0;
QueueHandle_t xQueue1, xQueue2;
int taskMLTCOneDummy[DUMMY_NUM];

#define BUTTON_PORT_NUMBER        (0)
#define BUTTON_PIN_NUMBER         (0)
SemaphoreHandle_t xSemaphore = NULL;

void taskB(void *parms __attribute__((unused))) {
	char buffer[10];

	// Infinite loop
	while (1) {
		trace_puts("Wait for ISR semaphore");

		if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE) {
			/* We were able to obtain the semaphore and can now access the
			 shared resource. */

			it2 = SysTick->VAL;
			SysTick->CTRL &= ~0x00000001;
			trace_puts("Semaphore given");
			blink_led_off();
			trace_printf("Time: %u\t%u\n", xTaskGetTickCount(),
					0x00FFFFFF & (it1 - it2));\
			/* We have finished accessing the shared resource.  Release the
			 semaphore. */
			NVIC_EnableIRQ(EXTI0_IRQn);

		}

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
	// Show the program parameters (passed via semihosting).
	// Output is via the semihosting output channel.
	SysTick_Init();
	vSemaphoreCreateBinary(xSemaphore);
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	blink_led_init();

	// Send a greeting to the trace device (skipped on Release).

	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	RCC->AHB1ENR |= BLINK_RCC_MASKx(BUTTON_PORT_NUMBER);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin in output push/pull mode
	GPIO_InitStructure.Pin = BLINK_PIN_MASK(BUTTON_PIN_NUMBER);
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BLINK_GPIOx(BUTTON_PORT_NUMBER), &GPIO_InitStructure);
	NVIC_SetPriority(EXTI0_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
	NVIC_EnableIRQ(EXTI0_IRQn);

	xTaskCreate(taskB, "taskB", 256, NULL, configMAX_PRIORITIES-1,
			&taskBHandle);

	vTaskStartScheduler();
	vTaskResume(taskBHandle);

	return 0;
}

#pragma GCC diagnostic pop

void EXTI0_IRQHandler(void) {

	trace_printf("EXTI\n");

	NVIC_DisableIRQ(EXTI0_IRQn);

	BaseType_t xHigherPriorityTaskWoken;
	blink_led_on();
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->CTRL |= 0x00000001;
	it1 = SysTick->VAL;

	xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);

	// Resume the suspended task.

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
