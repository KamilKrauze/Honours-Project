#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window() = default;
	~Window() = default;

	GLFWwindow* GetNativeWindow() const { return m_window; }

private:
	GLFWwindow* m_window;
};

#endif