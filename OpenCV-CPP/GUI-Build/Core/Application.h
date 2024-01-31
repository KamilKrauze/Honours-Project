#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Window.h"

class MediaManager;

class Application
{

public:
	Application(int width, int height, std::string title);
	~Application();


public:
	int run();

public:
	Window& GetWindow() { return *m_window; }
	static Application& Get() { return *s_appInstance; }

private:
	Window* m_window;
	static MediaManager* s_MediaManager;

	static Application* s_appInstance;
};

#endif