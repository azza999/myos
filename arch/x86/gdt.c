#include "gdt.h"
#include "screen.h"
#include "types.h"

typedef struct {
    u16_t limit_low;
    u16_t base_low;
    u8_t base_middle;
    u8_t access;
    u8_t granularity;
    u8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    u16_t limit;
    u32_t base;
} __attribute__((packed)) gdt_ptr_t;

extern void gdt_flush(u32_t gdtr_addr);

static gdt_entry_t s_gdt[3];
static gdt_ptr_t s_gdtr;

static void set_gdt_gate(int n, u32_t base, u32_t limit, u8_t access, u8_t granularity)
{
    s_gdt[n].base_low = base & 0xFFFF;
    s_gdt[n].base_middle = (base >> 16) & 0xFF;
    s_gdt[n].base_high = (base >> 24) & 0xFF;
    s_gdt[n].limit_low = limit & 0xFFFF;
    s_gdt[n].granularity = (limit >> 16) & 0x0F;
    s_gdt[n].granularity |= granularity & 0xF0;
    s_gdt[n].access = access;
}

void gdt_init(void)
{
    s_gdtr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    s_gdtr.base = (u32_t)s_gdt;

    set_gdt_gate(0, 0, 0, 0, 0);
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((u32_t)&s_gdtr);
}
