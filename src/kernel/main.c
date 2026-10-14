#include "drivers/uart.h"
#include "kernel/mmu.h"

// Simple delay loop to slow down the text output
void spin_delay(volatile unsigned int count) {
    while (count--) {
        __asm__ volatile("nop");
    }
}

void kernel_main(void) {
  uart_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] UART interface initialized.\n");

  mmu_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] MMU online. Translation tables mapping dynamic identity space.\n");
  
  // 4. Final handoff
  kprint("\n=========================================\n");
  kprint(" Hello from Snoopy! Kernel boot complete. \n");
  kprint("=========================================\n\n");
  kprint("[" CLR_GREEN "SNOOPY" CLR_RESET "] System entering idle kernel state.\n");

  while (1);
}
