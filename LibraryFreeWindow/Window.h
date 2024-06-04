#pragma once


#include <Windows.h>
#include <exception>
#include "EventLoop.h"
#define LFW_DEFAULT_WINDOW_CLASS L"LFW_DefaultWindowClass"

class WindowNotCreated : public std::exception {
	virtual const char* what() const throw();
};


class Window : public EventProcessor {
/* Static Methods */
public:
	static void initialize();
	static void deinitialize();
#ifdef _WIN32
	static LRESULT CALLBACK static_HandleEvent(HWND win, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

/* Platform-specific variables */
protected:
#ifdef _WIN32
	HWND m_winHandle;
#endif

/* Variables */
protected:
	EventLoop& m_eventLoop;


/* Methods */
public:
	Window(EventLoop& loop, int width, int height, bool global = false);
	~Window();

	virtual Event findNextEvent() override;
	virtual bool processEvent(const Event& event) override;
};
