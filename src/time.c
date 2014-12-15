#include "time.h"

volatile uint32_t usTicks;
volatile uint32_t usDelay;

void Time_Init(void) {
	SystemCoreClockUpdate();
  	SysTick_Config(SystemCoreClock / MICRO_TIME_BASE);
}

void delay(uint32_t nTime) {
	uint32_t startTime = usTicks;

	while(nTime > 0) {
		if(usTicks - startTime >= 1000) {	// test if 1 second passed
			nTime--;						// decrement ms delay
			startTime += 1000;				// make startTime = usTicks
		}
	}
}

void delayMicroseconds(uint32_t nTime) {
	usDelay = nTime;
	while(usDelay);
}
