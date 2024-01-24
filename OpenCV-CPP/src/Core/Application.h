#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"

class Application
{
public:
	Application()
	{
		m_window = new Window();
		s_app = this;
	}

	~Application()
	{
		delete s_app;
	}

public:
	Window& GetWindow() { return *m_window; }
	static Application* Get() { return s_app; }


private:
	Window* m_window;
	static Application* s_app;
};

#endif // !1