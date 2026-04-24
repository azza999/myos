BITS 32

global isr0x00
global isr0x08
global isr0x0d
global isr0x0e
global isr0x30

extern isr0x00_handler
extern isr0x08_handler
extern isr0x0d_handler
extern isr0x0e_handler
extern isr0x30_handler

section .text

isr0x00:
    cli
    pusha
    call isr0x00_handler
.hang00:
    hlt
    jmp .hang00

isr0x08:
    cli
    pusha
    call isr0x08_handler
.hang08:
    hlt
    jmp .hang08

isr0x0d:
    cli
    pusha
    call isr0x0d_handler
.hang0d:
    hlt
    jmp .hang0d

isr0x0e:
    cli
    pusha
    call isr0x0e_handler
.hang0e:
    hlt
    jmp .hang0e

isr0x30:
    cli
    pusha
    call isr0x30_handler
.hang30:
    hlt
    jmp .hang30

; connect irq

global irq1
extern irq1_handler

irq1:
    pusha
    call irq1_handler
    popa
    iretd