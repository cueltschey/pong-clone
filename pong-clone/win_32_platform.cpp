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
#include "platform_common.cpp"
#include "game.cpp"

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

	ShowCursor(FALSE);

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

	Input input = {};

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (running) {
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; ++i) {
			input.buttons[i].changed = false;
		}
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			switch (message.message) {
			case WM_KEYUP: {
				for (int i = 0; i < BUTTON_COUNT; ++i) input.buttons[i].is_down = false;
			} break;
			case WM_KEYDOWN: {
				unsigned int vk_code = (unsigned int)message.wParam;
				bool is_down = ((message.lParam & (1 << 32)) == 0);


#define process_b(b, vk)\
case vk:{\
input.buttons[b].is_down = is_down;\
input.buttons[b].changed = true;} break;

				switch (vk_code) {
					process_b(BUTTON_UP, VK_UP);
					process_b(BUTTON_DOWN, VK_DOWN);
					process_b(BUTTON_LEFT, VK_LEFT);
					process_b(BUTTON_RIGHT, VK_RIGHT);
				}
			} break;
			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		  }
		}

		// simulate game frame
		simulate_game(&input, delta_time);

		// render pixels
		StretchDIBits(hdc, 0, 0, renderstate.width, renderstate.height, 0, 0, renderstate.width, renderstate.height, renderstate.buffer_memory, &renderstate.buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
}