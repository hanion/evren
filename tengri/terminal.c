#include "terminal.h"


#define VGA_MEMORY  0xB8000

usize terminal_row = 0;
usize terminal_col = 0;
TERM_COLOR terminal_color = (TERM_COLOR_LIGHT_GREY | TERM_COLOR_BLACK << 4);
volatile u16* terminal_buffer = (u16*)VGA_MEMORY;

usize terminal_get_cursor_row() { return terminal_row; }
usize terminal_get_cursor_column() { return terminal_col; }


static inline u16 vga_entry(c8 uc, u8 color) {
	return (u16) uc | (u16) color << 8;
}


void terminal_initialize() {
	terminal_update_cursor();
	terminal_set_color(TERM_COLOR_LIGHT_GREY, TERM_COLOR_BLACK);
	terminal_clear();
}


void terminal_set_color(TERM_COLOR fg, TERM_COLOR bg) {
	terminal_color = fg | bg << 4;
}

static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ("outb { %0, %1 | %1, %0 }" : : "a"(val), "Nd"(port));
}
void terminal_set_cursor(usize x, usize y) {
	uint16_t pos = y * TERMINAL_WIDTH + x;
	outb(0x3D4, 0x0E);
	outb(0x3D5, (pos >> 8) & 0xFF);
	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);
}
void terminal_update_cursor() {
	terminal_set_cursor(terminal_col, terminal_row);
}




void terminal_put_entry_at(c8 entry, TERM_COLOR color, usize x, usize y) {
	terminal_buffer[y * TERMINAL_WIDTH + x] = vga_entry(entry, color);
}

void terminal_newline() {
	if (++terminal_row == TERMINAL_HEIGHT) {
		terminal_row = 0;
	}
	terminal_col = 0;
}

void terminal_putchar(c8 c) {
	if (c == '\n') {
		terminal_newline();
		return;
	}
	terminal_put_entry_at(c, terminal_color, terminal_col, terminal_row);
	if (++terminal_col == TERMINAL_WIDTH) {
		terminal_newline();
	}
	terminal_set_cursor(terminal_col, terminal_row);
}


void terminal_write(const c8* data, usize size) {
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
}

size_t strlen(const char* str) {
	usize len = 0;
	while (str[len]) len++;
	return len;
}
void terminal_write_cstr(const char* str) {
	terminal_write((const c8*)str, strlen(str));
}


void terminal_clear() {
	for (usize y = 0; y < TERMINAL_HEIGHT; y++) {
		for (usize x = 0; x < TERMINAL_WIDTH; x++) {
			terminal_buffer[y * TERMINAL_WIDTH + x] = vga_entry(' ', terminal_color);
		}
	}
}
void terminal_scroll() {
	// TODO: implement
}


