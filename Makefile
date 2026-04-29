AS = nasm
CC = gcc
LD = ld

BUILD_DIR = build

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -c \
	-I. -Ilib -Idrivers -Iarch/x86 -Imemory -Ishell 

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
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/shell.o \
	$(BUILD_DIR)/pmm.o \
	$(BUILD_DIR)/paging.o \
	$(BUILD_DIR)/paging_asm.o \
	$(BUILD_DIR)/heap.o

all: myos.iso

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot/boot.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/gdt_flush.o: arch/x86/gdt_flush.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/interrupt.o: arch/x86/interrupt.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/stdlib.o: lib/stdlib.c lib/stdlib.h lib/types.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/screen.o: drivers/screen.c drivers/screen.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c drivers/keyboard.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/gdt.o: arch/x86/gdt.c arch/x86/gdt.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/idt.o: arch/x86/idt.c arch/x86/idt.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/irq.o: arch/x86/irq.c arch/x86/irq.h arch/x86/pic.h arch/x86/io.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/pic.o: arch/x86/pic.c arch/x86/pic.h arch/x86/io.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/shell.o: shell/shell.c shell/shell.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/pmm.o: memory/pmm.c memory/pmm.h lib/types.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/paging.o: memory/paging.c memory/paging.h memory/pmm.h lib/types.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/paging_asm.o: arch/x86/paging.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/heap.o: memory/heap.c memory/heap.h memory/paging.h lib/types.h | $(BUILD_DIR)
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
	rm -rf iso
	rm -f myos.iso

.PHONY: all run clean