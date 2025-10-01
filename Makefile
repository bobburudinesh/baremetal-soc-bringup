
CROSS    ?= aarch64-linux-gnu
CC       := $(CROSS)-gcc
OBJCOPY  := $(CROSS)-objcopy

# --- Compile / Link flags ---
CFLAGS  := -O0 -g -ffreestanding -nostdlib -nostartfiles -Wall -Wextra -MMD -MP
CFLAGS  += -Icommon -Iboards/rpi5
# (optional) help linker drop unused sections once we grow:
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS := -T boards/rpi5/linker.ld -nostdlib -nostartfiles -Wl,--gc-sections

# --- Files / Paths ---
OUT   := build/rpi5
ELF   := $(OUT)/hello.elf
BIN   := $(OUT)/kernel8.img

ASMOBJ := $(OUT)/start.o
CSRCS  := boards/rpi5/uart_pl011.c apps/00_hello/kmain.c
COBJS  := $(patsubst %.c,$(OUT)/%.o,$(CSRCS))
DEPS   := $(COBJS:.o=.d) $(ASMOBJ:.o=.d)

# --- Default ---
all: $(BIN)

# --- Rules ---
$(OUT):
	@mkdir -p $(OUT)/boards/rpi5 $(OUT)/apps/00_hello

$(OUT)/start.o: boards/rpi5/start.S | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/%.o: %.c | $(OUT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(ASMOBJ) $(COBJS)
	$(CC) $(CFLAGS) $(ASMOBJ) $(COBJS) $(LDFLAGS) -o $@

$(BIN): $(ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

# --- Utilities (optional but handy) ---
size: $(ELF)
	@ls -lh $(BIN)
	@$(CROSS)-nm $(ELF) | egrep ' kmain$| uart_(init|puts|putc)$$' || true

clean:
	rm -rf build

.PHONY: all clean size
-include $(DEPS)
