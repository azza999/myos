#include "screen.h"
#include "idt.h"
#include "stdlib.h"

void kernel_main(void) {
    clear_screen();
    set_color(make_color(COLOR_BLACK, COLOR_WHITE));
    println("kernel start");
    println("loading idt...");

    idt_init();


    int a = 1;
    int b = 0;
    int c = a / b;
    println((char*)c);

    while (1) {
        asm("hlt");
    }

    
}