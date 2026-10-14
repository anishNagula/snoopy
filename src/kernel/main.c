#include "drivers/uart.h"
#include "kernel/mmu.h"
#include "kernel/exceptions.h"
#include "kernel/gic.h"
#include "kernel/timer.h"

void kernel_main(void) {
  uart_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] UART interface initialized.\n");

  mmu_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] MMU online. Translation tables mapping dynamic identity space.\n");

  exceptions_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] Exceptions: ready\n");

  gic_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] GIC: ready\n");

  timer_init(500);
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] Timer: armed\n");

  asm volatile("msr daifclr, #2");
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] Interrupts enabled: Waiting for timer...\n");
  
  // 4. Final handoff
  kprint("\n=========================================\n");
  kprint(" Hello from Snoopy! Kernel boot complete. \n");
  kprint("=========================================\n\n");
  kprint("[" CLR_GREEN "SNOOPY" CLR_RESET "] System entering idle kernel state.\n");

  while (1) {
    asm volatile("wfi");
  }
}
