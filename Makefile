CROSS   ?= aarch64-linux-gnu
CC      := $(CROSS)-gcc
LD      := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy

CFLAGS := -O0 -g -nostdlib -Wall -Wextra -ffreestanding -nostartfiles
LDFLAGS := -T boards/rpi5/linker.ld -nostdlib -nostartfiles

OUT := build/rpi5
OBJ := $(OUT)/start.o
ELF := $(OUT)/hello.elf
BIN := $(OUT)/kernel8.img

all: $(BIN)

$(OUT):
	mkdir -p $(OUT)
$(OUT)/start.o: boards/rpi5/start.S | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@
$(ELF): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)
clean:
	rm -rf build
.PHONY: all clean
