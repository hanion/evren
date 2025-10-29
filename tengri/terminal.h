#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

#define TERMINAL_WIDTH  80
#define TERMINAL_HEIGHT 25

typedef enum {
	TERM_COLOR_BLACK = 0,
	TERM_COLOR_BLUE = 1,
	TERM_COLOR_GREEN = 2,
	TERM_COLOR_CYAN = 3,
	TERM_COLOR_RED = 4,
	TERM_COLOR_MAGENTA = 5,
	TERM_COLOR_BROWN = 6,
	TERM_COLOR_LIGHT_GREY = 7,
	TERM_COLOR_DARK_GREY = 8,
	TERM_COLOR_LIGHT_BLUE = 9,
	TERM_COLOR_LIGHT_GREEN = 10,
	TERM_COLOR_LIGHT_CYAN = 11,
	TERM_COLOR_LIGHT_RED = 12,
	TERM_COLOR_LIGHT_MAGENTA = 13,
	TERM_COLOR_LIGHT_BROWN = 14,
	TERM_COLOR_WHITE = 15,
} TERM_COLOR;

void terminal_initialize();

void terminal_set_color(TERM_COLOR fg, TERM_COLOR bg);

void terminal_set_cursor(usize x, usize y);
void terminal_update_cursor();

usize terminal_get_cursor_row();
usize terminal_get_cursor_column();

void terminal_put_entry_at(c8 entry, TERM_COLOR color, usize x, usize y);

void terminal_putchar(c8 c);
void terminal_write(const c8* data, usize size);
void terminal_write_cstr(const char* str);

void terminal_clear();
void terminal_scroll();


#endif // _TERMINAL_H

