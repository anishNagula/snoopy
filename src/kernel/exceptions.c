#include <stdint.h>
#include "drivers/uart.h"
#include "kernel/exceptions.h"
#include "kernel/gic.h"
#include "kernel/timer.h"

extern void vectors(void);

void exceptions_init(void) {
    // load address of vector table into VBAR_EL1
    asm volatile(
        "msr vbar_el1, %0\n"
        "isb\n"
        :: "r"((uint64_t)vectors)
        : "memory"
    );
}

static void print_hex(uint64_t val) {
    const char hex[] = "0123456789abcdef";
    kprint("0x");
    for (int i = 60; i >= 0; i--) {
        uart_putc(hex[(val >> i) & 0xF]);
    }
}

void sync_exception_handler(void) {
    uint64_t esr, elr, far;

    asm volatile("mrs %0, esr_el1" : "=r"(esr));
    asm volatile("mrs %0, elr_el1" : "=r"(elr));
    asm volatile("mrs %0, far_el1" : "=r"(far));

    uint32_t ec = (esr >> 26) & 0x3F;
    uint32_t iss = esr & 0x01FFFFFF;

    kprint("\n[SYNC EXCEPTION]\n");
    kprint("  ESR_EL1 = "); print_hex(esr);  kprint("\n");      // exception syndrome register: captures why exception happend
    kprint("  ELR_EL1 = "); print_hex(elr);  kprint("\n");      // exception link register: holds exact PC addr of faulting instr
    kprint("  FAR_EL1 = "); print_hex(far);  kprint("\n");      // fault address register: contains memory address which was accessed
    kprint("  ES      = "); print_hex(ec);   kprint("\n");      // exception class: tell category of the event
    kprint("  ISS     = "); print_hex(iss);  kprint("\n");      // instruction specific syndrome: provides extra context

    // spin, unrecoverable
    while (1);
}

void irq_handler(void) {
    // acknowledge interrupt, tells GIC we've seen it and returns the INTID of the highest-priority pending interrupt
    uint32_t iar = gic_acknowledge();
    uint32_t irq = iar & 0x3FF;         // INTID lives in bits [9:0]

    if (irq == TIMER_IRQ) {
        timer_tick();
    } else {
        // unknown interrupt, acknowledge it so the GIC doesn't stall
        kprint("[irq] unhandled INTID: ");
    }

    // signal end-of-interrupt, GIC only forwards the next interrupt of this priority after we write here
    gic_end(iar);
}