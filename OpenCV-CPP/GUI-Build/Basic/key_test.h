#ifndef KEY_TEST
#define KEY_TEST

#include "Core/Application.h"

#include <iostream>

static void key_call(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int state = glfwGetKey(window, GLFW_KEY_M);
	if (state) {
		std::cout << state << "\n";
	}
}

void key_test();


#endif