BITS 32
global _start
extern kernel_main

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
_start:
    mov esp, stack_top
    mov ebp, 0
    push ebx
    push eax

    call kernel_main

.hang:
    cli
    hlt
    jmp .hang