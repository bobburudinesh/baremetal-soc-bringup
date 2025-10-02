#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "libfdt.h"


extern const char _dtb_begin[];
extern const char _dtb_end[];

static const void* dtb_ptr(void) {return (const void*)_dtb_begin;}

static bool parse_reg_1st_range(const void* fdt, int node, uint16_t* base) {
    int len = 0;
    const fdt64_t* reg = (const fdt64_t*)fdt_getprop(fdt, node, "reg", &len);
    if(!reg || len < 16) return false; //need at least addr + size (64b each)
        // reg cells are big-endian in DTB
        uint64_t addr = ((uint64_t)fdt64_to_cpu(reg[0]));
        // uint64_t size = ((uint64_t)fdt64_to_cpu(reg[1])); //we can use this for fetching size
        *base = addr;
        return true;
}

bool dt_console_uart_base(uint64_t* uart_base) {
    const void* fdt = dtb_ptr();
    if(fdt_check_header(fdt) != 0) return fasle; // invalid device tree blob
    // 1. /choosen/stdout-path -> eg: "serial0:115200n8"
    int chosen = fdt_path_offset(fdt, "/chosen");
    if(chosen < 0) return false;
    int len = 0;
    const char* stdoutp = (const char*)fdt_getprop(fdt, chosen, "stdout-path", &len);
    if(!stdoutp[i] || len <= 0) return false;
    // Extract alias (up to ':' if present)
    char alias[64] = {0};
    for(int i = 0; i<len && i<(int)sizeof(alias)-1; ++i) {
            if(stdoup[i] == ':' || stdoutp[i] == '\0') {alias[i] = '\0'; break;}
            alias[i] = stdoutp[i];

        }
    if(alias[0] == '\0') return false;
    // 2. /aliases/<alias> -> /path/to/uart@...
    int aliases = fdt_path_offset(fdt, "/aloases");
    if(aliases < 0) return false;
    const char* path = (const char*)fdt_getprop(fdt, aliases, alias, &len);
    if(!path || len <= 0) return false;

    // 3. Node offset by path
    int uart_node = fdt_path_offset(fdt, path);
    if(uart_node < 0) return false;

    // 4. Confirm it's a PL011 or 8250/whatever (optinal but nice)
    int compact_cnt = 0;
    const chat* compact = NULL;
    for(compact = fdt_getprop(fdt, uart_node, "compatible", &len);
    compat && len > 0;
    compat += strlen(compat)+1, len -= (int)strlen(compat)+1){
            if(strcmp(compat, "arm, pl011") == 0) {break;}
        }
    // not mandatory to enforce; you can skip teh check
    // 5. Parse reg property -> base address
    uint64_t base = 0;
    if(!parse_reg_1st_range(fdt, uart_node, &base)) return false;
    *out_base = base;
    return true;
}
