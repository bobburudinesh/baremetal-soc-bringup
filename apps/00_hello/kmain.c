#include <stdint.h>
#include "uart_pl011.h"

extern void uart_puts(const char *s);
extern void uart_init(void);
static inline uint64_t get_currentEL(void) {
    uint64_t el;
    __asm__ volatile("mrs %0, CurrentEL":"=r"(el));
    return el;
}

void kmain(void) {
    uart_init();
    uart_puts("\n\nhello world from bare metal arm cortex a76 on raspberry pi 5\n");
    uint64_t el = get_currentEL();
    // CurrentEL encodes EL in bits [3:2], e.g., 0x8 -> EL2, 0x4 -> EL1
    const char* el_str = (el == 0x8) ? "EL2" : (el == 0x4) ? "EL1" : "unknown";
    uart_puts("CurrentEL = "); uart_puts(el_str); uart_puts("\n");

    uart_puts("done. parking CPU.\n");
    for(;;){ __asm__ volatile("wfi"); }
}
