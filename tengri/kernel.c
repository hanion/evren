#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "key.c"
#include "terminal.c"

#include "ee.c"
#include "app_tetris.c"
#include "app_string.c"


static void sleep_cycles(u32 cycles) {
	for (volatile u32 i = 0; i < cycles; i++);
}


#define EE_APP_COUNT 2
EE_Application ee_apps[EE_APP_COUNT] = {
	(EE_Application){
		.name = "string",
		.on_ready =  AppString_on_ready,
		.on_event =  AppString_on_event,
		.on_update = AppString_on_update,
	},
	(EE_Application){
		.name = "tetris",
		.on_ready =  AppTetris_on_ready,
		.on_event =  AppTetris_on_event,
		.on_update = AppTetris_on_update,
	}
};
i8 ee_app_current = 1;


void kernel_main(void) {
	terminal_initialize();
	terminal_write_cstr("evren\nsays\nhello\n");
	terminal_update_cursor();

	while (1) {
		EE_Application* app = &ee_apps[ee_app_current];
		if(!app->is_ready) {
			app->on_ready();
			app->is_ready = true;
		}

		key_poll();
		if (is_key_just_pressed(KEY_TAB)) {
			ee_app_current = (ee_app_current+1) % EE_APP_COUNT;
			continue;
		}

		app->on_event();
		app->on_update(0.05);

		renderer_swap_buffers();
		platform_render_front_buffer();
		sleep_cycles(10000000);
	}
}



