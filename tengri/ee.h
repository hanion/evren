#ifndef _EE_H
#define _EE_H

#include "terminal.h"
#include "types.h"

typedef struct {
	const char* name;
	b8 is_ready;
	void (*on_ready)(void);
	void (*on_event)(void);
	void (*on_update)(f64);
} EE_Application;



typedef struct {
	i32 x, y;
	i32 width, height;
} Rect;

typedef u8 Color;


#ifdef EE_PANEL_SIZE
#define WIDTH 64
#define HEIGHT 32
#else
#define WIDTH 40
#define HEIGHT 25
#endif

extern u8 (*front_buffer)[WIDTH][HEIGHT];
extern u8 (*back_buffer)[WIDTH][HEIGHT];

void platform_render_front_buffer(void);
TERM_COLOR Color_to_term_color(Color color);

void renderer_swap_buffers(void);

void renderer_clear_back_buffer(void);
void renderer_set_pixel(i32 x, i32 y, Color color);

void renderer_draw_line(i32 x0, i32 y0, i32 x1, i32 y1);
void renderer_draw_linec(i32 x0, i32 y0, i32 x1, i32 y1, Color color);

void renderer_draw_rect(u8 x, u8 y, u8 width, u8 height, Color color);
void renderer_draw_Rect(Rect rect, Color color);

void renderer_draw_number(i32 num, i32 x, i32 y, bool bold);
void renderer_draw_text(const char *text, i32 x, i32 y, bool bold);
i32  renderer_calculate_text_width(const char* text, bool bold);
i32  renderer_calculate_number_width(i32 number, bool bold);


#endif // _EE_H
