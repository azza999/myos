AS = nasm
CC = gcc
LD = ld

BUILD_DIR = build

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -c \
	-Iinclude -Idrivers -Iarch/x86

LDFLAGS = -m elf_i386 -T linker.ld

OBJS = \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/stdlib.o \
	$(BUILD_DIR)/gdt_flush.o \
	$(BUILD_DIR)/interrupt.o \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/screen.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/irq.o \
	$(BUILD_DIR)/gdt.o \
	$(BUILD_DIR)/pic.o \
	$(BUILD_DIR)/keyboard.o

all: myos.iso

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot/boot.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/stdlib.o: include/stdlib.c include/stdlib.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/gdt_flush.o: boot/gdt_flush.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/interrupt.o: arch/x86/interrupt.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/kernel.o: kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/screen.o: drivers/screen.c drivers/screen.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/idt.o: arch/x86/idt.c arch/x86/idt.h drivers/screen.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/irq.o: arch/x86/irq.c arch/x86/irq.h arch/x86/pic.h arch/x86/io.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/gdt.o: arch/x86/gdt.c arch/x86/gdt.h drivers/screen.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/pic.o: arch/x86/pic.c arch/x86/pic.h arch/x86/io.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c drivers/keyboard.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

myos.iso: $(BUILD_DIR)/kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp $(BUILD_DIR)/kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso iso

run: myos.iso
	qemu-system-i386 -cdrom myos.iso -no-reboot -no-shutdown -d int,cpu_reset,guest_errors -monitor stdio

clean:
	rm -rf $(BUILD_DIR)
	rm -f myos.iso
	rm -f iso/boot/kernel.elf
	rm -f iso/boot/grub/grub.cfg

.PHONY: all run clean
