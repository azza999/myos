#ifndef MYOS_MULTIBOOT_H
#define MYOS_MULTIBOOT_H

#include "types.h"

typedef struct multiboot_info {
    u32_t flags;

    u32_t mem_lower;
    u32_t mem_upper;

    /* 나머지는 지금 필요 없으니까 생략 */
} multiboot_info_t;

#endif