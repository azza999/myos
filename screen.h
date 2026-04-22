#ifndef SCREEN_H
#define SCREEN_H

typedef enum {
    COLOR_BLACK = 0x0,
    COLOR_BLUE = 0x1,
    COLOR_GREEN = 0x2,
    COLOR_CYAN = 0x3,
    COLOR_RED = 0x4,
    COLOR_MAGENTA = 0x5,
    COLOR_BROWN = 0x6,
    COLOR_LIGHT_GREY = 0x7,
    COLOR_DARK_GREY = 0x8,
    COLOR_LIGHT_BLUE = 0x9,
    COLOR_LIGHT_GREEN = 0xA,
    COLOR_LIGHT_CYAN = 0xB,
    COLOR_LIGHT_RED = 0xC,
    COLOR_LIGHT_MAGENTA = 0xD,
    COLOR_YELLOW = 0xE,
    COLOR_WHITE = 0xF
} vga_color;

unsigned char make_color(vga_color bg, vga_color fg);
void set_color(unsigned char new_color);

void clear_screen(void);
void move_cursor(unsigned char x, unsigned char y);

void put_char(char c);
void print(const char* str);
void println(const char* str);

#endif