#ifndef KEY_TEST
#define KEY_TEST

#include "Core/Application.h"
#include "Core/MediaManager.h"

#include <iostream>

static void key_call(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto index = MediaManager::Get().get_current_index();

	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		MediaManager::Get().bind(index + 1);

	if (glfwGetKey(window, GLFW_KEY_LEFT))
		MediaManager::Get().bind(index - 1);

}


#endif