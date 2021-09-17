#include "nami/graphics.h"

#include "nami/camera.h"

#include <stb_image/stb_image.h>

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGHT = 600;

Camera camera(glm::vec3(5, 1, 15), glm::vec3(0, 1, 0));

namespace graphics::core {

    bool initialClick = true;
    double previousMouseX = DEFAULT_WINDOW_WIDTH / 2.0f;
    double previousMouseY = DEFAULT_WINDOW_HEIGHT / 2.0f;

    namespace {

        bool isWireframe = false;
        bool windowHasFocus = true;

        // ==== util functions ====

        void toggle_wireframe() {
            if (isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            isWireframe = !isWireframe;
        }

        void print_camera_pos() {
            glm::vec3 position = camera.getPosition();
            glm::vec2 rotation = camera.getRotation();
            std::cout << "Camera is at world position ("
                      << position.x << ", " << position.y << ", " << position.z
                      << "): pitch " << rotation.x << ", yaw " << rotation.y
                      << std::endl;
        }

        // this needs to exist because the key callback only fires once on keydown
        // you can't tell whether a key is being held or not
        void handleKeyInput(GLFWwindow *window, Camera &camera) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.move(Camera::Direction::FORWARDS); }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.move(Camera::Direction::LEFT); }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.move(Camera::Direction::BACKWARDS);}
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.move(Camera::Direction::RIGHT); }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { camera.move(Camera::Direction::UP); }
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { camera.move(Camera::Direction::DOWN); }
        }

        // ==== callbacks ====

        void error_callback(int error, const char *desc) {
            fprintf(stderr, "something's gone wrong! error %i: %s\n", error, desc);
        }

        void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                windowHasFocus = true;
            }
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
                    case GLFW_KEY_TAB:
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        windowHasFocus = false;
                        break;
                    case GLFW_KEY_Q:
                        print_camera_pos();
                        break;
                    default:
                        break;
                }
            }
        }

        void cursor_pos_callback(GLFWwindow *window, double x, double y) {

            if (initialClick) {
                previousMouseX = float(x);
                previousMouseY = float(y);
                initialClick = false;
            }

            auto xDifference = float(x - previousMouseX);
            auto yDifference = float(previousMouseY - y);
            previousMouseX = x;
            previousMouseY = y;

            if (windowHasFocus) {
                camera.look(xDifference, yDifference);
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
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_pos_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // resize viewport on window resize

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

        // ==== shaders, buffers etc. ====

        unsigned int stdShader;
        generation::generate_shader_program(
                stdShader,
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
        generation::generate_texture(backgroundTexture, "..\\assets\\clouds.bmp");

        unsigned int houseVao, houseVbo, woodTexture;
        generation::generate_house_model(houseVao, houseVbo);
        generation::generate_texture(woodTexture, "..\\assets\\wood.bmp");

        // ==== MVP matrices ====

        // a model matrix transforms the object's vertices into the world space
        glm::mat4 modelMat = glm::mat4(1.0f);

        // the view matrix transforms the camera
        // (this is unused - the camera class generates the view matrix now)
        glm::mat4 viewMat = glm::mat4(1.0f);

        // the projection matrix determines the perspective + fov
        glm::mat4 projectionMat;
        projectionMat = glm::perspective(
                glm::radians(45.0f), float(DEFAULT_WINDOW_WIDTH) / float(DEFAULT_WINDOW_HEIGHT),
                0.1f, 100.f
        );

        // ==== MAIN RENDER LOOP ====
        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.2, 0.3, 0.3, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            handleKeyInput(window, camera);

            // -------------------------------------------------------
            // ==== WAVES ====

            // generate and store the new sets of heights
            float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
            wave::update_heights(float(glfwGetTime()));
            std::vector<float> vecStream = wave::get_vector_stream();
            std::copy(vecStream.begin(), vecStream.end(), waveData);

            glBindVertexArray(waveVao);

            // uniforms
            // TODO: move code (up) to somewhere more intuitive
            glUseProgram(stdShader);
            glUniformMatrix4fv(glGetUniformLocation(stdShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
            glUniformMatrix4fv(glGetUniformLocation(stdShader, "viewMat"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
            glUniformMatrix4fv(glGetUniformLocation(stdShader, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

            // texture
            glBindTexture(GL_TEXTURE_2D, waveTexture);

            // drawing
            glBindBuffer(GL_ARRAY_BUFFER, waveVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(waveData), waveData); // update values in buffer with new ones
            glDrawElements(GL_TRIANGLE_STRIP, wave::INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

            // -------------------------------------------------------
            // ==== HOUSE ====

            glBindVertexArray(houseVao);

//            glm::mat4x4 houseModelMat = glm::mat4(1.0f);
            glm::mat4x4 houseModelMat = glm::translate(modelMat, glm::vec3(5.5, 0, 5.5));
//            houseModelMat = glm::rotate(houseModelMat, glm::radians(270.0f), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(glGetUniformLocation(stdShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(houseModelMat));

            glBindBuffer(GL_ARRAY_BUFFER, houseVbo);
            glBindTexture(GL_TEXTURE_2D, woodTexture);
            glDrawArrays(GL_TRIANGLES, 0, 9999); // TODO: 9999 IS STUPID

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
