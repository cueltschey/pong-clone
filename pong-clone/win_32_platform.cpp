#include <windows.h>
#include "utils.cpp"

global_var bool running = true;

struct Render_State {
	int height, width;
	void* buffer_memory;
	BITMAPINFO	buffer_bitmap_info;
};
global_var Render_State renderstate;
#include "render.cpp"


LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;


	switch (uMsg){
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderstate.width = rect.right - rect.left;
			renderstate.height = rect.bottom - rect.top;

			int buffer_size = renderstate.width * renderstate.height * sizeof(unsigned int);
			if (renderstate.buffer_memory) VirtualFree(renderstate.buffer_memory, 0, MEM_RELEASE);
			renderstate.buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			renderstate.buffer_bitmap_info.bmiHeader.biSize = sizeof(renderstate.buffer_bitmap_info.bmiHeader);
			renderstate.buffer_bitmap_info.bmiHeader.biWidth = renderstate.width;
			renderstate.buffer_bitmap_info.bmiHeader.biHeight = renderstate.height;
			renderstate.buffer_bitmap_info.bmiHeader.biPlanes = 1;
			renderstate.buffer_bitmap_info.bmiHeader.biBitCount = 32;
			renderstate.buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
		} break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	//ShowCursor(FALSE);

	// create WNDCLASS
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	LPCWSTR name = L"Pong Window CLass";
	LPCWSTR title = L"Pong!!";
	window_class.lpszClassName = name;
	window_class.lpfnWndProc = window_callback;

	RegisterClass(&window_class);

	// Create window instance
	HWND window = CreateWindow(window_class.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);


	while (running) {
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		clear_screen(0xff5500);
		draw_rect(0, 0, .2, .2, 0x00ff22);
		StretchDIBits(hdc, 0, 0, renderstate.width, renderstate.height, 0, 0, renderstate.width, renderstate.height, renderstate.buffer_memory, &renderstate.buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}