#include "key.h"

KEY_STATE key_states[KEY_STATES_COUNT] = {0};


KEY_STATE key_get_state(KEY_CODE key) {
	return key_states[key];
}
b8 is_key_just_pressed(KEY_CODE key) {
	return (key_states[key] == KEY_STATE_PRESSED);
}

b8 is_key_held(KEY_CODE key) {
	return (key_states[key] == KEY_STATE_HELD);
}

b8 is_key_just_released(KEY_CODE key) {
	return (key_states[key] == KEY_STATE_RELEASED);
}

b8 is_key_being_pressed(KEY_CODE key) {
	return (is_key_held(key) || is_key_just_pressed(key));
}



static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ( "inb { %1, %0 | %0, %1 }" : "=a"(ret) : "Nd"(port) );
	return ret;
}
KEY_CODE keyboard_poll_scan_code() {
	if (inb(KEYBOARD_STATUS_PORT) & 1) {
		return inb(KEYBOARD_DATA_PORT);
	} else {
		return 0;
	}
}
void key_poll() {
	for (int i = 0; i < KEY_STATES_COUNT; i++) {
		if (key_states[i] == KEY_STATE_PRESSED) {
			key_states[i] = KEY_STATE_HELD;
		} else if (key_states[i] == KEY_STATE_RELEASED) {
			key_states[i] = KEY_STATE_NONE;
		}
	}

	while (1) {
		KEY_CODE sc = keyboard_poll_scan_code();
		if (sc == 0) break;

		if (sc & 0x80) {
			KEY_CODE key = sc & 0x7F;
			key_states[key] = KEY_STATE_RELEASED;
		} else {
			if (key_states[sc] != KEY_STATE_HELD) {
				key_states[sc] = KEY_STATE_PRESSED;
			}
		}
	}
}





c8 key_code_to_char(KEY_CODE keycode) {
	switch (keycode) {
		case KEY_A: return 'a';
		case KEY_B: return 'b';
		case KEY_C: return 'c';
		case KEY_D: return 'd';
		case KEY_E: return 'e';
		case KEY_F: return 'f';
		case KEY_G: return 'g';
		case KEY_H: return 'h';
		case KEY_I: return 'i';
		case KEY_J: return 'j';
		case KEY_K: return 'k';
		case KEY_L: return 'l';
		case KEY_M: return 'm';
		case KEY_N: return 'n';
		case KEY_O: return 'o';
		case KEY_P: return 'p';
		case KEY_Q: return 'q';
		case KEY_R: return 'r';
		case KEY_S: return 's';
		case KEY_T: return 't';
		case KEY_U: return 'u';
		case KEY_V: return 'v';
		case KEY_W: return 'w';
		case KEY_X: return 'x';
		case KEY_Y: return 'y';
		case KEY_Z: return 'z';

		case KEY_1: return '1';
		case KEY_2: return '2';
		case KEY_3: return '3';
		case KEY_4: return '4';
		case KEY_5: return '5';
		case KEY_6: return '6';
		case KEY_7: return '7';
		case KEY_8: return '8';
		case KEY_9: return '9';
		case KEY_0: return '0';

		case KEY_SPACE: return ' ';
		case KEY_ENTER: return '\n';
		default: break;
	}
	return '\0';
}
