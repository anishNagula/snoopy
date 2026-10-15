#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include <stddef.h>
#include <stdint.h>

// align a size up to the nearest multiple of `align`, it must be a power of 2
#define ALIGN_UP(n, align) (((size_t)(n) + (align) - 1) & ~((size_t)(align) - 1))

// minimum allocation alignment, aarch64 requires 8 bytes for 64-bit stores (bump / free-list allocators both honour it)
#define HEAP_ALIGN 8


// call once before any kmalloc from main
void heap_init(void);

// allocate size bytes from heap, returns ptr to at least `size` bytes of usable memory, NULL if OOM
void *kmalloc(size_t size);

// release memory previously returned by kmalloc
void kfree(void *ptr);

// print allocator state to UART for debugging
void heap_dump(void);

#endif