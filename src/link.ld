ENTRY(_start)

SECTIONS {
  . = 0x40000000;

  /* .text.boot first */
  .text : {
    KEEP(*(.text.boot))
    KEEP(*(.text.vectors))
    *(.text .text.*)
  }

  . = ALIGN(8);
  .rodata : {
    *(.rodata .rodata.*)
  }

  . = ALIGN(8);
  .data : {
    *(.data .data.*)
  }

  . = ALIGN(4096);
  .bss : {
    __bss_start = .;
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN(4096);
    __bss_end = .;
  }

  /* stack: 16KB */
  . = ALIGN(16);
  __stack_bottom = .;
  . += 0x4000;
  _stack_top = .;


  /* heap: 1MB */
  . = ALIGN(8);
  __heap_start = .;
  . += 0x100000;
  __heap_end = .;
}
