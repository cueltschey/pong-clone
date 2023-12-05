internal void
render_pixels() {
	unsigned int* pixel = (unsigned int*)renderstate.buffer_memory;
	for (int i = 0; i < renderstate.height; ++i) {
		for (int j = 0; j < renderstate.width; ++j) {
			*pixel++ = i * 0xfff + j * 0xffEE00;
		}
	}
}
internal void
clear_screen(unsigned int color) {
	unsigned int* pixel = (unsigned int*)renderstate.buffer_memory;
	for (int i = 0; i < renderstate.height; ++i) {
		for (int j = 0; j < renderstate.width; ++j) {
			*pixel++ = color;
		}
	}
}

internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, unsigned int color) {

	x0 = clamp(0, x0, renderstate.width);
	x1 = clamp(0, x1, renderstate.width);
	y0 = clamp(0, y0, renderstate.height);
	y1 = clamp(0, y1, renderstate.height);


	for (int y = y0; y < y1; ++y) {
		unsigned int* pixel = (unsigned int*)renderstate.buffer_memory + x0 + y * renderstate.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}


global_var float render_scale = 0.01f;

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color) {

	x *= renderstate.height * render_scale;
	y *= renderstate.height * render_scale;
	half_size_x *= renderstate.height * render_scale;
	half_size_y *= renderstate.height * render_scale;

	x += renderstate.width / 2.f;
	y += renderstate.height / 2.f;

	// change to relative pixels
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}