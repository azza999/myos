#include "screen.h"

static volatile char* s_vga = (volatile char*)0xB8000;
static unsigned char s_row = 0;
static unsigned char s_col = 0;
static unsigned char s_color = 0x0F;

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

void move_cursor(unsigned char x, unsigned char y)
{
    while (x >= 80) {
        x -= 80;
        y++;
    }

    while (y >= 25) {
        scroll();
        y--;
    }

    s_col = x;
    s_row = y;
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

    move_cursor(0, 0);
}

void put_char(char c)
{
    int index;

    if (c == '\n') {
        move_cursor(0, s_row + 1);
        return;
    }

    if (c == '\r') {
        move_cursor(0, s_row);
        return;
    }

    index = (s_row * 80 + s_col) * 2;
    s_vga[index] = c;
    s_vga[index + 1] = s_color;

    move_cursor(s_col + 1, s_row);
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
