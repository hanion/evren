#ifndef _APP_TETRIS_C
#define _APP_TETRIS_C

#include "key.h"
#include "types.h"
#include "ee.h"

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20

#define TETRIS_OFFSET_Y 2
#define TETRIS_OFFSET_X 20

#define SHAPE_COUNT 7


typedef struct {
	i32 x, y;
} v2i;

v2i v2i_add(v2i a, v2i b) {
	return (v2i){ a.x + b.x, a.y + b.y };
}


typedef struct {
	v2i blocks[4];
	Color color;
} Shape;

typedef struct {
	Shape shape;
	v2i position;

} Tetromino;

void rotate_clockwise(Shape* shape) {
	for (i32 i = 0; i < 4; i++) {
		i32 x = shape->blocks[i].x;
		i32 y = shape->blocks[i].y;
		shape->blocks[i].x = -y;
		shape->blocks[i].y = x;
	}
}
void rotate_counter_clockwise(Shape* shape) {
	for (i32 i = 0; i < 4; i++) {
		i32 x = shape->blocks[i].x;
		i32 y = shape->blocks[i].y;
		shape->blocks[i].x = y;
		shape->blocks[i].y = -x;
	}
}

const Shape shape_l = {
	.blocks = { {0,-1}, {0,0}, {0,1}, {1,1} },
	.color = 0b100
};
const Shape shape_j = {
	.blocks = { {0,-1}, {0,0}, {0,1}, {-1,1} },
	.color = 0b001
};
const Shape shape_s = {
	.blocks = { {0,-1}, {0,0}, {1,0}, {1,1} },
	.color = 0b010
};
const Shape shape_z = {
	.blocks = { {0,-1}, {0,0}, {-1,0}, {-1,1} },
	.color = 0b100
};
const Shape shape_t = {
	.blocks = { {0,-1}, {0,0}, {-1,0}, {1,0} },
	.color = 0b101
};
const Shape shape_o = {
	.blocks = { {0,-1}, {0,0}, {-1,-1}, {-1,0} },
	.color = 0b110
};
const Shape shape_i = {
	.blocks = { {0,-1}, {0,0}, {0,1}, {0,2} },
	.color = 0b011
};

const Shape all_shapes[SHAPE_COUNT] = {
	shape_l, shape_j, shape_s, shape_z, shape_t, shape_o, shape_i
};




f64 m_delta_time = 0.01f;
f32 m_time_passed = 0.0f;
f32 m_move_timer = 1.0f;

u16 m_score = 0;

u8 m_grid[TETRIS_WIDTH][TETRIS_HEIGHT] = {{0}};

Tetromino m_current = {};

void reset_game();
void draw_frame();
void draw_current();
void copy_to_back_buffer();
bool is_valid_position(const Tetromino* t);
bool move_tetromino(Tetromino* t, v2i dir);
void place_current();
void spawn_new();
void clear_lines();


static u32 _rand_state = 1122334455;
static b8 _rand_state_initialized = 0;

static inline u32 read_tsc_low() {
	u32 low;
	__asm__ volatile ("rdtsc" : "=a"(low) :: "edx");
	return low;
}

i32 tetris_randi() {
	if (!_rand_state_initialized) {
		_rand_state = read_tsc_low();
		_rand_state_initialized = 1;
	}
	_rand_state = _rand_state * 1103515245 + 12345;
	return (_rand_state >> 16) & 0x7FFF;
}



void AppTetris_on_ready() {
	reset_game();
}

void AppTetris_on_update(double delta_time) {
	renderer_clear_back_buffer();

	m_time_passed += delta_time;
	if (m_time_passed > m_move_timer) {
		m_time_passed = 0;
		if (!move_tetromino(&m_current, (v2i){0,1})) {
			place_current();
			spawn_new();
			m_score++;
		}
	}

	draw_frame();
	copy_to_back_buffer();
	draw_current();

	renderer_draw_number(m_score, 1,1, false);
}

