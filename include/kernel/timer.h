#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

#define TIMER_IRQ   30      // INTID 30: Non-secure EL1 physical timer (PPI)

void timer_init(uint32_t interval_ms);
void timer_tick(void);
uint64_t timer_get_ticks(void);

#endif