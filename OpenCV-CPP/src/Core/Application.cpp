#include "Core/Application.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description);

Application* Application::s_appInstance = nullptr;

Application::Application(int width, int height, std::string title)
{
	if (!s_appInstance) {
		s_appInstance = this;
	}

	m_window = new Window({width, height, title});

	if (!glfwInit())
		exit(EXIT_FAILURE);
}

Application::~Application()
{
}

int Application::run()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = m_window->get_window_native();
	window = glfwCreateWindow(m_window->GetWidth(), m_window->GetHeight(), m_window->GetTitle(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD. Exiting." << std::endl;
		exit(EXIT_FAILURE);
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	delete[] window;

	return 0;
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}