#ifndef KERNEL_EXCEPTIONS_H
#define KERNEL_EXCEPTIONS_H

void exceptions_init(void);
void sync_exception_handler(void);
void irq_handler(void);

#endif