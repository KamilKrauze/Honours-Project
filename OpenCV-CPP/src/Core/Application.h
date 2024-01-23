#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Window.h"

class Application
{
public:
	Application()
	{
		s_app = *this;
	}

private:
	inline static Window window;
	static Application s_app;

public:
	Application& AppWindow() { return s_app; }
};

#endif // !1