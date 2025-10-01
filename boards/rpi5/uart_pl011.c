#include <stdint.h>
#include <math.h>
#include "mmio.h"
#include "uart_pl011.h"

// Clock and baud for this peripheral are already configured by closed firmware from broadcom

static inline void uart_enable_minimal(void){
  mmio_write32(RPI5_BOOT_UART_10_BASE + UART_CR, 0);
  mmio_write32(RPI5_BOOT_UART_10_BASE + UART_ICR, UART_ICR_ALL);
  mmio_write32(RPI5_BOOT_UART_10_BASE + UART_LCR_H, UART_LCRH_WLEN_8);
  mmio_write32(RPI5_BOOT_UART_10_BASE + UART_CR, UART_CR_UARTEN | UART_CR_TXE);
}
void uart_init(void){ uart_enable_minimal(); }

static inline void uart_putc(char c) {
    // Wait while TX FIFO is full
    while(mmio_read32(RPI5_BOOT_UART_10_BASE + UART_FR) & FR_TXFF) {/* spin */}
    mmio_write32((RPI5_BOOT_UART_10_BASE + UART_DR), (uint32_t)(c));
}

static inline void uart_flush(void) {
    // wait for transmitter not busy
    while(mmio_read32(RPI5_BOOT_UART_10_BASE + UART_FR) & FR_BUSY) {/* spin */}
}

void uart_puts(const char *s) {
    while(*s) {
        if(*s == '\n') uart_putc('\r');
        uart_putc(*s++);
        uart_flush();
    }
}
