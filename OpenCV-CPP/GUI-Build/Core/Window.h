#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowSpecification {
	int width = 0;
	int height = 0;
	std::string title = "";

	WindowSpecification() = default;
	WindowSpecification(int width, int height, std::string_view title) : width(width), height(height), title(title) {}
	~WindowSpecification() = default;
};

class Window
{

public:

	Window(WindowSpecification spec) { this->m_spec = std::move(spec); }
	~Window() = default;

public:
	inline GLFWwindow* get_window_native() { return window; }

	int GetWidth() { return m_spec.width; }
	int GetHeight() { return m_spec.height; }
	const char* GetTitle() { return std::string_view{ m_spec.title }.data(); }

private:
	WindowSpecification m_spec;
	GLFWwindow* window;

};

#endif