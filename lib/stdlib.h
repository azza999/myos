#include "types.h"

#ifndef MYOS_STDLIB_H
#define MYOS_STDLIB_H

void itoa(u32_t val, char* buf);
void itohex(u32_t val, char* buf);
void memset(void* dst, u8_t val, u8_t size);


#endif
