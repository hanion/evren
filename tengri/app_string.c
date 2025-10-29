#ifndef _APP_STRING_C
#define _APP_STRING_C

#include "ee.h"

typedef struct {
	const char* string;
	Rect rect;
	bool bold;
	i32 dir;
} Message;

#define MSG_COUNT 2
Message msgs[MSG_COUNT] = {
	//{ "The quick brown fox jumps over the lazy dog.", (Rect){0,0,0,8}, false, -1 },
	{ "evren", (Rect){0, 0,0,8}, false, -1 },
	{ "0.2.0", (Rect){0,16,0,8}, false, -1 },
};
f64 timer = 0;


void scroll_msg(Message* msg) {
	msg->rect.x += msg->dir;//DeltaTime * 60;
	if (msg->dir < 0) {
		if (msg->rect.x < -msg->rect.width) {
			msg->rect.x = /*WIDTH*/ 40;
		}
	} else {
		if (msg->rect.x > WIDTH) {
			msg->rect.x = -msg->rect.width;
		}
	}
}

void AppString_on_ready() {
	for (uint8_t i = 0; i < MSG_COUNT; ++i) {
		msgs[i].rect.width = renderer_calculate_text_width(msgs[i].string, msgs[i].bold);
	}
	msgs[1].rect.x = renderer_calculate_text_width("evren ", false);
}

void AppString_on_event() {}


void AppString_on_update(f64 delta_time) {
	timer += delta_time;

	if (timer > 0.1) {
		timer = 0;
		for (uint8_t i = 0; i < MSG_COUNT; ++i) {
			Message* msg = &msgs[i];
			scroll_msg(msg);
		}
	}

	renderer_clear_back_buffer();
	for (uint8_t i = 0; i < MSG_COUNT; ++i) {
		Message* msg = &msgs[i];
		renderer_draw_text(msg->string, msg->rect.x, msg->rect.y, msg->bold);
	}
}



#endif // _APP_STRING_C
