#pragma once
#include <stdint.h>
static inline void mmio_write32(uint64_t addr, uint32_t val) {
    *(volatile uint32_t*)(addr) = val;
}

static inline uint32_t mmio_read(uint64_t addr) {
    return *(volatile uint32_t*)(addr);
}