void draw_frame() {
	v2i top_left  = { TETRIS_OFFSET_X-1, TETRIS_OFFSET_Y-1 };
	v2i top_right = { TETRIS_OFFSET_X+TETRIS_WIDTH, TETRIS_OFFSET_Y-1 };

	v2i bottom_left  = { TETRIS_OFFSET_X-1, TETRIS_OFFSET_Y + TETRIS_HEIGHT };
	v2i bottom_right = { TETRIS_OFFSET_X+TETRIS_WIDTH, TETRIS_OFFSET_Y + TETRIS_HEIGHT };


	renderer_draw_line(top_left.x, top_left.y, top_right.x, top_right.y);
	renderer_draw_line(bottom_left.x, bottom_left.y, bottom_right.x, bottom_right.y);
	renderer_draw_line(top_right.x, top_right.y, bottom_right.x, bottom_right.y);
	renderer_draw_line(top_left.x, top_left.y, bottom_left.x, bottom_left.y);
}
void draw_current() {
	for (u8 block = 0; block < 4; ++block) {
		renderer_set_pixel(
			TETRIS_OFFSET_X + m_current.position.x + m_current.shape.blocks[block].x,
			TETRIS_OFFSET_Y + m_current.position.y + m_current.shape.blocks[block].y,
			m_current.shape.color
		);
	}
}

void reset_game() {
	for (u32 y = 0; y < TETRIS_HEIGHT; ++y) {
		for (u32 x = 0; x < TETRIS_WIDTH; ++x) {
			m_grid[x][y] = 0;
		}
	}
	m_move_timer = 1.0f;
	m_score = 0;
	spawn_new();
}

void copy_to_back_buffer() {
	for (u8 x = 0; x < TETRIS_WIDTH; ++x) {
		for (u8 y = 0; y < TETRIS_HEIGHT; ++y) {
			if (m_grid[x][y]) {
				Color c;
				c = m_grid[x][y];
				renderer_set_pixel(x+TETRIS_OFFSET_X, y+TETRIS_OFFSET_Y, c);
			}
		}
	}
}


bool is_valid_position(const Tetromino* t) {
	for (u8 block = 0; block < 4; ++block) {
		v2i pos = v2i_add(t->position, t->shape.blocks[block]);
		if (pos.y < 0 || pos.x < 0 || pos.y >= TETRIS_HEIGHT || pos.x >= TETRIS_WIDTH) {
			return false;
		}
		if (m_grid[pos.x][pos.y]) {
			return false;
		}
	}
	return true;
}

bool move_tetromino(Tetromino* t, v2i dir) {
	Tetromino nt = *t;
	nt.position = v2i_add(t->position, dir);
	if (is_valid_position(&nt)) {
		t->position = nt.position;
		return true;
	}
	return false;
}


// NOTE: it must be valid position before calling this
void place_current() {
	for (u8 block = 0; block < 4; ++block) {
		v2i pos = v2i_add(m_current.position, m_current.shape.blocks[block]);
		m_grid[pos.x][pos.y] = m_current.shape.color;
	}
	clear_lines();
}


void spawn_new() {
	m_current.position = (v2i){TETRIS_WIDTH/2,1};
	m_current.shape = all_shapes[tetris_randi()%SHAPE_COUNT];
	if (!is_valid_position(&m_current)) {
		reset_game();
	}
}

void clear_lines() {
	for (i8 row = TETRIS_HEIGHT; row > 0; ) {
		bool full = true;
		for (u8 col = 0; col < TETRIS_WIDTH; ++col) {
			if (m_grid[col][row]==0) {
				full = false;
				break;
			}
		}

		if (full) {
			for (i8 y = row; y > 1; --y) {
				for (u8 col = 0; col < TETRIS_WIDTH; ++col) {
					m_grid[col][y] = m_grid[col][y-1];
				}
			}
			for (u8 col = 0; col < TETRIS_WIDTH; ++col) {
				m_grid[col][0] = 0;
			}
			m_score += 10;
		} else {
			row--;
		}
	}
}




void AppTetris_on_event() {
	if (is_key_just_pressed(KEY_S)) {
		move_tetromino(&m_current, (v2i){0,1});
	}

	if (is_key_just_pressed(KEY_A)) {
		move_tetromino(&m_current, (v2i){-1,0});
	}


	if (is_key_just_pressed(KEY_D)) {
		move_tetromino(&m_current, (v2i){1,0});
	}
	if (is_key_just_pressed(KEY_E)) {
		rotate_clockwise(&m_current.shape);
		if(!is_valid_position(&m_current)) {
			rotate_counter_clockwise(&m_current.shape);
		}
	}

	if (is_key_just_pressed(KEY_Q)) {
		rotate_counter_clockwise(&m_current.shape);
		if(!is_valid_position(&m_current)) {
			rotate_clockwise(&m_current.shape);
		}
	}
	if (is_key_just_pressed(KEY_SPACE)) {
		m_move_timer = 0.001f;
	}

	if (is_key_just_released(KEY_SPACE)) {
		m_move_timer = 1.0f;
	}
}





#endif // _APP_TETRIS_C
