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