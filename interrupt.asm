BITS 32

global isr0x30, isr0x0
extern isr0x30_handler, isr0x0_handler

section .text
isr0x30:
    cli
    call isr0x30_handler

isr0x0:
	cli
	call isr0x0_handler

.hang:
    hlt
    jmp .hang