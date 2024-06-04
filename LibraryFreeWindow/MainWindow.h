#pragma once
#include "Window.h"
#include "EventLoop.h"

class MainWindow : public Window {
private:
	HWND btn1;
public:
	MainWindow(EventLoop& loop, int width = 1280, int height = 720);
};

