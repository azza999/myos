#ifndef MYOS_MEMORY_HEAP_H
#define MYOS_MEMORY_HEAP_H

#include "types.h"
#include "pmm.h"
#include "paging.h"

void heap_init(void);
void* kmalloc(u32_t size);

#endif