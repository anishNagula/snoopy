#include "drivers/uart.h"
#include "kernel/panic.h"

__attribute__((noreturn))
void panic(const char *message) {
    uart_write("\nSNOOPY PANIC: ");

    if (message != 0) {
        uart_write(message);
    }

    uart_write("\nSystem halted.\n");

    for (;;) {
        __asm__ volatile("wfe");
    }
}