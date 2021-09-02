#include "nami/graphics.h"

#include <stb_image/stb_image.h>

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGHT = 600;

namespace graphics::core {

    namespace {

        bool isWireframe = false;

        void toggle_wireframe() {
            if (isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            isWireframe = !isWireframe;
        }

        void error_callback(int error, const char *desc) {
            fprintf(stderr, "something's gone wrong! error %i: %s\n", error, desc);
        }

        void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS) {
                switch (key) {
                    case GLFW_KEY_ESCAPE:
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                        break;
                    case GLFW_KEY_E:
                        toggle_wireframe();
                        break;
                    default:
                        break;
                }
            }
        }

        void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
        }

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

        // load and set window icon
        GLFWimage icon[1];
        icon[0].pixels = stbi_load("..\\assets\\water.bmp", &icon[0].width, &icon[0].height, nullptr, 4);
        glfwSetWindowIcon(window, 1, icon);
        stbi_image_free(icon[0].pixels);

        gladLoadGL();

        glEnable(GL_DEPTH_TEST);

    }

    void start_render_loop(GLFWwindow* window) {

        // a model matrix transforms the object's vertices into the world space
        // currently unused...
        glm::mat4 modelMat = glm::mat4(1.0f);

        // initially, the camera is at the world space origin
        // the view matrix transforms this to wherever you need it to be
        glm::mat4 viewMat = glm::mat4(1.0f);
        viewMat = glm::translate(viewMat, glm::vec3(-5.0f, -2.0f, -20.0f));
        viewMat = glm::rotate(viewMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, -5.0f));

        // the projection matrix determines the perspective of the view
        // FOV, ortho vs perspective, etc.
        glm::mat4 projectionMat;
        projectionMat = glm::perspective(
                glm::radians(45.0f), float(DEFAULT_WINDOW_WIDTH) / float(DEFAULT_WINDOW_HEIGHT),
                0.1f, 100.f
        );

        unsigned int shaderProgram;
        generation::generate_shader_program(shaderProgram);

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int waveVbo, waveTexture;
        generation::generate_wave_model(vao, waveVbo, waveTexture);

        // ==== specify attributes ====
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(0); // vertex positions
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // texture coordinates

        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.2, 0.3, 0.3, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            // ==== VIEW ====
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "modelMat");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "viewMat");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projectionMat");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));

            // ==== WAVES ====
            wave::update_heights(float(glfwGetTime()));

            float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
            std::vector<float> vecStream = wave::get_vector_stream();
            std::copy(vecStream.begin(), vecStream.end(), waveData); // there's definitely a better way TODO

            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, waveTexture);

            // update the waveVbo with the new heights
            glBindBuffer(GL_ARRAY_BUFFER, waveVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(waveData), waveData);

            // draw the strips of triangle for the wave (glDrawElements because we're using an EBO)
            glDrawElements(GL_TRIANGLE_STRIP, wave::INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window);
            glfwPollEvents();

        }

        glfwDestroyWindow(window);
        glfwTerminate();

    }

}
