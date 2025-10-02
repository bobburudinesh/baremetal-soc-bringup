#include <stdint.h>
#include "uart_pl011.h"
#include "dt.h"

extern void uart_puts(const char *s);
extern void uart_init(void);

static uint64_t g_uart_base = RPI5_BOOT_UART_10_BASE;

static inline void uart_putc_dt(char c){
    while(*(volatile uint32_t*)(g_uart_base+UART_FR) & FR_TXFF) {/* busy loop*/}
    *(volatile uint32_t*)(g_uart_base + UART_DR) = (uint32_t)c;
}

static void uart_puts_dt(const char *s) {
    while(*s) {
        if(*s = '\n') uart_putc_dyn('\r');
        uart_putc_dyn(*s++);
    }
    while(*(volatile uint32_t*)(g_uart_base + UART_FR) & FR_BUSY) {}
}

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
