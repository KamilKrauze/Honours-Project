#include "key_test.h"

#include <cassert>

void key_test()
{
	GLFWwindow* window = Application::Get().GetWindow().get_window_native();
	assert(window != nullptr);

	int state = glfwGetKey(window, GLFW_KEY_M);
	if (state) {
		std::cout << state << "\n";
	}
}
