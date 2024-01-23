#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window() = default;
	
	~Window() { delete window; }

	GLFWwindow* Get() { return window; }

private:
	GLFWwindow* window;
};

#endif