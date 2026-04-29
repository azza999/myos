#include "memory/paging.h"
#include "memory/pmm.h"
#include "screen.h"

#define PAGE_ENTRIES 1024

extern void paging_load_directory(u32_t* page_directory);
extern void paging_enable(void);

static u32_t* current_directory = 0;

static u32_t page_directory[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

static void clear_page(u32_t* page)
{
    for (u32_t i = 0; i < PAGE_ENTRIES; i++) {
        page[i] = 0;
    }
}

void paging_map_page(u32_t virtual_addr, u32_t physical_addr, u32_t flags)
{
    u32_t directory_index = virtual_addr >> 22;
    u32_t table_index = (virtual_addr >> 12) & 0x3FF;

    u32_t* page_table;

    if (!(current_directory[directory_index] & PAGE_PRESENT)) {
        u32_t page_table_addr = pmm_alloc_page();

        if (page_table_addr == 0) {
            println("PMM alloc failed for page table");
            return;
        }

        page_table = (u32_t*)page_table_addr;
        clear_page(page_table);

        current_directory[directory_index] =
            page_table_addr | PAGE_PRESENT | PAGE_RW;
    } else {
        page_table = (u32_t*)(current_directory[directory_index] & 0xFFFFF000);
    }

    page_table[table_index] =
        (physical_addr & 0xFFFFF000) | (flags & 0xFFF);
}

void paging_init(void)
{
    current_directory = page_directory;

    clear_page(page_directory);

    for (u32_t addr = 0; addr < 0x00400000; addr += PAGE_SIZE) {
        paging_map_page(addr, addr, PAGE_PRESENT | PAGE_RW);
    }

    paging_load_directory(page_directory);
    paging_enable();

    println("Paging enabled");
}