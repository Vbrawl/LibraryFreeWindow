#include "Window.h"

const char* WindowNotCreated::what() const throw() { return "Window was not created!"; }


#ifdef _WIN32

void Window::initialize() {
	WNDCLASS winClass = {};
	winClass.lpfnWndProc = Window::static_HandleEvent;
	winClass.hInstance = GetModuleHandle(0);
	winClass.hbrBackground = CreateSolidBrush(0x00ffffff);
	winClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	winClass.cbWndExtra = sizeof(Window*);
	winClass.lpszClassName = LFW_DEFAULT_WINDOW_CLASS;

	RegisterClassW(&winClass);
}

void Window::deinitialize() {
	UnregisterClassW(LFW_DEFAULT_WINDOW_CLASS, GetModuleHandle(0));
}

LRESULT CALLBACK Window::static_HandleEvent(HWND win, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool runDefault = true;
	LRESULT result = 0;

	Window* winobj = (Window*)GetWindowLongPtrW(win, GWLP_USERDATA);

	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(win);
		break;
	case WM_DESTROY:
		winobj->m_eventLoop.RemoveEventProcessorLater(winobj);
		break;
	default:
		// If we haven't defined a handler for the message
		// then just let the default handler do it's job.
		result = DefWindowProcW(win, msg, wParam, lParam);
		break;
	}
	return result;
}


Window::Window(EventLoop& loop, int width, int height, bool global) : m_eventLoop(loop) {
	this->m_winHandle = CreateWindowW(
		LFW_DEFAULT_WINDOW_CLASS,
		TEXT("Main Window"),
		(global ? CS_GLOBALCLASS : 0) | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	if (this->m_winHandle == NULL) {
		throw WindowNotCreated();
	}
	else {
		SetWindowLongPtrW(this->m_winHandle, GWLP_USERDATA, (LONG_PTR)this);
	}
}

Window::~Window() {
	DestroyWindow(this->m_winHandle);
}

Event Window::findNextEvent() {
	struct Event evt;
	evt.code = UndefinedEvent;
	evt.data = std::make_shared<MSG>();
	if (PeekMessageW((MSG*)evt.data.get(), this->m_winHandle, NULL, NULL, PM_REMOVE)) {
		evt.code = WindowEvent;
	}
	else {
		evt.code = NoEvent;
	}

	return evt;
}

bool Window::processEvent(const Event& event) {
	bool processed = false;

	switch (event.code) {
	case WindowEvent:
		MSG* data = (MSG*)event.data.get();
		TranslateMessage(data);
		DispatchMessageW(data);
		processed = true;
	}
	return processed;
}
#endif