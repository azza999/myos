#include "shell.h"
#include "screen.h"
#include "keyboard.h"

void shell_update(void);
static void shell_put_prompt(void);
static void shell_handle_key_event(const key_event_t* event);
static void shell_execute_line(void);

static const char* s_prompt = "myos> ";

void shell_update(void) {
	key_event_t ev;

    while (keyboard_poll_event(&ev)) {
        shell_handle_key_event(&ev);
    }
}

void shell_init(void) {
	shell_put_prompt();
	while(1) {
		shell_update();
		__asm__ volatile("hlt");
	}
}


static void shell_put_prompt(void) {
	print(s_prompt);
}


static void shell_handle_key_event(const key_event_t* ev) {

	if (ev->pressed == 0) {
		return;
	}

	const keycode_t key = ev->key;

	const char ascii_key = keyboard_event_to_ascii(ev);

	if (key == KEY_ENTER) {
		put_char('\n');
		// execute program
		return;
	}

	if (key == KEY_BACKSPACE) {
		put_char('\b');
		put_char(' ');
		put_char('\b');
		return;
	}

	// unprintable key
	if (ascii_key == 0) {
		return;
	}

	put_char(ascii_key);
}
static void shell_execute_line(void);
static char key_event_to_ascii(const key_event_t* event);