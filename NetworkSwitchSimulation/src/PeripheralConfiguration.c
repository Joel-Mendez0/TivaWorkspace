#include "PeripheralConfiguration.h"

void ConfigureUART(){

    // UART 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // UART 3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART3) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));

    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTEnable(UART0_BASE);
    UARTEnable(UART3_BASE);
}

void ConfigureI2C(){
    // I2C0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    I2CMasterEnable(I2C0_BASE);
}


void UARTPrint(uint32_t ui32Base, const char* str){
    while(*str != '\0'){
        UARTCharPut(ui32Base, *str);
        str++;
    }
}

void I2CSendData(uint32_t ui32Base, uint8_t ui8SlaveAddr, uint8_t* data, uint8_t length){
    I2CMasterSlaveAddrSet(ui32Base, ui8SlaveAddr, false);

    if(length <= 0){return;}

    if(length == 1){
        I2CMasterDataPut(ui32Base, *data);
        I2CMasterControl(ui32Base, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy(ui32Base));
    }
    else if(length == 2){
        I2CMasterDataPut(ui32Base, *data);
        I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_START);
        while (I2CMasterBusy(ui32Base));

        data++;

        I2CMasterDataPut(ui32Base, *data);
        I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while (I2CMasterBusy(ui32Base));
    }
    else{
        I2CMasterDataPut(ui32Base, *data);
        I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_START);
        while (I2CMasterBusy(ui32Base));

        uint8_t i;
        for(i = 1; i < length - 1; i++){
            I2CMasterDataPut(ui32Base, data[i]);
            I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_CONT);
            while (I2CMasterBusy(ui32Base));
        }

        I2CMasterDataPut(ui32Base, data[length-1]);
        I2CMasterControl(ui32Base, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while (I2CMasterBusy(ui32Base));

    }

}

void ConfigureBoardLED(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

void LCDSendCMD(char cmd){
    char data_u, data_l;
      uint8_t data_t[4];
      data_u = (cmd&0xf0);
      data_l = ((cmd<<4)&0xf0);
      data_t[0] = data_u|0x0C;  //en=1, rs=0
      data_t[1] = data_u|0x08;  //en=0, rs=0
      data_t[2] = data_l|0x0C;  //en=1, rs=0
      data_t[3] = data_l|0x08;  //en=0, rs=0
      I2CSendData(I2C0_BASE, LCD_ADDRESS, (uint8_t *)data_t, 4);
      DelayMs(1);

}
void LCDInit(){
    DelayMs(50);
    LCDSendCMD(0x30);
    DelayMs(5);
    LCDSendCMD(0x30);
    DelayMs(1);
    LCDSendCMD(0x30);
    DelayMs(10);
    LCDSendCMD(0x20);
    DelayMs(10);
    LCDSendCMD(0x28);
    DelayMs(1);
    LCDSendCMD(0x08);
    DelayMs(1);
    LCDSendCMD(0x01);
    DelayMs(1);
    DelayMs(1);
    LCDSendCMD(0x06);
    DelayMs(1);
    LCDSendCMD(0x0C);
}
void LCDclear(){
    LCDSendCMD(0x01);
    DelayMs(5);
}

void LCDCur(int row, int col){
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    LCDSendCMD(col);
}

void LCDSendData(char data){
    char data_u, data_l;
       uint8_t data_t[4];
       data_u = (data&0xf0);
       data_l = ((data<<4)&0xf0);
       data_t[0] = data_u|0x0D;  //en=1, rs=1
       data_t[1] = data_u|0x09;  //en=0, rs=1
       data_t[2] = data_l|0x0D;  //en=1, rs=1
       data_t[3] = data_l|0x09;  //en=0, rs=1
       I2CSendData(I2C0_BASE, LCD_ADDRESS, (uint8_t *)data_t, 4);
       DelayMs(1);

}

void LCDSendString(char* str){
    while (*str) LCDSendData(*str++);
}
