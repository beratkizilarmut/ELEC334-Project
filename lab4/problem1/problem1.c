/* main.c Problem 1, millisecond delayer 
    Berat Kizilarmut */
#include "stm32g0xx.h"
volatile uint32_t delaycount = 0; /* Delay counter, counts every millisecond */
void delay_ms(volatile uint32_t s) /* Set as volatile to prevent optimization */
{
    delaycount = 0; /* Set counter as zero */
    while(1) /* Continuously checks the counter */
    {
        if(delaycount == s)
        return;
    }
}

void SysTick_Handler(void)
{
    delaycount++; /* SysTick increases the count every millisecond */
}

int main(void) { /* Overriding default interrupt handler func */
    SysTick_Config(16000); /* 16 MHz Clock speed, 16000 ticks mean a millisecond */
    delay_ms(3000); /* Delay 3000 ms, 3 seconds */
    while(1) { } /* Endless loop */
    return 0;
}
