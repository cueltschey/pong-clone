internal void
render_pixels(void* buffer_memory, int h, int w) {
	unsigned int* pixel = (unsigned int*)buffer_memory;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			*pixel++ = i * 0xfff + j * 0xffEE00;
		}
	}
}
internal void
clear_screen(void* buffer_memory, int h, int w, unsigned int color) {
	unsigned int* pixel = (unsigned int*)buffer_memory;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			*pixel++ = color;
		}
	}
}

internal void
draw_rect_in_pixels(void* buffer_memory, int w, int h, int x0, int y0, int x1, int y1, unsigned int color) {

	x0 = clamp(0, x0, w);
	x1 = clamp(0, x1, w);
	y0 = clamp(0, y0, h);
	y1 = clamp(0, y1, h);


	for (int y = y0; y < y1; ++y) {
		unsigned int* pixel = (unsigned int*)buffer_memory + x0 + y * w;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

draw_rect(){}