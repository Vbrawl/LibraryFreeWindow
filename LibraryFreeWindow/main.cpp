#include <windows.h>
#include <memory>
#include "Window.h"
#include "MainWindow.h"
#include "EventLoop.h"

using std::make_shared;

int CALLBACK WinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE prevhinstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	EventLoop loop;

	Window::initialize();

	auto win = make_shared<MainWindow>(loop);
	loop.AddEventProcessor(win);

	loop.Start();

	Window::deinitialize();


	return 0;
}