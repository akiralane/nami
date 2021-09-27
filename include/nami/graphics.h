#ifndef NAMI_GRAPHICS_H
#define NAMI_GRAPHICS_H

#include "nami/wave.h"
#include "nami/smoke.h"

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
        void generate_shader_program(unsigned int &program, const char *vLocation, const char *fLocation);
        void generate_texture(unsigned int &texture, const char* location);

        /**
         * Appends the vertices of a cuboid to a buffer according to the given dimensions
         * @param buffer The buffer to append to
         * @param scale The size of the cuboid (x, y, z)
         * @param position An offset to add to the cuboid's vertices
         * @param shouldTile OPTIONAL: true if the cuboid's texture should maintain a fixed ratio+size, or false if it should stretch to fill each face
         */
        void buffer_cuboid(std::vector<float> &buffer, glm::vec3 scale, glm::vec3 position, bool shouldTile = true);

        void generate_wave_model(unsigned int &vao, unsigned int &vbo, unsigned int &ibo);
        void generate_house_model(unsigned int &vao, unsigned int &vbo);
        void generate_smoke_model(unsigned int &vao, unsigned int &vbo);
        void generate_background_model(unsigned int &vao, unsigned int &vbo);
    }

    namespace core {

        /**
         * Performs initialisation and setup of the OpenGL environment. Creates a new window to display in.
         * @param window The pointer to hold the window
         */
        void init(GLFWwindow *&window);

        /**
         * Begins the main render loop.
         * @param window The window to render to
         */
        void start_render_loop(GLFWwindow *window);

    }

}

#endif // NAMI_GRAPHICS_H
