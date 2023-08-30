#include "stdint.h"
#include "HalUart.h"

#include "stdio.h"

static void Hw_init(void);

void main(void)
{
    Hw_init();

    uint32_t i = 100;
    while(i --)
    {
        Hal_uart_put_char('Y');
    }
    Hal_uart_put_char('\n');                // char = '' ..?

    putstr("oh! Hell no..!\n");             // string = ""  ...???

    i = 100;
    while(i--)
    {
        uint8_t ch = Hal_uart_get_char();
        Hal_uart_put_char(ch);
    }
}

static void Hw_init(void)
{
    Hal_uart_init();
}
