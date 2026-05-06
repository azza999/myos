#ifndef MYOS_TYPES_H
#define MYOS_TYPES_H

#define NULL ((void*)0)

typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;
typedef signed char    i8_t;
typedef signed short   i16_t;
typedef signed int     i32_t;

typedef struct {
    u16_t x;
    u16_t y;
} position_t;

#endif
