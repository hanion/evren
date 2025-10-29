#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "key.h"
#include "terminal.h"

#include "key.c"
#include "terminal.c"


static void sleep_cycles(u32 cycles) {
	for (volatile u32 i = 0; i < cycles; i++);
}

void kernel_main(void) {
	terminal_initialize();
	terminal_write_cstr("evren\nsays\nhello\n");
	terminal_update_cursor();

	while (1) {
		key_poll();

		if (is_key_just_pressed(KEY_ENTER)) {
			terminal_putchar('\n');
			terminal_update_cursor();
		}

		if (is_key_just_pressed(KEY_A)) {
			terminal_putchar('a');
		} else if (is_key_held(KEY_A)) {
			terminal_putchar(' ');
		} else if (is_key_just_released(KEY_A)) {
			terminal_putchar('_');
		}

		if (is_key_being_pressed(KEY_S)) {
			terminal_putchar('s');
		}

		sleep_cycles(1000000);
	}
}



