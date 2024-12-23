#ifndef INCLUDE_UTIL_H_
#define INCLUDE_UTIL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "driverlib/sysctl.h"

void DelayMs(uint32_t ms);
void itoa(uint32_t num, char *str);
#endif
