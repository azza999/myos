#include "idt.h"
#include "screen.h"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

struct idt_entry {
    u16 offset_low;
    u16 selector;
    u8 zero;
    u8 type_attr;
    u16 offset_high;
} __attribute__((packed));

struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtr;

extern void isr0x30(void);
extern void isr0x0(void);

static void set_idt_gate(int n, u32 handler) {
	char buf[10] = {0,};
	itoa(n, buf);
	println(buf);
	itoa((int)handler, buf);
	println(buf);
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x10;
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

static void load_idt(void) {
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (u32)idt;
    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

void isr0x0_handler(void) {
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: INT 0");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x30_handler(void) {
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: INT 0x30");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void idt_init(void) {
    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, 0);
    }

    set_idt_gate(0x0, (u32)isr0x0);
    set_idt_gate(0x30, (u32)isr0x30);
    load_idt();
}