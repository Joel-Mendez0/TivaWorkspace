/**
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

void DelayMs(uint32_t ms){
    uint32_t SystemClock = SysCtlClockGet();
    uint32_t count = (SystemClock / 3000) * ms;
    SysCtlDelay(count);
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);

    bool Running = true;

    while(Running){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_1 | GPIO_PIN_2);
        DelayMs(1000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, 0);
        DelayMs(1000);
    }

    return 0;
}
