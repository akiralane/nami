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

        // ==== SHADERS ====

        unsigned int waveShader;
        generation::generate_shader_program(
                waveShader,
                "..\\shaders\\simple.vert", "..\\shaders\\simple.frag");

        unsigned int backgroundShader;
        generation::generate_shader_program(
                backgroundShader,
                "..\\shaders\\background.vert", "..\\shaders\\simple.frag");

        unsigned int waveVao, waveVbo, waveIbo, waveTexture;
        generation::generate_wave_model(waveVao, waveVbo, waveIbo);
        generation::generate_texture(waveTexture, "..\\assets\\water.bmp");

        unsigned int backgroundVao, backgroundVbo, backgroundIbo, backgroundTexture;
        generation::generate_background_model(backgroundVao, backgroundVbo, backgroundIbo);
        generation::generate_texture(backgroundTexture, "..\\assets\\cat.bmp");

        // ==== MVP matrices ====

        // a model matrix transforms the object's vertices into the world space
        glm::mat4 modelMat = glm::mat4(1.0f);

        // the view matrix transforms the camera
        glm::mat4 viewMat = glm::mat4(1.0f);
        viewMat = glm::translate(viewMat, glm::vec3(-5.0f, -2.0f, -10.0f));
        viewMat = glm::rotate(viewMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, -5.0f));
        viewMat = glm::rotate(viewMat, glm::radians(45.0f), glm::vec3(0, 1, 0));

        // the projection matrix determines the perspective
        glm::mat4 projectionMat;
        projectionMat = glm::perspective(
                glm::radians(45.0f), float(DEFAULT_WINDOW_WIDTH) / float(DEFAULT_WINDOW_HEIGHT),
                0.1f, 100.f
        );

        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.2, 0.3, 0.3, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // -------------------------------------------------------
            // ==== WAVES ====

            // generate and store the new sets of heights
            float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
            wave::update_heights(float(glfwGetTime()));
            std::vector<float> vecStream = wave::get_vector_stream();
            std::copy(vecStream.begin(), vecStream.end(), waveData);

            glBindVertexArray(waveVao);

            // uniforms
            glUseProgram(waveShader);
            glUniformMatrix4fv(glGetUniformLocation(waveShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
            glUniformMatrix4fv(glGetUniformLocation(waveShader, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
            glUniformMatrix4fv(glGetUniformLocation(waveShader, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

            // texture
            glBindTexture(GL_TEXTURE_2D, waveTexture);

            // drawing
            glBindBuffer(GL_ARRAY_BUFFER, waveVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(waveData), waveData); // update values in buffer with new ones
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waveIbo);
            glDrawElements(GL_TRIANGLE_STRIP, wave::INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

            // -------------------------------------------------------
            // ==== BACKGROUND ====
            // by rendering the background last, we reduce the number of fragments that the shader has to run on
            // this does, however, mean we have to trick opengl into thinking the background is always at maximum depth
            // this is done in the vertex shader, but in case anything is already at depth 1.0 we change the depth test condition here
            glDepthFunc(GL_LEQUAL);

            glBindVertexArray(backgroundVao);

            // uniforms
            glUseProgram(backgroundShader);
            glUniformMatrix4fv(glGetUniformLocation(backgroundShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
            glUniformMatrix4fv(glGetUniformLocation(backgroundShader, "viewMat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
            glUniformMatrix4fv(glGetUniformLocation(backgroundShader, "projectionMat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

            // texture
            glBindTexture(GL_TEXTURE_2D, backgroundTexture);

            // drawing
            glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundIbo);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDepthFunc(GL_LESS); // reset to normal

            int errorCode = glGetError();
            if (errorCode != 0) { std::cerr << "GL ERROR " << errorCode << std::endl; }

            glfwSwapBuffers(window);
            glfwPollEvents();

        }

        glfwDestroyWindow(window);
        glfwTerminate();

    }

}
