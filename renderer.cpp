internal void 
clear_screen(u32 color) {
	unsigned int* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void 
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void
draw_arena_borders(float arena_x, float arena_y, u32 color) {
	arena_x *= render_state.height * render_scale;
	arena_y *= render_state.height * render_scale;

	int x0 = (int)((float)render_state.width * .5f - arena_x);
	int x1 = (int)((float)render_state.width * .5f + arena_x);
	int y0 = (int)((float)render_state.height * .5f - arena_y);
	int y1 = (int)((float)render_state.height * .5f + arena_y);

	draw_rect_in_pixels(0, 0, render_state.width, y0, color);
	draw_rect_in_pixels(0, y1, x1, render_state.height, color);
	draw_rect_in_pixels(0, y0, x0, y1, color);
	draw_rect_in_pixels(x1, y0, render_state.width, render_state.height, color);
}

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {

	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void
draw_text(const char* character[5], float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_x = x;

	for (int i = 0; i < 5; i++) {
		const char* row = character[i];
		while (*row) {
			if (*row == '0') {
				draw_rect(x, y, half_size, half_size, color);
			}
			x += size;
			row++;
		}
		y -= size;
		x = original_x;
	}
}

internal void 
draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	if (x > 0 && number > 9) {
		int digits_offset = floor(log10(number));
		x += size * 4.f * digits_offset;
	}

	do
	{
		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			draw_text(char_0, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			draw_text(char_1, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 2: {
			draw_text(char_2, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_text(char_3, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_text(char_4, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_text(char_5, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_text(char_6, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_text(char_7, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_text(char_8, x, y, size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_text(char_9, x, y, size, color);
			x -= size * 4.f;
		} break;
		}
	} while (number);
}

internal void
draw_option_bars(float x, float y, int setting_value) {
	draw_rect(x, y, 10, 4, setting_value > 2 ? color_button : color_button_unfocused);
	draw_rect(x, y - 10, 10, 4, setting_value > 1 ? color_button : color_button_unfocused);
	draw_rect(x, y - 20, 10, 4, setting_value > 0 ? color_button : color_button_unfocused);
}

internal void
draw_exit_menu(float text_size, int setting_value) {
	draw_rect(0, 0, 19, 11, color_borders);
	draw_text(text_exit, -8, 7, text_size, color_text);
	draw_rect(-10, -5, 7, 4, setting_value ? color_button : color_button_unfocused);
	draw_text(text_yes, -14, -3.5f, text_size * 0.75f, setting_value ? color_text : color_text_unfocused);
	draw_rect(10, -5, 7, 4, setting_value ? color_button : color_button_unfocused);
	draw_text(text_no, 8, -3.5f, text_size * 0.75f, setting_value ? color_text : color_text_unfocused);
}

internal void 
draw_pause_icon() {
	draw_rect(-4, 0, 2, 6, color_borders);
	draw_rect(4, 0, 2, 6, color_borders);
}