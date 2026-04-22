AS = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -c
LDFLAGS = -m elf_i386 -T linker.ld

all: myos.iso

boot.o: boot.asm
	$(AS) -f elf32 boot.asm -o boot.o

interrupt.o: interrupt.asm
	$(AS) -f elf32 interrupt.asm -o interrupt.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

screen.o: screen.c screen.h
	$(CC) $(CFLAGS) screen.c -o screen.o

idt.o: idt.c idt.h screen.h
	$(CC) $(CFLAGS) idt.c -o idt.o

kernel.elf: boot.o interrupt.o kernel.o screen.o idt.o linker.ld
	$(LD) $(LDFLAGS) -o kernel.elf boot.o interrupt.o kernel.o screen.o idt.o

myos.iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso iso

run: myos.iso
	qemu-system-i386 -cdrom myos.iso -no-reboot -no-shutdown -d int,cpu_reset,guest_errors -monitor stdio

clean:
	rm -f *.o *.elf *.iso
	rm -rf iso/boot/kernel.elf iso/boot/grub/grub.cfg

.PHONY: all run clean