BITS 32

global gdt_flush

gdt_flush:
    mov eax, [esp + 4]    ; 첫 번째 인자: gdt_ptr 주소
    lgdt [eax]

    mov ax, 0x10          ; kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush_cs    ; far jump로 CS 갱신

.flush_cs:
    ret