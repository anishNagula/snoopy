#ifndef KERNEL_GIC_H
#define KERNEL_GIC_H

#include <stdint.h>

// GICv2 base address on QEMU virt
#define GICD_BASE   0x08000000UL
#define GICC_BASE   0x08010000UL


// distributor register offsets
#define GICD_CTLR           0x000       // distributor control register
#define GICD_ISENABLER      0x100       // interrupt set-enable registers
#define GICD_IPRIORITYR     0x400       // interrupt priority registers
#define GICD_ITARGETSR      0x800       // interrupt processor target regs
#define GICD_ICFGR          0xC00       // interrupt configuration regs

// CPU interface register offsets
#define GICC_CTLR           0x000       // CPU interface control register
#define GICC_PMR            0x004       // interrupt priority mask register
#define GICC_IAR            0x00C       // interrupt acknowledge register
#define GICC_EOIR           0x010       // end of interrupt register

// helpers
#define GICD_REG(off) (*(volatile uint32_t *)(GICD_BASE + (off)))
#define GICC_REG(off) (*(volatile uint32_t *)(GICC_BASE + (off)))


void gic_init(void);
void gic_enable_irq(uint32_t irq);
uint32_t gic_acknowledge(void);
void gic_end(uint32_t iar);

#endif