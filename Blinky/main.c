#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "tm4c123gh6pm.h"
#include "HeaderTest.h"


#define HIGH_ON_BOARD GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define LOW_ON_BOARD GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0)

int main(void)
{

    int num = returnbruh(); // Testing Headers

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configure LEDS as OUTPUTS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    // Configure Switch as Input
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    volatile uint8_t switch1 = 0;
    volatile uint32_t registervalue = 0;

    while(1){

        switch1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

        uint32_t registervalue = *(GPIO_PORTF_DATA_BITS_R);

        (switch1) ? LOW_ON_BOARD : HIGH_ON_BOARD;



    }
}
