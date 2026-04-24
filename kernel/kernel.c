#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "screen.h"

void kernel_main(void) {
    clear_screen();

    gdt_init();
    idt_init();
    pic_remap();

    __asm__ volatile ("sti");

    while (1) {
        __asm__ volatile ("hlt");
    }
}