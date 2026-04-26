#include "idt.h"
#include "screen.h"
#include "types.h"

typedef struct {
    u16_t offset_low;
    u16_t selector;
    u8_t zero;
    u8_t type_attr;
    u16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    u16_t limit;
    u32_t base;
} __attribute__((packed)) idt_ptr_t;

static idt_entry_t s_idt[256];
static idt_ptr_t s_idtr;

extern void isr0x00(void);
extern void isr0x08(void);
extern void isr0x0d(void);
extern void isr0x0e(void);
extern void isr0x30(void);
extern void irq1(void);

static void set_idt_gate(int n, u32_t handler)
{
    s_idt[n].offset_low = handler & 0xFFFF;
    s_idt[n].selector = 0x08;
    s_idt[n].zero = 0;
    s_idt[n].type_attr = 0x8E;
    s_idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

static void clear_idt_gate(int n)
{
    s_idt[n].offset_low = 0;
    s_idt[n].selector = 0;
    s_idt[n].zero = 0;
    s_idt[n].type_attr = 0;
    s_idt[n].offset_high = 0;
}

static void load_idt(void)
{
    s_idtr.limit = sizeof(s_idt) - 1;
    s_idtr.base = (u32_t)s_idt;
    __asm__ volatile ("lidt %0" : : "m"(s_idtr));
}

void isr0x00_handler(void)
{
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #DE (INT 0x00)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x08_handler(void)
{
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #DF (INT 0x08)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x0d_handler(void)
{
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #GP (INT 0x0D)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x0e_handler(void)
{
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: #PF (INT 0x0E)");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void isr0x30_handler(void)
{
    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_RED));
    println("EXCEPTION: INT 0x30");

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void idt_init(void)
{
    int i;

    for (i = 0; i < 256; i++) {
        clear_idt_gate(i);
    }

    set_idt_gate(0x00, (u32_t)isr0x00);
    set_idt_gate(0x08, (u32_t)isr0x08);
    set_idt_gate(0x0D, (u32_t)isr0x0d);
    set_idt_gate(0x0E, (u32_t)isr0x0e);
    set_idt_gate(0x30, (u32_t)isr0x30);
    set_idt_gate(0x21, (u32_t)irq1);

    load_idt();
}
