#ifndef SNOOPY_KERNEL_FDT_H
#define SNOOPY_KERNEL_FDT_H

#include <stddef.h>

#define FDT_HEADER_SIZE 40u

// validate FDT header supplied at kernel entry

// returns 1 on success, 0 on failure
// on success stores the total FDT size in *total_size

int fdt_validate(const void *dtb, size_t *total_size);


#endif