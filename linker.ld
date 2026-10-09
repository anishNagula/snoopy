OUTPUT_ARCH(aarch64)
ENTRY(_image_start)

SECTIONS
{
    . = 0x40080000;

    .text : ALIGN(16)
    {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    .rodata : ALIGN(16)
    {
        *(.rodata .rodata.*)
    }

    .data : ALIGN(16)
    {
        *(.data .data.*)
    }

    .bss  (NOLOAD) : ALIGN(16)
    {
       . = ALIGN(16);
       __bss_start = .;

       *(.bss .bss.*)
       *(COMMON)

       // reserve a 16KiB bootstrap stack
       . = ALIGN(16);
       . += 16384;
       __stack_top = .;

       . = ALIGN(16);
       __bss_end = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.note .note.*)
        *(.eh_frame .eh_frame.*)
    }
}