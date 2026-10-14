#ifndef UART_H
#define UART_H

#include <stdint.h>

// PL011 UART base address on QEMU virt
#define UART_BASE       0x09000000UL

// register offsets
#define UARTDR          0x000     // Data register
#define UARTFR          0x018     // Flag register
#define UARTIBRD        0x024     // Int Baud Rate Div
#define UARTFBRD        0x028     // Frac Baud Rate Div
#define UARTLCR_H       0x02c     // Line control
#define UARTCR          0x030     // Control register

// UARTFR bits
#define UARTFR_TXFF     (1 << 5)  // Transmit FIFO full
#define UARTFR_BUSY     (1 << 3)  // UART busy

// UARTLCR_H bits
#define UARTLCR_FEN     (1 << 4)  // Enable FIFOs
#define UARTLCR_WLEN8   (3 << 5)  // 8-bit word length

// UARTCR bits
#define UARTCR_UARTEN   (1 << 0)  // UART enable
#define UARTCR_TXE      (1 << 8)  // Transmit enable
#define UARTCR_RXE      (1 << 9)  // Receive enable

#define CLR_RESET   "\033[0m"
#define CLR_GREEN   "\033[32m"

// Helper macro: read a PL011 register
#define UART_REG(offset) \
    (*(volatile uint32_t *)(UART_BASE + (offset)))


void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void kprint(const char *s);


#endif
