#include "Core/Application.h"
#include "Core/GUICore.h"

#include "Basic/BasicGUI.h"
#include "Basic/key_test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

	glfwSetKeyCallback(window, key_call);

	// Setup Dear ImGui context
	SETUP_GUI();

	// Setup Dear ImGui style
	GUI_DARK();

	// Setup Platform/Renderer backends
	SETUP_BACKENDS();


	int display_w, display_h;
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f,0.0f,0.0f,1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		key_test();

		glfwPollEvents();

		//key_test();

		// Start the Dear ImGui frame
		START_GUI_FRAME();

		// Enter GUI code here...
		BGui::basic_gui();

		// Rendering
		RENDER_GUI();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		UPDATE_AND_RENDER();

		glfwSwapBuffers(window);
	}

	// Cleanup
	CLEANUP_GUI();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}