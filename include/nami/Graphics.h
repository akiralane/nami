#ifndef NAMI_GRAPHICS_H
#define NAMI_GRAPHICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

namespace Graphics {

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
