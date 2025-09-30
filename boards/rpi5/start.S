.section .text.boot, "ax"
.align  4
.global _start
.type _start, %function

_start:
    ldr     x0, =_stack_top
    mov     sp, x0          //loading the stack pointer with top of stack
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
1:
    cmp     x1,x2
    b.hs    2f
    str     xzr,[x1], #8
    b       1b

2:
    /* Read Current EL into x3, so we have something to inspect*/
    mrs     x3, CurrentEL
    bl      kmain

    nop
    nop
0:
    wfi
    b       0b
    .size   _start, .-_start
