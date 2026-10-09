#include "drivers/uart.h"

void kernel_main(void) {
    uart_init();

    uart_write("\n");
    uart_write("================================\n");
    uart_write("        SNOOPY KERNEL\n");
    uart_write("================================\n");
    uart_write("Architecture: AArch64\n");
    uart_write("Platform:     QEMU virt\n");
    uart_write("Status:       Kernel entered.\n");
    uart_write("\n");

    for (;;) {
        __asm__ volatile("wfe");
    }
}