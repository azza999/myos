#include "keyboard.h"
#include "screen.h"
#include "io.h"
#include "stdlib.h"

typedef enum {
    SC_NONE = 0,
    SC_ESC = 0x01,
    SC_1 = 0x02, SC_2 = 0x03, SC_3 = 0x04, SC_4 = 0x05,
    SC_5 = 0x06, SC_6 = 0x07, SC_7 = 0x08, SC_8 = 0x09,
    SC_9 = 0x0A, SC_0 = 0x0B,
    SC_MINUS = 0x0C,
    SC_EQUAL = 0x0D,
    SC_BACKSPACE = 0x0E,
    SC_TAB = 0x0F,
    SC_Q = 0x10, SC_W = 0x11, SC_E = 0x12, SC_R = 0x13,
    SC_T = 0x14, SC_Y = 0x15, SC_U = 0x16, SC_I = 0x17,
    SC_O = 0x18, SC_P = 0x19,
    SC_LBRACKET = 0x1A,
    SC_RBRACKET = 0x1B,
    SC_ENTER = 0x1C,
    SC_LCTRL = 0x1D,
    SC_A = 0x1E, SC_S = 0x1F, SC_D = 0x20, SC_F = 0x21,
    SC_G = 0x22, SC_H = 0x23, SC_J = 0x24, SC_K = 0x25,
    SC_L = 0x26,
    SC_SEMICOLON = 0x27,
    SC_APOSTROPHE = 0x28,
    SC_GRAVE = 0x29,
    SC_LSHIFT = 0x2A,
    SC_BACKSLASH = 0x2B,
    SC_Z = 0x2C, SC_X = 0x2D, SC_C = 0x2E, SC_V = 0x2F,
    SC_B = 0x30, SC_N = 0x31, SC_M = 0x32,
    SC_COMMA = 0x33,
    SC_DOT = 0x34,
    SC_SLASH = 0x35,
    SC_RSHIFT = 0x36,
    SC_KP_STAR = 0x37,
    SC_LALT = 0x38,
    SC_SPACE = 0x39,
    SC_CAPSLOCK = 0x3A,
    SC_F1 = 0x3B,
    SC_F2 = 0x3C,
    SC_F3 = 0x3D,
    SC_F4 = 0x3E,
    SC_F5 = 0x3F,
    SC_F6 = 0x40,
    SC_F7 = 0x41,
    SC_F8 = 0x42,
    SC_F9 = 0x43,
    SC_F10 = 0x44,
    SC_NUMLOCK = 0x45,
    SC_SCROLLLOCK = 0x46,
    SC_KP_7 = 0x47,
    SC_KP_8 = 0x48,
    SC_KP_9 = 0x49,
    SC_KP_MINUS = 0x4A,
    SC_KP_4 = 0x4B,
    SC_KP_5 = 0x4C,
    SC_KP_6 = 0x4D,
    SC_KP_PLUS = 0x4E,
    SC_KP_1 = 0x4F,
    SC_KP_2 = 0x50,
    SC_KP_3 = 0x51,
    SC_KP_0 = 0x52,
    SC_KP_DOT = 0x53,
    SC_F11 = 0x57,
    SC_F12 = 0x58,
    SC_E0 = 0xE0,
    SC_E1 = 0xE1
} scancode_t;

#define INPUT_QUEUE_SIZE 64

