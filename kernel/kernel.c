#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "screen.h"
#include "stdlib.h"
#include "keyboard.h"

typedef struct {
    char render_str[5];
    unsigned char pressed;
    unsigned char x;
    unsigned char y;
} key_entry_t;

static key_entry_t s_keys[KEY_MAX];

static void print_at(unsigned char x, unsigned char y, const char* str)
{
    move_cursor(x, y);
    print(str);
}

static void print_fixed3(const unsigned char* str)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (str[i] == '\0') {
            put_char(' ');
        } else {
            put_char(str[i]);
        }
    }
}

static void draw_key(unsigned char x, unsigned char y, keycode_t key)
{
    if (key <= KEY_NONE || key >= KEY_MAX) {
        return;
    }

    move_cursor(x, y);

    if (s_keys[key].pressed) {
        set_color(make_color(COLOR_WHITE, COLOR_BLACK));
    } else {
        set_color(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
    }

    put_char('[');
    print_fixed3((const unsigned char*)s_keys[key].render_str);
    put_char(']');

    set_color(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
}

static void init_keymap(void)
{
    int i;

    for (i = 0; i < KEY_MAX; i++) {
        s_keys[i].render_str[0] = '\0';
        s_keys[i].pressed = 0;
        s_keys[i].x = 0;
        s_keys[i].y = 0;
    }

    s_keys[KEY_ESC] = (key_entry_t){"ESC", 0, 2, 4};
    s_keys[KEY_1] = (key_entry_t){"1", 0, 2, 6};
    s_keys[KEY_2] = (key_entry_t){"2", 0, 8, 6};
    s_keys[KEY_3] = (key_entry_t){"3", 0, 14, 6};
    s_keys[KEY_4] = (key_entry_t){"4", 0, 20, 6};
    s_keys[KEY_5] = (key_entry_t){"5", 0, 26, 6};
    s_keys[KEY_6] = (key_entry_t){"6", 0, 32, 6};
    s_keys[KEY_7] = (key_entry_t){"7", 0, 38, 6};
    s_keys[KEY_8] = (key_entry_t){"8", 0, 44, 6};
    s_keys[KEY_9] = (key_entry_t){"9", 0, 50, 6};
    s_keys[KEY_0] = (key_entry_t){"0", 0, 56, 6};
    s_keys[KEY_BACKSPACE] = (key_entry_t){"BSP", 0, 68, 6};
    s_keys[KEY_TAB] = (key_entry_t){"TAB", 0, 2, 8};
    s_keys[KEY_Q] = (key_entry_t){"Q", 0, 8, 8};
    s_keys[KEY_W] = (key_entry_t){"W", 0, 14, 8};
    s_keys[KEY_E] = (key_entry_t){"E", 0, 20, 8};
    s_keys[KEY_R] = (key_entry_t){"R", 0, 26, 8};
    s_keys[KEY_T] = (key_entry_t){"T", 0, 32, 8};
    s_keys[KEY_Y] = (key_entry_t){"Y", 0, 38, 8};
    s_keys[KEY_U] = (key_entry_t){"U", 0, 44, 8};
    s_keys[KEY_I] = (key_entry_t){"I", 0, 50, 8};
    s_keys[KEY_O] = (key_entry_t){"O", 0, 56, 8};
    s_keys[KEY_P] = (key_entry_t){"P", 0, 62, 8};
    s_keys[KEY_CAPSLOCK] = (key_entry_t){"CAP", 0, 2, 10};
    s_keys[KEY_A] = (key_entry_t){"A", 0, 8, 10};
    s_keys[KEY_S] = (key_entry_t){"S", 0, 14, 10};
    s_keys[KEY_D] = (key_entry_t){"D", 0, 20, 10};
    s_keys[KEY_F] = (key_entry_t){"F", 0, 26, 10};
    s_keys[KEY_G] = (key_entry_t){"G", 0, 32, 10};
    s_keys[KEY_H] = (key_entry_t){"H", 0, 38, 10};
    s_keys[KEY_J] = (key_entry_t){"J", 0, 44, 10};
    s_keys[KEY_K] = (key_entry_t){"K", 0, 50, 10};
    s_keys[KEY_L] = (key_entry_t){"L", 0, 56, 10};
    s_keys[KEY_ENTER] = (key_entry_t){"ENT", 0, 68, 10};
    s_keys[KEY_LEFTSHIFT] = (key_entry_t){"LSH", 0, 2, 12};
    s_keys[KEY_Z] = (key_entry_t){"Z", 0, 8, 12};
    s_keys[KEY_X] = (key_entry_t){"X", 0, 14, 12};
    s_keys[KEY_C] = (key_entry_t){"C", 0, 20, 12};
    s_keys[KEY_V] = (key_entry_t){"V", 0, 26, 12};
    s_keys[KEY_B] = (key_entry_t){"B", 0, 32, 12};
    s_keys[KEY_N] = (key_entry_t){"N", 0, 38, 12};
    s_keys[KEY_M] = (key_entry_t){"M", 0, 44, 12};
    s_keys[KEY_RIGHTSHIFT] = (key_entry_t){"RSH", 0, 68, 12};
    s_keys[KEY_LEFTCTRL] = (key_entry_t){"LCT", 0, 2, 14};
    s_keys[KEY_LEFTALT] = (key_entry_t){"LAT", 0, 8, 14};
    s_keys[KEY_SPACE] = (key_entry_t){"SPC", 0, 26, 14};
    s_keys[KEY_RIGHTALT] = (key_entry_t){"RAT", 0, 50, 14};
    s_keys[KEY_RIGHTCTRL] = (key_entry_t){"RCT", 0, 68, 14};
    s_keys[KEY_UP] = (key_entry_t){"UP", 0, 62, 18};
    s_keys[KEY_LEFT] = (key_entry_t){"LT", 0, 56, 20};
    s_keys[KEY_DOWN] = (key_entry_t){"DN", 0, 62, 20};
    s_keys[KEY_RIGHT] = (key_entry_t){"RT", 0, 68, 20};
}

static void process(key_event_t* ev)
{
    if (ev->key <= KEY_NONE || ev->key >= KEY_MAX) {
        return;
    }

    s_keys[ev->key].pressed = ev->pressed;
}

static void render(void)
{
    int i;

    clear_screen();
    print_at(24, 1, "[ KEY VIEWER ]");

    for (i = 0; i < KEY_MAX; i++) {
        if (s_keys[i].render_str[0] == '\0') {
            continue;
        }

        draw_key(s_keys[i].x, s_keys[i].y, i);
    }
}

void kernel_main(void)
{
    key_event_t ev;

    clear_screen();
    gdt_init();
    idt_init();
    pic_remap();
    keyboard_init();

    __asm__ volatile ("sti");

    init_keymap();

    println("keyboard ready");
    render();

    while (1) {
        while (keyboard_poll_event(&ev)) {
            process(&ev);
            render();
        }

        __asm__ volatile ("hlt");
    }
}
