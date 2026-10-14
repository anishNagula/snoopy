#include "kernel/mmu.h"
#include <stdint.h>

// descriptor bit definitions
#define DESC_VALID      (1UL << 0)
#define DESC_TABLE      (1UL << 1)  // level 0, 1, 2 pointing to next table
#define DESC_BLOCK      (0UL << 1)  // level 1, 2 termination blocks


#define DESC_AF         (1UL << 10)
#define DESC_SH_INNER   (3UL << 8)
#define DESC_SH_NONE    (0UL << 8)
#define DESC_AP_RW_EL1  (0UL << 6)
#define DESC_ATTR(idx)  ((uint64_t)(idx) << 2)
#define DESC_PXN        (1UL << 53)
#define DESC_UXN        (1UL << 54)

// composite block descriptors
#define BLOCK_NORMAL \
    (DESC_VALID | DESC_BLOCK | DESC_AF | \
     DESC_SH_INNER | DESC_AP_RW_EL1 | DESC_ATTR(0))

#define BLOCK_DEVICE \
    (DESC_VALID | DESC_BLOCK | DESC_AF | \
     DESC_SH_NONE | DESC_AP_RW_EL1 | DESC_ATTR(1) | DESC_PXN  | DESC_UXN)

// MAIR & TCR settings
#define MAIR_NORMAL_WB  0xFFUL
#define MAIR_DEVICE_NG  0x00UL
#define MAIR_VALUE      ((MAIR_DEVICE_NG << 8) | MAIR_NORMAL_WB)

#define TCR_T0SZ        (16UL << 0)
#define TCR_IRGN0_WB    (1UL << 8)
#define TCR_ORGN0_WB    (1UL << 10)
#define TCR_SH0_INNER   (3Ul << 12)
#define TCR_TG0_4K      (0UL << 14)
#define TCR_EPD1        (1UL << 23)
#define TCR_IPS_48BIT   (5UL << 32)
#define TCR_VALUE       (TCR_T0SZ | TCR_IRGN0_WB | TCR_ORGN0_WB | \
                         TCR_SH0_INNER | TCR_TG0_4K | TCR_EPD1 | TCR_IPS_48BIT)


// page tables
static uint64_t l0_table[512]     __attribute__((aligned(4096)));
static uint64_t l1_table[512]     __attribute__((aligned(4096)));
static uint64_t l2_dev_table[512] __attribute__((aligned(4096)));
static uint64_t l2_table[512]     __attribute__((aligned(4096)));


// clean the data cache by VA to the Point of Coherency
// MMU table walker reads from RAM and not from data cache
void clean_cache_range(uint64_t start, uint64_t size) {
  uint64_t line_size = 64;
  uint64_t end = start + size;
  start &= ~(line_size - 1);       // align down to cache line

  for (uint64_t addr = start; addr < end; addr += line_size) {
    asm volatile("dc cvac, %0" :: "r"(addr) : "memory");
  }

  asm volatile("dsb sy" ::: "memory");
}


void mmu_init(void) {
  
  // zeroing the tables
  for (int i = 0; i < 512; i++) {
    l0_table[i]     = 0;
    l1_table[i]     = 0;
    l2_dev_table[i] = 0;
    l2_table[i]     = 0;
  }

  // building L0 table, each entry covers the first 512GB
  l0_table[0] = (uint64_t)l1_table | DESC_VALID | DESC_TABLE;
  
  // build L1 table, L1[0] covers 0GB to 1GB and points at the device L2 table, L1[1] covers 1GB to 2GB and points at the kernel L2 table
  l1_table[0] = (uint64_t)l2_dev_table | DESC_VALID | DESC_TABLE;
  l1_table[1] = (uint64_t)l2_table | DESC_VALID | DESC_TABLE;

  // build the device for L2 table
  for (int i = 0; i < 512; i++) {
    uint64_t pa = (uint64_t)i * 0x200000UL;
    l2_dev_table[i] = pa | BLOCK_DEVICE;
  }

  // build the kernel L2 table
  for (int i = 0; i < 8; i++) {
    uint64_t pa = 0x40000000UL + (uint64_t)i * 0x200000UL;
    l2_table[i] = pa | BLOCK_NORMAL;
  }

  // clean tables to RAM so hardware walker can see them
  clean_cache_range((uint64_t)l0_table,     4096);
  clean_cache_range((uint64_t)l1_table,     4096);
  clean_cache_range((uint64_t)l2_dev_table, 4096);
  clean_cache_range((uint64_t)l2_table,     4096);

  // program system registers
  asm volatile("msr mair_el1,   %0" :: "r"(MAIR_VALUE) : "memory");
  asm volatile("msr tcr_el1,    %0" :: "r"(TCR_VALUE)  : "memory");
  asm volatile("msr ttbr0_el1,  %0" :: "r"((uint64_t)l0_table) : "memory");
  
  // invalidate TLBs and instruction cache
  asm volatile("tlbi vmalle1" ::: "memory");
  asm volatile("ic iallu"     ::: "memory");    // invalidate the icache
  asm volatile("dsb sy"       ::: "memory");
  asm volatile("isb");

  // enable MMU and Caches
  uint64_t sctlr;
  asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));
  sctlr |= (1UL << 0);  // M - MMU
  sctlr |= (1UL << 2);  // C - Data Cache
  sctlr |= (1UL << 12); // I - Instruction Cache
  asm volatile("msr sctlr_el1, %0" :: "r"(sctlr) : "memory");
  asm volatile("isb");
}
