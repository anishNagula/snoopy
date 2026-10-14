#include "drivers/uart.h"
#include "kernel/mmu.h"


void kernel_main(void) {
  mmu_init();
  uart_init();

  kprint("Hello from Snoopy!\n");

  while (1);
}
