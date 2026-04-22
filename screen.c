#include "screen.h"

static volatile char* vga = (volatile char*)0xB8000;
static unsigned char row = 0;
static unsigned char col = 0;
static unsigned char color = 0x0F;

unsigned char make_color(vga_color bg, vga_color fg) {
    return (bg << 4) | (fg & 0x0F);
}

void set_color(unsigned char new_color) {
    color = new_color;
}

static void scroll(void) {
    for (int y = 1; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            int from = (y * 80 + x) * 2;
            int to = ((y - 1) * 80 + x) * 2;

            vga[to] = vga[from];
            vga[to + 1] = vga[from + 1];
        }
    }

    for (int x = 0; x < 80; x++) {
        int index = (24 * 80 + x) * 2;
        vga[index] = ' ';
        vga[index + 1] = color;
    }
}

void move_cursor(unsigned char x, unsigned char y) {
    while (x >= 80) {
        x -= 80;
        y++;
    }

    while (y >= 25) {
        scroll();
        y--;
    }

    col = x;
    row = y;
}

void clear_screen(void) {
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            int index = (y * 80 + x) * 2;
            vga[index] = ' ';
            vga[index + 1] = color;
        }
    }

    move_cursor(0, 0);
}

void put_char(char c) {
    if (c == '\n') {
        move_cursor(0, row + 1);
        return;
    }

    if (c == '\r') {
        move_cursor(0, row);
        return;
    }

    int index = (row * 80 + col) * 2;
    vga[index] = c;
    vga[index + 1] = color;

    move_cursor(col + 1, row);
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

void println(const char* str) {
    print(str);
    put_char('\n');
}