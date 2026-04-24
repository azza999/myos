#include "gdt.h"
#include "screen.h"


typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct {
    u16 limit_low;      // limit 0~15
    u16 base_low;       // base 0~15
    u8  base_middle;    // base 16~23
    u8  access;         // access flags
    u8  granularity;    // flags + limit 16~19
    u8  base_high;      // base 24~31
} __attribute__((packed)) gdt_entry;

typedef struct
{
	u16 limit;
    u32 base;
} __attribute__((packed)) gdt_ptr;

extern void gdt_flush(u32 gdtr_addr);

static gdt_entry gdt[3];
static gdt_ptr gdtr;

void set_gdt_gate(int n, u32 base, u32 limit, u8 access, u8 granularity);
void gdt_init(void);

void set_gdt_gate(int n, u32 base, u32 limit, u8 access, u8 granularity)
{
    gdt[n].base_low    = base & 0xFFFF;
    gdt[n].base_middle = (base >> 16) & 0xFF;
    gdt[n].base_high   = (base >> 24) & 0xFF;

    gdt[n].limit_low   = limit & 0xFFFF;

    gdt[n].granularity = (limit >> 16) & 0x0F;
    gdt[n].granularity |= granularity & 0xF0;

    gdt[n].access = access;
}

void gdt_init(void)
{
    gdtr.limit = (sizeof(gdt_entry) * 3) - 1;
    gdtr.base  = (u32)gdt;

    set_gdt_gate(0, 0, 0, 0, 0);                  // null descriptor
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);   // kernel code
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   // kernel data

    gdt_flush((u32)&gdtr);
}