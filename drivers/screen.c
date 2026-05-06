/*
cursor와 pointer가 있음.
cursor는 깜빡이는 거, pointer는 현재 글자를 출력할 수 있는 위치.
*/

#include "screen.h"
#include "types.h"
#include "stdlib.h"
#include "arch/x86/io.h"

static volatile char* s_vga = (volatile char*)0xB8000;

static position_t pointer = {0};

static unsigned char s_color = 0x0F;

#define VGA_WIDTH 80
#define VGA_HEIGHT 24
#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5

static inline void outb(u16_t port, u8_t value);

void move_cursor(position_t cur) {
    u16_t pos = cur.y * VGA_WIDTH + cur.x;

    outb(VGA_CTRL_REGISTER, 0x0F);
    outb(VGA_DATA_REGISTER, (u8_t)(pos & 0xFF));

    outb(VGA_CTRL_REGISTER, 0x0E);
    outb(VGA_DATA_REGISTER, (u8_t)((pos >> 8) & 0xFF));
}

void resize_cursor(u8_t cursor_start, u8_t cursor_end) {
    outb(VGA_CTRL_REGISTER, 0x0A);
    outb(VGA_DATA_REGISTER, cursor_start);

    outb(VGA_CTRL_REGISTER, 0x0B);
    outb(VGA_DATA_REGISTER, cursor_end);
}

unsigned char make_color(vga_color_t bg, vga_color_t fg)
{
    return (bg << 4) | (fg & 0x0F);
}

void set_color(unsigned char new_color)
{
    s_color = new_color;
}

static void scroll(void)
{
    int x;
    int y;

    for (y = 1; y < 25; y++) {
        for (x = 0; x < 80; x++) {
            int from = (y * 80 + x) * 2;
            int to = ((y - 1) * 80 + x) * 2;

            s_vga[to] = s_vga[from];
            s_vga[to + 1] = s_vga[from + 1];
        }
    }

    for (x = 0; x < 80; x++) {
        int index = (24 * 80 + x) * 2;
        s_vga[index] = ' ';
        s_vga[index + 1] = s_color;
    }
}

void move_pointer(u8_t x, u8_t y)
{
    while (x >= 80) {
        x -= 80;
        y++;
    }

    while (y >= 25) {
        scroll();
        y--;
    }

    pointer.x = x;
    pointer.y = y;
}

void clear_screen(void)
{
    int x;
    int y;

    for (y = 0; y < 25; y++) {
        for (x = 0; x < 80; x++) {
            int index = (y * 80 + x) * 2;
            s_vga[index] = ' ';
            s_vga[index + 1] = s_color;
        }
    }

    move_pointer(0, 0);
}

void put_char(char c)
{
    int index;

    if (c == '\n') {
        move_pointer(0, pointer.y + 1);
        return;
    }

    if (c == '\r') {
        move_pointer(0, pointer.y);
        return;
    }

    if (c == '\b') {
        if (pointer.x > 0) {
            pointer.x--;
        }
        put_char(' ');
        pointer.x--;
        return;
    }

    index = (pointer.y * 80 + pointer.x) * 2;
    s_vga[index] = c;
    s_vga[index + 1] = s_color;

    move_pointer(pointer.x + 1, pointer.y);
}

void print(const char* str)
{
    int i;

    for (i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

void println(const char* str)
{
    print(str);
    put_char('\n');
}

void print_int(const u32_t val) {
    char buf[12] = {0,};
    itoa(val, buf);
    println(buf);
}

void print_hex(const u32_t val) {
    char buf[12] = {0,};
    itohex(val, buf);
    println(buf);
}

position_t get_current_pointer(void) {
    position_t position = {pointer.x, pointer.y};
    return position;
}