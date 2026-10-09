
LLVM_BIN := $(shell brew --prefix llvm)/bin
CC       := $(LLVM_BIN)/clang
OBJCOPY  := $(LLVM_BIN)/llvm-objcopy

TARGET   := --target=aarch64-none-elf
CFLAGS   := $(TARGET) -mcpu=cortex-a72 \
            -ffreestanding -fno-builtin \
            -fno-stack-protector -fno-pic -fno-pie \
            -mgeneral-regs-only \
            -Wall -Wextra -O2 -g \
            -I src/include

LDFLAGS  := -nostdlib -fuse-ld=lld \
            -Wl,-T,linker.ld \
            -Wl,--build-id=none \
            -Wl,-Map,build/snoopy.map

SOURCES  := src/arch/aarch64/entry.S \
            src/drivers/uart/pl011.c \
            src/kernel/main.c

.PHONY: all run clean inspect

all: build/snoopy.img

build/snoopy.elf: $(SOURCES) linker.ld \
                src/include/drivers/uart.h
	mkdir -p build
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@

build/snoopy.img: build/snoopy.elf
	$(OBJCOPY) -O binary $< $@

run: build/snoopy.img
	qemu-system-aarch64 \
		-machine virt \
		-cpu cortex-a72 \
		-m 512M \
		-nographic \
		-monitor none \
		-serial stdio \
		-kernel build/snoopy.img

inspect: build/snoopy.elf
	$(LLVM_BIN)/llvm-objdump -d $<

clean:
	rm -rf build
