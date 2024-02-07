#include "Core/Application.h"
#include "Core/GUICore.h"
#include "Core/CAEHelper.h"
#include "Core/MediaManager.h"

#include "Basic/BasicGUI.h"
#include "Basic/key_test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

static void error_callback(int error, const char* description);

inline static void init();

Application* Application::s_appInstance = nullptr;
MediaManager* Application::s_MediaManager = nullptr;

Application::Application(int width, int height, std::string title)
{
	if (!s_appInstance) {
		s_appInstance = this;
	}

	m_window = new Window({width, height, title});
	s_MediaManager = new MediaManager();
	

	if (!glfwInit())
		exit(EXIT_FAILURE);
}

Application::~Application()
{
	//delete s_appInstance;
	delete s_MediaManager;
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
	SETUP_GUI_CONTEXT();

	// SetUp IO
	ImGuiIO& io = ImGui::GetIO(); (void)io; \
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	/* Enable Keyboard Controls */
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	/* Enable Gamepad Controls */
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		/* Enable Docking */
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		/* Enable Multi-Viewport / Platform Windows */

	// Setup Dear ImGui style
	GUI_DARK();

	// Setup Platform/Renderer backends
	SETUP_BACKENDS();

	init();

	glEnable(GL_TEXTURE_2D);

	MediaManager::Get().attach(0);

	int display_w, display_h;
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f,0.0f,0.0f,1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glfwPollEvents();

		// Start the Dear ImGui frame.
		START_GUI_FRAME();
		{
			// IMGUI CODE HERE....

			BGui::basic_gui();

		}

		// Rendering ImGUI frame.
		RENDER_GUI();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        UPDATE_AND_RENDER();

		glfwSwapBuffers(window);
	}

	MediaManager::Get().dettach();

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


inline static void init()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.21f, 0.19f, 0.23f, 1.0f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.215f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.215f, 0.29f, 0.50f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.215f, 0.29f, 1.00f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}