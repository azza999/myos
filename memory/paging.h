#ifndef MYOS_MEMORY_PAGING_H
#define MYOS_MEMORY_PAGING_H

#include "types.h"

#define PAGE_SIZE       4096

#define PAGE_PRESENT    0x1
#define PAGE_RW         0x2
#define PAGE_USER       0x4

void paging_init(void);

void paging_map_page(u32_t virtual_addr, u32_t physical_addr, u32_t flags);

#endif