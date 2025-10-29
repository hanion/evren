#ifndef _KEY_H
#define _KEY_H

#include "types.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEY_STATES_COUNT 356

typedef enum {
	KEY_NONE = 0,
	KEY_ESCAPE = 0x01,
	KEY_1      = 0x02,
	KEY_2      = 0x03,
	KEY_3      = 0x04,
	KEY_4      = 0x05,
	KEY_5      = 0x06,
	KEY_6      = 0x07,
	KEY_7      = 0x08,
	KEY_8      = 0x09,
	KEY_9      = 0x0A,
	KEY_0      = 0x0B,
	KEY_MINUS  = 0x0C,
	KEY_EQUAL  = 0x0D,
	KEY_BACKSPACE = 0x0E,
	KEY_TAB   = 0x0F,

	KEY_Q     = 0x10,
	KEY_W     = 0x11,
	KEY_E     = 0x12,
	KEY_R     = 0x13,
	KEY_T     = 0x14,
	KEY_Y     = 0x15,
	KEY_U     = 0x16,
	KEY_I     = 0x17,
	KEY_O     = 0x18,
	KEY_P     = 0x19,
	KEY_LEFTBRACKET  = 0x1A,
	KEY_RIGHTBRACKET = 0x1B,
	KEY_ENTER = 0x1C,
	KEY_LEFTCTRL = 0x1D,

	KEY_A     = 0x1E,
	KEY_S     = 0x1F,
	KEY_D     = 0x20,
	KEY_F     = 0x21,
	KEY_G     = 0x22,
	KEY_H     = 0x23,
	KEY_J     = 0x24,
	KEY_K     = 0x25,
	KEY_L     = 0x26,
	KEY_SEMICOLON = 0x27,
	KEY_QUOTE = 0x28,
	KEY_GRAVE = 0x29,
	KEY_LEFTSHIFT = 0x2A,
	KEY_BACKSLASH = 0x2B,

	KEY_Z     = 0x2C,
	KEY_X     = 0x2D,
	KEY_C     = 0x2E,
	KEY_V     = 0x2F,
	KEY_B     = 0x30,
	KEY_N     = 0x31,
	KEY_M     = 0x32,
	KEY_COMMA = 0x33,
	KEY_DOT   = 0x34,
	KEY_SLASH = 0x35,
	KEY_RIGHTSHIFT = 0x36,
	KEY_KP_ASTERISK = 0x37,
	KEY_LEFTALT = 0x38,
	KEY_SPACE = 0x39,
} KEY_CODE;


typedef enum {
	KEY_STATE_NONE,
	KEY_STATE_PRESSED,
	KEY_STATE_HELD,
	KEY_STATE_RELEASED,
} KEY_STATE;


extern KEY_STATE key_states[KEY_STATES_COUNT];
KEY_STATE key_get_state(KEY_CODE key);
b8 is_key_just_pressed (KEY_CODE key);
b8 is_key_held         (KEY_CODE key);
b8 is_key_just_released(KEY_CODE key);
b8 is_key_being_pressed(KEY_CODE key);
void key_poll();
c8 key_code_to_char(KEY_CODE keycode);


#endif // _KEY_H
