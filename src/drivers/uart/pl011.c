#include "drivers/uart.h"

typedef unsigned int u32;

#define PL011_BASE  0x09000000UL

#define UART_DR     0x000
#define UART_FR     0x018
#define UART_IBRD   0x024
#define UART_FBRD   0x028
#define UART_LCR_H  0x02C
#define UART_CR     0x030
#define UART_ICR    0x044

#define UART_FR_TXFF    (1u << 5)

static volatile u32 *uart_reg(u32 offset) {
    return (volatile u32 *)(PL011_BASE + offset); 
}

void uart_init(void) {
    *uart_reg(UART_CR) = 0;             // disable UART before changing config
    *uart_reg(UART_ICR) = 0x7FF;        // clear pending interrupts
    
    // 24 MHz UART clock, 115200 baud
    *uart_reg(UART_IBRD) = 13;
    *uart_reg(UART_FBRD) = 1;

    *uart_reg(UART_LCR_H) = (3u << 5) | (1u << 4);  // eight data bits, FIFO enabled

    *uart_reg(UART_CR) = (1u << 0) |    // enable UART, transmitter, reciever
                         (1u << 8) |
                         (1u << 9);
}

void uart_putc(char c) {
    if (c == '\n')
        uart_putc('\r');

    // wait until the transmit FIFO has space
    while (*uart_reg(UART_FR) & UART_FR_TXFF) {
        __asm__ volatile("yield");
    }

    *uart_reg(UART_DR) = (u32)c;
}

void uart_write(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}