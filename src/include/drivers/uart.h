#ifndef SNOOPY_DRIVERS_UART_H
#define SNOOPY_DRIVERS_UART_H

void uart_init(void);
void uart_putc(char c);
void uart_write(const char *str);

#endif