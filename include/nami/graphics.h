#ifndef NAMI_GRAPHICS_H
#define NAMI_GRAPHICS_H

#include "nami/wave.h"

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

namespace graphics {

    namespace generation {
        void generate_shader_program(unsigned int &program);
        void generate_texture(unsigned int &texture, const char* location);
        void generate_wave_model(unsigned int &vao, unsigned int &vbo, unsigned int &ibo, unsigned int &texture);
        void generate_skybox_model(unsigned int &vao, unsigned int &vbo, unsigned int &ibo);
    }

    namespace core {

        /**
         * Sets up GLFW and GLAD, creating a new window
         * @param window The pointer in which the window will be placed
         */
        void init(GLFWwindow *&window);

        /**
         * Begins the main render loop
         * @param window The window to render to
         */
        void start_render_loop(GLFWwindow *window);

    }

}

#endif // NAMI_GRAPHICS_H
