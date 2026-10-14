#include "drivers/uart.h"

void uart_init(void) {
  // disable UART before configuration
  UART_REG(UARTCR) = 0;

  // set baud rate (QEMU virt uses 24MHz UART clock)
  UART_REG(UARTIBRD) = 13;
  UART_REG(UARTFBRD) = 1;

  // line control: 8-bit word length, FIFOs enabled, 1 stop bit, no parity
  UART_REG(UARTLCR_H) = UARTLCR_WLEN8 | UARTLCR_FEN;
  // renable: UART on, TX on, RX on
  UART_REG(UARTCR) = UARTCR_UARTEN | UARTCR_TXE | UARTCR_RXE;
}

void uart_putc(char c) {
  while (UART_REG(UARTFR) & UARTFR_TXFF)
    ;
  UART_REG(UARTDR) = (uint32_t)c;
}

void uart_puts(const char *s) {
  while (*s) {
    uart_putc(*s++);
  }
}

void kprint(const char *s) {
  uart_puts(s);
}
