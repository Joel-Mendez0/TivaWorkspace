#ifndef INCLUDE_PERIPHERALCONFIGURATION_H_
#define INCLUDE_PERIPHERALCONFIGURATION_H_

#define HIGH_ON_BOARD GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define LOW_ON_BOARD GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0)
#define LCD_ADDRESS 0x27

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "Util.h"

void ConfigureUART();
void UARTPrint(uint32_t ui32Base, const char* str);
void ConfigureBoardLED();
void ConfigureI2C();
void I2CSendData(uint32_t ui32Base, uint8_t ui8SlaveAddr, uint8_t* data, uint8_t length);
void LCDSendCMD(char cmd);
void LCDInit();
void LCDclear();
void LCDCur(int row, int col);
void LCDSendData(char data);
void LCDSendString(char* str);




#endif
