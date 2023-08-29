#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

uint32_t putstr(const char* s)
{
    uint32_t c = 0;
    while(*s)
    {
        Hal_uart_put_char(*s++);
        c++;
    }
    return c;
}