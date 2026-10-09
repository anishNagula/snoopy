.section .text.boot, "ax"
.global _image_start
.global _start

_image_start:
    // AArch64 Image header, 64 bytes
    b       _start
    nop
    .quad   0x80000     // text_offset
    .quad   0           // image_size ~ not specified
    .quad   0           // flags
    .quad   0           // reserved
    .quad   0           // reserved
    .quad   0           // reserved
    .word   0x644d5241  // ARM\x64 magic
    .word   0           // reserved

_start:
    // establish 16-byte-aligned stack before calling C
    adrp    x0, __stack_top
    add     x0, x0, :lo12:__stack_top
    mov     sp, x0

    // enter the generic kernel
    bl      kernel_main

1:
    wfe
    b   1b