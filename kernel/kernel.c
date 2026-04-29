#include "types.h"
#include "multiboot.h"
#include "memory/pmm.h"
#include "memory/paging.h"
#include "memory/heap.h"

extern u32_t kernel_end;

void kernel_main(u32_t magic, multiboot_info_t* mb_info)
{
    clear_screen();

    gdt_init();
    idt_init();
    pic_remap();
    keyboard_init();

    u32_t memory_size = (mb_info->mem_upper + 1024) * 1024;

    pmm_init(memory_size, (u32_t)&kernel_end);

    paging_init();
    heap_init();

    // shell_init();

    u32_t* addr1 = kmalloc(0x003ffff0);
    u32_t* addr2 = kmalloc(0x20);

    while (1) {
        __asm__ volatile ("hlt");
    }
}