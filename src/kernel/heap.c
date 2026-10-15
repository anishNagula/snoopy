#include "kernel/heap.h"
#include "drivers/uart.h"
#include <stdint.h>

extern char __heap_start[];
extern char __heap_end[];

// internal state
static block_header_t *heap_head;         // first block in the chain


// private helpers
static block_header_t *write_header(char *addr, size_t size, int is_free, block_header_t *next) {
    block_header_t *hdr = (block_header_t * )addr;
    hdr->size = size;
    hdr->is_free = is_free;
    hdr->next = next;

    return hdr;
}

// payload pointer from header, bytes just after the header struct
static inline void *payload(block_header_t *hdr) {
    return (char *)hdr + HEADER_SIZE;
}

// header from payload pointer, bytes just before payload
static inline block_header_t *header(void *ptr) {
    return (block_header_t *)((char *)ptr - HEADER_SIZE);
}


// public APIs
void heap_init(void) {
    size_t total = (size_t)(__heap_end - __heap_start);
    size_t payload_size = total - HEADER_SIZE;

    heap_head = write_header(__heap_start, payload_size, 1, NULL);
    kprint("[heap] init: free-list allocator\n");
}


void *kmalloc(size_t size) {

    if (size == 0) return NULL;

    // round upto 8 byte alignment to satisfy aarch64 data alignment requirements.
    size = ALIGN_UP(size, HEAP_ALIGN);

    block_header_t *cur = heap_head;
    while (cur != NULL) {
        if (cur->is_free && cur->size >= size) {
            size_t remainder = cur->size - size;
            if (remainder > HEADER_SIZE + HEAP_ALIGN) {
                char *split_addr = (char *)payload(cur) + size;
                block_header_t *new_block = write_header(
                    split_addr,
                    remainder - HEADER_SIZE,
                    1,
                    cur->next
                );
                cur->size = size;
                cur->next = new_block;
            }

            cur->is_free = 0;
            return payload(cur);
        }

        cur = cur->next;
    }

    kprint("[heap] OOM: kmalloc returning NULL\n");
    return NULL;
}


void kfree(void *ptr) {
    if (ptr == NULL) return;

    block_header_t *hdr = header(ptr);
    hdr->is_free = 1;

    if (hdr->next != NULL && hdr->next->is_free) {
        hdr->size += HEADER_SIZE + hdr->next->size;
        hdr->next = hdr->next->next;
    }
}

void heap_dump(void) {
    kprint("[heap] --- free-list dump ---\n");
    block_header_t *cur = heap_head;
    int idx = 0;
    while (cur != NULL) {
        kprint("[heap] block ");
        char ibuf[8];
        int n = idx, pos = 7;
        ibuf[pos] = '\0';
        if (n == 0) { ibuf[--pos] = '0'; }
        while (n > 0) { ibuf[--pos] = '0' + (n % 10); n /= 10; }
        kprint(&ibuf[pos]);
        kprint(": size=");
        kprint(cur->is_free ? " FREE\n" : " USED\n");
        cur = cur->next;
        idx++;
    }

    kprint("[heap] --------------------\n");
}