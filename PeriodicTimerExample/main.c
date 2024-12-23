#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "tm4c123gh6pm.h"

#define HIGH_GPIO_2 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2)
#define LOW_GPIO_2 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0)

// Global variable to count the number of interrupts
volatile uint8_t interrupt_count = 0;
volatile bool Running = true;
volatile uint64_t loopcount = 0;

void Timer0IntHandler(void);

int main(void) {

    // Set the system clock to 16 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // GPIO Configuration

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Enable the Timer 0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Wait for the Timer 0 module to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // Configure Timer 0 as a 32-bit periodic timer
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Set the timer load value to trigger every second
    uint32_t load_value = SysCtlClockGet(); // 16 MHz clock = 1 second
    TimerLoadSet(TIMER0_BASE, TIMER_A, load_value - 1);

    // Register the interrupt handler for Timer 0A
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);

    // Enable the timer timeout interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable the Timer 0A interrupt in the NVIC
    IntEnable(INT_TIMER0A);

    // Enable global interrupts
    IntMasterEnable();

    // Enable Timer 0A
    TimerEnable(TIMER0_BASE, TIMER_A);

    while (Running) {
        loopcount++;
        if(interrupt_count == 3){
            break;
        }
    }
}

// Timer 0A interrupt handler
void Timer0IntHandler(void) {

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    interrupt_count++;

    if(interrupt_count == 3){
        IntDisable(INT_TIMER0A);
    }
}
