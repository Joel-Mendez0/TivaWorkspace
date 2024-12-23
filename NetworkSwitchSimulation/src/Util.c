#include "Util.h"

void DelayMs(uint32_t ms){
    uint32_t SystemClock = SysCtlClockGet();
    uint32_t count = (SystemClock / 3000) * ms;
    SysCtlDelay(count);
}

void itoa(uint32_t num, char *str) {
    uint32_t i = 0;
    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    // Process each digit
    while (num != 0) {
        str[i++] = (num % 10) + '0';  // Get next digit
        num = num / 10;
    }

    str[i] = '\0';  // Null-terminate the string

    // Reverse the string
    uint32_t j;
    for (j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}
