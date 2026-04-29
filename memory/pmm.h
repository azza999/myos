#ifndef MYOS_MEMORY_PMM_H
#define MYOS_MEMORY_PMM_H

#include "types.h"

#define PMM_PAGE_SIZE 4096

void pmm_init(u32_t memory_size, u32_t kernel_end);

u32_t pmm_alloc_page(void);
void pmm_free_page(u32_t addr);

u32_t pmm_get_total_pages(void);
u32_t pmm_get_used_pages(void);
u32_t pmm_get_free_pages(void);

#endif