typedef struct {
    key_event_t buf[INPUT_QUEUE_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} input_queue_t;

static keycode_t s_scancode_map[128];
static keycode_t s_scancode_map_e0[128];
static input_queue_t s_input_queue;

static void init_scancode_map(void)
{
    int i;

    for (i = 0; i < 128; i++) {
        s_scancode_map[i] = KEY_NONE;
        s_scancode_map_e0[i] = KEY_NONE;
    }

    s_scancode_map[SC_ESC] = KEY_ESC;
    s_scancode_map[SC_1] = KEY_1;
    s_scancode_map[SC_2] = KEY_2;
    s_scancode_map[SC_3] = KEY_3;
    s_scancode_map[SC_4] = KEY_4;
    s_scancode_map[SC_5] = KEY_5;
    s_scancode_map[SC_6] = KEY_6;
    s_scancode_map[SC_7] = KEY_7;
    s_scancode_map[SC_8] = KEY_8;
    s_scancode_map[SC_9] = KEY_9;
    s_scancode_map[SC_0] = KEY_0;
    s_scancode_map[SC_MINUS] = KEY_MINUS;
    s_scancode_map[SC_EQUAL] = KEY_EQUAL;
    s_scancode_map[SC_BACKSPACE] = KEY_BACKSPACE;
    s_scancode_map[SC_TAB] = KEY_TAB;
    s_scancode_map[SC_Q] = KEY_Q;
    s_scancode_map[SC_W] = KEY_W;
    s_scancode_map[SC_E] = KEY_E;
    s_scancode_map[SC_R] = KEY_R;
    s_scancode_map[SC_T] = KEY_T;
    s_scancode_map[SC_Y] = KEY_Y;
    s_scancode_map[SC_U] = KEY_U;
    s_scancode_map[SC_I] = KEY_I;
    s_scancode_map[SC_O] = KEY_O;
    s_scancode_map[SC_P] = KEY_P;
    s_scancode_map[SC_LBRACKET] = KEY_LEFTBRACE;
    s_scancode_map[SC_RBRACKET] = KEY_RIGHTBRACE;
    s_scancode_map[SC_ENTER] = KEY_ENTER;
    s_scancode_map[SC_LCTRL] = KEY_LEFTCTRL;
    s_scancode_map[SC_A] = KEY_A;
    s_scancode_map[SC_S] = KEY_S;
    s_scancode_map[SC_D] = KEY_D;
    s_scancode_map[SC_F] = KEY_F;
    s_scancode_map[SC_G] = KEY_G;
    s_scancode_map[SC_H] = KEY_H;
    s_scancode_map[SC_J] = KEY_J;
    s_scancode_map[SC_K] = KEY_K;
    s_scancode_map[SC_L] = KEY_L;
    s_scancode_map[SC_SEMICOLON] = KEY_SEMICOLON;
    s_scancode_map[SC_APOSTROPHE] = KEY_APOSTROPHE;
    s_scancode_map[SC_GRAVE] = KEY_GRAVE;
    s_scancode_map[SC_LSHIFT] = KEY_LEFTSHIFT;
    s_scancode_map[SC_BACKSLASH] = KEY_BACKSLASH;
    s_scancode_map[SC_Z] = KEY_Z;
    s_scancode_map[SC_X] = KEY_X;
    s_scancode_map[SC_C] = KEY_C;
    s_scancode_map[SC_V] = KEY_V;
    s_scancode_map[SC_B] = KEY_B;
    s_scancode_map[SC_N] = KEY_N;
    s_scancode_map[SC_M] = KEY_M;
    s_scancode_map[SC_COMMA] = KEY_COMMA;
    s_scancode_map[SC_DOT] = KEY_DOT;
    s_scancode_map[SC_SLASH] = KEY_SLASH;
    s_scancode_map[SC_RSHIFT] = KEY_RIGHTSHIFT;
    s_scancode_map[SC_KP_STAR] = KEY_KPASTERISK;
    s_scancode_map[SC_LALT] = KEY_LEFTALT;
    s_scancode_map[SC_SPACE] = KEY_SPACE;
    s_scancode_map[SC_CAPSLOCK] = KEY_CAPSLOCK;
    s_scancode_map[SC_F1] = KEY_F1;
    s_scancode_map[SC_F2] = KEY_F2;
    s_scancode_map[SC_F3] = KEY_F3;
    s_scancode_map[SC_F4] = KEY_F4;
    s_scancode_map[SC_F5] = KEY_F5;
    s_scancode_map[SC_F6] = KEY_F6;
    s_scancode_map[SC_F7] = KEY_F7;
    s_scancode_map[SC_F8] = KEY_F8;
    s_scancode_map[SC_F9] = KEY_F9;
    s_scancode_map[SC_F10] = KEY_F10;
    s_scancode_map[SC_NUMLOCK] = KEY_NUMLOCK;
    s_scancode_map[SC_SCROLLLOCK] = KEY_SCROLLLOCK;
    s_scancode_map[SC_KP_7] = KEY_KP7;
    s_scancode_map[SC_KP_8] = KEY_KP8;
    s_scancode_map[SC_KP_9] = KEY_KP9;
    s_scancode_map[SC_KP_MINUS] = KEY_KPMINUS;
    s_scancode_map[SC_KP_4] = KEY_KP4;
    s_scancode_map[SC_KP_5] = KEY_KP5;
    s_scancode_map[SC_KP_6] = KEY_KP6;
    s_scancode_map[SC_KP_PLUS] = KEY_KPPLUS;
    s_scancode_map[SC_KP_1] = KEY_KP1;
    s_scancode_map[SC_KP_2] = KEY_KP2;
    s_scancode_map[SC_KP_3] = KEY_KP3;
    s_scancode_map[SC_KP_0] = KEY_KP0;
    s_scancode_map[SC_KP_DOT] = KEY_KPDOT;
    s_scancode_map[SC_F11] = KEY_F11;
    s_scancode_map[SC_F12] = KEY_F12;

    s_scancode_map_e0[SC_ENTER] = KEY_KPENTER;
    s_scancode_map_e0[SC_LCTRL] = KEY_RIGHTCTRL;
    s_scancode_map_e0[SC_SLASH] = KEY_KPSLASH;
    s_scancode_map_e0[SC_LALT] = KEY_RIGHTALT;
    s_scancode_map_e0[SC_KP_7] = KEY_HOME;
    s_scancode_map_e0[SC_KP_8] = KEY_UP;
    s_scancode_map_e0[SC_KP_9] = KEY_PAGEUP;
    s_scancode_map_e0[SC_KP_4] = KEY_LEFT;
    s_scancode_map_e0[SC_KP_6] = KEY_RIGHT;
    s_scancode_map_e0[SC_KP_1] = KEY_END;
    s_scancode_map_e0[SC_KP_2] = KEY_DOWN;
    s_scancode_map_e0[SC_KP_3] = KEY_PAGEDOWN;
    s_scancode_map_e0[SC_KP_0] = KEY_INSERT;
    s_scancode_map_e0[SC_KP_DOT] = KEY_DELETE;
}

static unsigned char keyboard_parse_scancode(unsigned char scancode, key_event_t* ev)
{
    static unsigned char extended = 0;
    unsigned char released;
    unsigned char code;
    keycode_t key;

    if (scancode == SC_E0) {
        extended = 1;
        return 0;
    }

    released = scancode & 0x80;
    code = scancode & 0x7F;

    if (extended) {
        key = s_scancode_map_e0[code];
        extended = 0;
    } else {
        key = s_scancode_map[code];
    }

    if (key == KEY_NONE) {
        return 0;
    }

    ev->key = key;
    ev->pressed = released ? 0 : 1;
    return 1;
}

static void input_push_event(key_event_t ev)
{
    if (s_input_queue.count >= INPUT_QUEUE_SIZE) {
        return;
    }

    s_input_queue.buf[s_input_queue.tail] = ev;
    s_input_queue.tail = (s_input_queue.tail + 1) % INPUT_QUEUE_SIZE;
    s_input_queue.count++;
}

uint8_t keyboard_poll_event(key_event_t* ev)
{
    if (s_input_queue.count == 0) {
        return 0;
    }

    *ev = s_input_queue.buf[s_input_queue.head];
    s_input_queue.head = (s_input_queue.head + 1) % INPUT_QUEUE_SIZE;
    s_input_queue.count--;
    return 1;
}

void keyboard_init(void)
{
    s_input_queue.head = 0;
    s_input_queue.tail = 0;
    s_input_queue.count = 0;
    init_scancode_map();
}

void keyboard_handle_interrupt(void)
{
    unsigned char scancode = inb(0x60);
    key_event_t ev;

    if (keyboard_parse_scancode(scancode, &ev)) {
        input_push_event(ev);
    }
}
