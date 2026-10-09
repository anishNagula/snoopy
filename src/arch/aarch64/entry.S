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
    // x0 contains DTB address on entry
    // preserve it before using registers for initialization
    mov     x19, x0

    // clear .bss in 8-byte units
    adrp    x1, __bss_start
    add     x1, x1, :lo12:__bss_start

    adrp    x2, __bss_end
    add     x2, x2, :lo12:__bss_end

    mov     x3, xzr

.Lclear_bss:
    cmp     x1, x2
    b.hs    .Lbss_done
    str     x3, [x1], #8
    b       .Lclear_bss

.Lbss_done:
    // init stack after clearing .bss
    adrp    x1, __stack_top
    add     x1, x1, :lo12:__stack_top
    mov     sp, x1

    // pass preserved device-tree pointer to C
    mov x0, x19
    bl kernel_main

.Lhalt:
    wfe
    b       .Lhalt