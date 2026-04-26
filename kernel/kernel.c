#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "screen.h"
#include "stdlib.h"
#include "keyboard.h"

void kernel_main(void)
{
    key_event_t ev;

    clear_screen();
    gdt_init();
    idt_init();
    pic_remap();
    keyboard_init();

    __asm__ volatile ("sti");

    shell_init();

    while (1) {
        __asm__ volatile ("hlt");
    }
}
