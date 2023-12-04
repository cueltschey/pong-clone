#include <windows.h>

bool running = true;
int w;
int h;
void* buffer_memory;
BITMAPINFO	buffer_bitmap_info;

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
			w = rect.right - rect.left;
			h = rect.bottom - rect.top;

			int buffer_size = w * h * sizeof(unsigned int);
			if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);
			buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
			buffer_bitmap_info.bmiHeader.biWidth = w;
			buffer_bitmap_info.bmiHeader.biHeight = h;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
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
		unsigned int* pixel = (unsigned int*)buffer_memory;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				*pixel++ = i * j;
			}
		}
		StretchDIBits(hdc, 0, 0, w, h, 0, 0, w, h, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

	
}