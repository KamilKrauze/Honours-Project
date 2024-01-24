//#pragma comment(lib, "opengl32.lib")

#include <iostream>

#include "Core/Application.h"

int main() {
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfwSetErrorCallback(error_callback);

    //if (!glfwInit())
    //    exit(EXIT_FAILURE);

    ////auto win = static_cast<GLFWwindow*>(Application::Application().Get().GetWindow().GetNativeWindow());
    //window = glfwCreateWindow(800, 800, "Honours Project", NULL, NULL);
    //if (!window)
    //{
    //    glfwTerminate();
    //    exit(EXIT_FAILURE);
    //}

    //glfwMakeContextCurrent(window);

    //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    //{
    //    std::cout << "Failed to initialize GLAD. Exiting." << std::endl;
    //    return -1;
    //}

    //while (!glfwWindowShouldClose(window))
    //{
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //    glfwSwapBuffers(window);

    //    glfwPollEvents();
    //}

    //glfwDestroyWindow(window);
    //glfwTerminate();

    Application app(800, 800, "Honours Project");
    return app.run();

    //return 0;
}