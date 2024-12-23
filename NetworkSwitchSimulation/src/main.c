#include "PeripheralConfiguration.h"
#include "Util.h"

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    ConfigureUART();
    ConfigureBoardLED();
    ConfigureI2C();
    LCDInit();

    uint32_t SystemClock = SysCtlClockGet();
    char buffer[10];
    itoa(SystemClock, buffer);
    UARTPrint(UART0_BASE, "System Clock: ");
    UARTPrint(UART0_BASE, buffer);
    UARTPrint(UART0_BASE, " Hz\r\n");

    uint8_t data[10] = {1,2,3,4,5,6,7,8,9,10};
    uint8_t length = sizeof(data) / sizeof(data[0]);

    I2CSendData(I2C0_BASE, LCD_ADDRESS, data, length);

    LCDclear();
    LCDSendString("Take Take");
    LCDCur(1, 0);
    LCDSendString("Take Take");



}
