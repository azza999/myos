#include "shell.h"
#include "screen.h"
#include "keyboard.h"

#define CMD_BUFFER_SIZE 128

static const char* s_prompt = "myos> ";

typedef struct {
	u8_t length;
	u8_t cursor;
	char buffer[CMD_BUFFER_SIZE];
} cmd_buffer_t;

static cmd_buffer_t s_cmd_buffer;

void shell_update(void);
static void shell_put_prompt(void);
static void shell_handle_key_event(const key_event_t* event);
static void shell_execute_line(void);
static void cmd_buffer_clear(void);
static int cmd_buffer_insert(char c);
static int cmd_buffer_backspace(void);
static void cmd_buffer_delete(void);
// static int cmd_buffer_move_left(void);
// static int cmd_buffer_move_right(void);


void shell_update(void) {
	key_event_t ev;
	position_t pointer;

	pointer = get_current_pointer();

	resize_cursor(14,15);
	move_cursor(pointer);

    while (keyboard_poll_event(&ev)) {
        shell_handle_key_event(&ev);
    }
}

void shell_init(void) {
	

	cmd_buffer_clear();
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
	position_t pointer = {0};

	if (key == KEY_ENTER) {
		put_char('\n');
		println(s_cmd_buffer.buffer);
		print_int(s_cmd_buffer.length);
		print_int(s_cmd_buffer.cursor);
		// execute program
		cmd_buffer_clear();
		shell_put_prompt();
		return;
	}

	if (key == KEY_BACKSPACE) {
		put_char('\b');
		cmd_buffer_backspace();
		return;
	}
/*
	if (key == KEY_LEFT) {
		pointer = get_current_pointer();
		move_pointer(pointer.x-1, pointer.y);
		cmd_buffer_move_left();
		return;
	}

	if (key == KEY_RIGHT) {
		pointer = get_current_pointer();
		move_pointer(pointer.x+1, pointer.y);
		cmd_buffer_move_right();
		return;
	}
*/

	// unprintable key
	if (ascii_key == 0) {
		return;
	}

	cmd_buffer_insert(ascii_key);
	put_char(ascii_key);
}

static void cmd_buffer_clear(void) {
	s_cmd_buffer.length = 0;
	s_cmd_buffer.cursor = 0;
	memset(s_cmd_buffer.buffer, 0, CMD_BUFFER_SIZE);
}

// return length
static int cmd_buffer_insert(char c) {
	// cursor == length면 진입 x
	for (u8_t i = s_cmd_buffer.length+1; i > s_cmd_buffer.cursor+1; --i) {
		s_cmd_buffer.buffer[i] = s_cmd_buffer.buffer[i-1];
	}

	s_cmd_buffer.buffer[s_cmd_buffer.cursor] = c;

	s_cmd_buffer.length++;
	s_cmd_buffer.cursor++;

	return s_cmd_buffer.length;
}

static int cmd_buffer_backspace(void) {
	if(s_cmd_buffer.cursor < 1) return;
	s_cmd_buffer.cursor--;
	cmd_buffer_delete();
}

static void cmd_buffer_delete(void) {
	if (s_cmd_buffer.length < 1) return;
	if (s_cmd_buffer.cursor == s_cmd_buffer.length) return;

	s_cmd_buffer.length--;
	for (u8_t i = s_cmd_buffer.cursor; i < s_cmd_buffer.length; i++) {
		s_cmd_buffer.buffer[i] = s_cmd_buffer.buffer[i+1];
	}
}
/*
static int cmd_buffer_move_left(void) {
	if (s_cmd_buffer.cursor > 0) {
		s_cmd_buffer.cursor--;
	}
	return s_cmd_buffer.cursor;
}

static int cmd_buffer_move_right(void) {
	if (s_cmd_buffer.cursor < s_cmd_buffer.length) {
		s_cmd_buffer.cursor++;
	}
	return s_cmd_buffer.cursor;
}
*/