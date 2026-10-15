#include "drivers/uart.h"
#include "kernel/mmu.h"
#include "kernel/exceptions.h"
#include "kernel/gic.h"
#include "kernel/timer.h"
#include "kernel/heap.h"

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

  heap_init();
  kprint("[" CLR_GREEN "BOOT" CLR_RESET "] Heap: ready\n");

  void *a = kmalloc(128);
  void *b = kmalloc(64);
  void *c = kmalloc(256);
  
  kprint("[" CLR_GREEN "TEST" CLR_RESET "] allocated a, b, c\n");
  heap_dump();

  kfree(b);
  kprint("[" CLR_GREEN "TEST" CLR_RESET "] freed b\n");
  heap_dump();

  void *d = kmalloc(48);
  kprint("[" CLR_GREEN "TEST" CLR_RESET "] allocated d (should reuse b slot)\n");
  heap_dump();

  kfree(a);
  kfree(c);
  kfree(d);
  kprint("[" CLR_GREEN "TEST" CLR_RESET "] freed all: heap should be mostly coalesced\n");

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
