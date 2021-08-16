#include "nami/Graphics.h"

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGHT = 600;

namespace Graphics {

    void error_callback(int error, const char* desc) {
        fprintf(stderr, "something's gone wrong! error %i: %s", error, desc);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void init(GLFWwindow* &window) {

        glfwInit();

        window = glfwCreateWindow(
                DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                "波", nullptr, nullptr
        );


        glfwSetKeyCallback(window, key_callback); // receive input
        glfwSetErrorCallback(error_callback); // report errors
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // resize viewport on window resize

        glfwMakeContextCurrent(window);

        gladLoadGL();

    }

    void start_render_loop(GLFWwindow* window) {

        while (!glfwWindowShouldClose(window)) {

            glfwSwapBuffers(window);
            glfwPollEvents();

        }

    }

}
