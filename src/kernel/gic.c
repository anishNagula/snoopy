#include "kernel/gic.h"

void gic_init(void) {
    // enable the distributor, forwards interrupts to the CPU interfaces
    GICD_REG(GICD_CTLR) = 1;

    // set CPU interface priority mask (0xFF is accept any priority level)
    GICC_REG(GICC_PMR) = 0xFF;

    // enable CPU interface so it can signal the CPU
    GICC_REG(GICC_CTLR) = 1;
}


void gic_enable_irq(uint32_t irq) {
    
    // GICD_ISENABLER is an array of 32-bit registers, one bit per IRQ, IRQ n -> register index n/32, bit n%32
    uint32_t reg_idx = irq / 32;
    uint32_t bit     = irq % 32;
    GICD_REG(GICD_ISENABLER + reg_idx * 4) = (1U << bit);
}

uint32_t gic_acknowledge(void) {
    // reading GICC_IAR atomically acknowledges the interrupt and returns the INTID in bits [9:0], the
    // remaining bits encode the CPU source ID.
    return GICC_REG(GICC_IAR);
}

void gic_end(uint32_t iar) {
    // writing the IAR value back to GICC_EOIR signals to the GIC that we are done handling this interrupt
    // GIC can then forward the next pending interrupt of the same priority.
    GICC_REG(GICC_EOIR) = iar;
}