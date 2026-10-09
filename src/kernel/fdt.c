#include "kernel/fdt.h"

static unsigned int read_be32(const unsigned char *p) {
    return ((unsigned int)p[0] << 24) |
           ((unsigned int)p[1] << 16) |
           ((unsigned int)p[2] << 8) |
           ((unsigned int)p[3]);
}

int fdt_validate(const void *dtb, size_t *total_size) {

    if (dtb == 0 || total_size == 0) return 0;

    const unsigned char *p = (const unsigned char *)dtb;

    // FDT magic is 0xd00dfeed, (big-endian)
    if (p[0] != 0xD0 ||
        p[1] != 0x0D ||
        p[2] != 0xFE ||
        p[3] != 0xED) {
        return 0;
    }

    unsigned int size = read_be32(&p[4]);

    // standart FDT header occupies atleast 40 bytes
    if (size < FDT_HEADER_SIZE)
        return 0;

    *total_size = (size_t)size;
    return 1;
}