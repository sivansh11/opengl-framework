#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char *name)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            RUNTIME_ASSERT(false, "GLFW failed to initialize!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, name, NULL, NULL);
        ASSERT(window != nullptr, "GLFW failed to create window");

        glfwMakeContextCurrent(window);

        // glfwSwapInterval(0);

        gladLoadGL();
    }
    ~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void startFrame()
    {
        glfwPollEvents();
    }
    void endFrame()
    {
        glfwSwapBuffers(window);
    }
    GLFWwindow* getGLFWwindow() { return window; }
    bool shouldClose() { return glfwWindowShouldClose(window); }

    void size(int &width, int height) { glfwGetWindowSize(window, &width, &height); }

private:
    GLFWwindow* window;
};

#endif