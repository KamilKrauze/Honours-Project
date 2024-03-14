#ifndef GUI_H
#define GUI_H

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#ifndef IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot/implot.h>


#define SETUP_GUI_CONTEXT() \
	IMGUI_CHECKVERSION(); \
	ImGui::CreateContext(); \
	ImPlot::CreateContext()

#define GUI_DARK() ImGui::StyleColorsDark()
#define GUI_LIGHT() ImGui::StyleColorsLight()

#define Stuff

#define SETUP_BACKENDS() \
	ImGui_ImplGlfw_InitForOpenGL(window, true); \
	ImGui_ImplOpenGL3_Init("#version 450")

#define START_GUI_FRAME() \
	ImGui_ImplOpenGL3_NewFrame(); \
	ImGui_ImplGlfw_NewFrame(); \
	ImGui::NewFrame()

#define RENDER_GUI() ImGui::Render()
		

// Update and Render additional Platform Windows
// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
#if _WIN32
#define UPDATE_AND_RENDER() \
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)	\
		{ \
			GLFWwindow* backup_current_context = glfwGetCurrentContext(); \
			ImGui::UpdatePlatformWindows(); \
			ImGui::RenderPlatformWindowsDefault(); \
			glfwMakeContextCurrent(backup_current_context); \
		}
#elif __linux__
UPDATE_AND_RENDER()
#endif

#define CLEANUP_GUI() \
	ImGui_ImplOpenGL3_Shutdown(); \
	ImGui_ImplGlfw_Shutdown(); \
	ImGui::DestroyContext(); \
	ImPlot::DestroyContext()

#endif
