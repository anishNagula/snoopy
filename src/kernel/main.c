#include <stddef.h>

#include "drivers/uart.h"
#include "kernel/fdt.h"
#include "kernel/panic.h"

void kernel_main(const void *dtb) {
    
    uart_init();
    size_t dtb_size = 0;

    if (!fdt_validate(dtb, &dtb_size))
        panic("Invalid device tree supplied at kernel entry");

    uart_write("\n");
    uart_write("================================\n");
    uart_write("        SNOOPY KERNEL\n");
    uart_write("================================\n");
    uart_write("Architecture: AArch64\n");
    uart_write("Platform:     QEMU virt\n");
    uart_write("Device tree:  Valid\n");
    uart_write("Status:       Kernel initialized.\n");
    uart_write("\n");

    (void)dtb_size;

    for (;;) {
        __asm__ volatile("wfe");
    }
}