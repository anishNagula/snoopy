#ifndef KERNEL_GIC_H
#define KERNEL_GIC_H

#include <stdint.h>

// GICv2 base address on QEMU virt
#define GICD_BASE   0x08000000UL        // distributor (settings, priority assignment, target CPUs, enable/disable individual interrupt lines)
#define GICC_BASE   0x08010000UL        // CPU interface (manages interrupts delivered directly to a specific processor core)


// distributor register offsets
#define GICD_CTLR           0x000       // distributor control register (on/off)
#define GICD_ISENABLER      0x100       // interrupt set-enable registers (enabled/masked)
#define GICD_IPRIORITYR     0x400       // interrupt priority registers (assign)
#define GICD_ITARGETSR      0x800       // interrupt processor target regs (specify CPU core)
#define GICD_ICFGR          0xC00       // interrupt configuration regs (level-sensitive/edge-triggered)

// CPU interface register offsets
#define GICC_CTLR           0x000       // CPU interface control register (enable/disable)
#define GICC_PMR            0x004       // interrupt priority mask register (0xFF = any priority level)
#define GICC_IAR            0x00C       // interrupt acknowledge register (r-o reg used by kernel when interrupt fires)
#define GICC_EOIR           0x010       // end of interrupt register (written by kernel after handling interrupt)

// helpers
#define GICD_REG(off) (*(volatile uint32_t *)(GICD_BASE + (off)))
#define GICC_REG(off) (*(volatile uint32_t *)(GICC_BASE + (off)))


void gic_init(void);
void gic_enable_irq(uint32_t irq);
uint32_t gic_acknowledge(void);
void gic_end(uint32_t iar);

#endif