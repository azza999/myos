#include "idt.h"
#include "screen.h"
#include "pic.h"
#include "screen.h"
#include "io.h"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct {
    u16 offset_low;
    u16 selector;
    u8 zero;
    u8 type_attr;
    u16 offset_high;
} __attribute__((packed)) idt_entry;

typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_ptr;

static idt_entry idt[256];
static idt_ptr idtr;

extern void isr0x00(void);
extern void isr0x08(void);
extern void isr0x0d(void);
extern void isr0x0e(void);
extern void isr0x30(void);
extern void irq1(void);

static void set_idt_gate(int n, u32 handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;      // 현재 네 GDT 기준 code selector
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;     // present=1, ring0, 32-bit interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

static void clear_idt_gate(int n) {
    idt[n].offset_low = 0;
    idt[n].selector = 0;
    idt[n].zero = 0;
    idt[n].type_attr = 0;
    idt[n].offset_high = 0;
}

static void load_idt(void) {
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (u32)idt;
    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

void isr0x00_handler(void) {
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #DE (INT 0x00)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x08_handler(void) {
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #DF (INT 0x08)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x0d_handler(void) {
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #GP (INT 0x0D)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x0e_handler(void) {
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #PF (INT 0x0E)");

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

// irq1 => keyboard interrupt
void irq1_handler(void) {
    
    unsigned char scancode = inb(0x60);
    static unsigned char key_down[256];
    static unsigned char extended = 0;

    if (scancode == 0xE0) {
        extended = 1;
        return;
    }

    if (scancode & 0x80) {
        unsigned char code = scancode & 0x7F;

        if (extended)
            key_down[code + 128] = 0;
        else
            key_down[code] = 0;

    } else {
        if (extended)
            key_down[scancode + 128] = 1;
        else
            key_down[scancode] = 1;
    }

    extended = 0;

    pic_send_eoi(1);
}

void idt_init(void) {
    for (int i = 0; i < 256; i++) {
        clear_idt_gate(i);
    }

    // mandatory system interrupts
    set_idt_gate(0x00, (u32)isr0x00);
    set_idt_gate(0x08, (u32)isr0x08);
    set_idt_gate(0x0D, (u32)isr0x0d);
    set_idt_gate(0x0E, (u32)isr0x0e);
    set_idt_gate(0x30, (u32)isr0x30);

    set_idt_gate(0x21, (u32)irq1);

    load_idt();
}