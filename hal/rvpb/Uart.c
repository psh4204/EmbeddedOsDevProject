#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t* Uart;  // Reg.c에서 선언 된 친구를 가져옴

void Hal_uart_init(void)
{
    // Enable UART
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;
}

void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF);
    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Hal_uart_get_char(void)
{
    uint8_t data;

    while(Uart->uartfr.bits.RXFE);

    data = Uart->uartdr.all; // Stack 쪽 메모리를 잘써라. ROM, HW레지스터 일일이 읽는거 보다 수십수백배 빠름.

    if(data & 0xFFFFFFF00)
    {
        Uart->uartdr.all = 0xFF;
        return 0;
    }
    
    return (uint8_t)(data & 0xFF);
}