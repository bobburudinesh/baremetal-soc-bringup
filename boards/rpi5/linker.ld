ENTRY(_start)

SECTIONS {
    . = 0x0000000000080000;
    .text : {
        *(.text.boot)
        *(.text*)
        *(.rodata*)
    }
    .data : {
        *(.data*)
    }
    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }
    . = ALIGN(16);
    _stack_top = . + 0x4000; /* 16KB of Stack*/
}
