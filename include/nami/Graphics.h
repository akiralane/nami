#ifndef NAMI_GRAPHICS_H
#define NAMI_GRAPHICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

namespace Graphics {

    void error_callback(int error, const char* desc);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    /**
     * Sets up GLFW and GLAD, creating a new window
     * @param window The pointer in which the window will be placed
     */
    void init(GLFWwindow* &window);

    /**
     * Begins the main render loop
     * @param window The window to render to
     */
    void start_render_loop(GLFWwindow* window);

}

#endif
