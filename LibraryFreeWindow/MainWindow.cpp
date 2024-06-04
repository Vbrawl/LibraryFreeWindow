#include "MainWindow.h"
#include <math.h>


MainWindow::MainWindow(EventLoop& loop, int width, int height)
	: Window(loop, width, height, true) {

	static int BTN_WIDTH = 100;
	static int BTN_HEIGHT = 30;

	this->btn1 = CreateWindowW(
		L"BUTTON",
		L"Hello World",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		width / 2 - BTN_WIDTH / 2,
		height / 2 - BTN_HEIGHT / 2,
		BTN_WIDTH,
		BTN_HEIGHT,
		this->m_winHandle,
		NULL,
		(HINSTANCE)GetWindowLongPtr(this->m_winHandle, GWLP_HINSTANCE),
		NULL);
}