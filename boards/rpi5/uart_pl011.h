#pragma once
#include <stdint.h>
// RPI 5 dedicated boot UART on JST header
// MMIO base address for serial 10/uart10 as it is the port that is mapped to 3pin debug header on rpi5
#define RPI5_BOOT_UART_10_BASE     0x107D001000ULL

#define UART_DR                 0x000
#define UART_FR                 0x018
#define UART_RSR                0x004
#define UART_ILPR               0x020
#define UART_IBRD               0x024
#define UART_LCR_H              0x02C
#define UART_CR                 0x030
#define UART_IFLS               0x034
#define UART_IMSC               0x038
#define UART_RIS                0x03C
#define UART_MIS                0x040
#define UART_ICR                0x044
#define UART_DMACR              0x048

#define FR_TXFF               (1u << 5)   //Transmit FIFO Full
#define FR_BUSY                 (1u << 3)   // Transmit busy

