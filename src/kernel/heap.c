#include "kernel/heap.h"
#include "drivers/uart.h"
#include <stdint.h>

extern char __heap_start[];
extern char __heap_end[];

// internal state
static char *bump_ptr;          // next free byte


// public APIs
void heap_init(void) {
    bump_ptr = __heap_start;

    kprint("[heap] init: bump allocator\n");
    kprint("[heap] start: 0x");
    kprint("\n");
}


void *kmalloc(size_t size) {

    if (size == 0) return NULL;

    // round upto 8 byte alignment to satisfy aarch64 data alignment requirements.
    size = ALIGN_UP(size, HEAP_ALIGN);

    if (bump_ptr + size > __heap_end) {
        kprint("[heap] OOM: kmalloc returning NULL\n");
        return NULL;
    }

    void *ptr = bump_ptr;
    bump_ptr += size;
    return ptr;
}


void kfree(void *ptr) {
    // bump allocate cannot free individual blocks, accepting pointer silently is intentional.
    (void)ptr;
}

void heap_dump(void) {
    kprint("[heap] bump allocator: free from bump_ptr to __heap_end\n");
}