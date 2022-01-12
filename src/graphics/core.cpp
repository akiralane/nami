#include "nami/graphics.h"

#include "nami/camera.h"

#include <stb_image/stb_image.h>

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGHT = 600;

namespace graphics::core {

    Camera* camera;

    bool initialClick = true;
    double previousMouseX = DEFAULT_WINDOW_WIDTH / 2.0f;
    double previousMouseY = DEFAULT_WINDOW_HEIGHT / 2.0f;

    namespace {

        bool wireframe = false;
        bool windowHasFocus = true;

        // ==== util functions ====

        void toggle_wireframe() {
            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            wireframe = !wireframe;
        }

        void print_camera_pos(GLFWwindow* window) {
            auto camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            glm::vec3 position = camera->getPosition();
            glm::vec2 rotation = camera->getRotation();
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
                        print_camera_pos(window);
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
                auto camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
                camera->look(xDifference, yDifference);
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

        // by setting the camera object as our windowUserPointer, we can retrieve it
        // in the glfw callbacks where we would have otherwise needed to make it global
        // try (-1.35418, 2.47308, 1.71069) pitch -1.3, yaw 35.9004
        camera = new Camera(
                glm::vec3(1.5, 1.7, 1.5), glm::vec3(0, 0, -1),
                glm::vec3(0, 1, 0), 47, 2.19975
        );
        glfwSetWindowUserPointer(window, camera);

        glfwSetKeyCallback(window, key_callback); // receive key-downs (not presses!)
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

    void start_render_loop(GLFWwindow* window) {

        // ==== shaders, buffers, textures ====

        unsigned int stdShader;
        generation::generate_shader_program(
                stdShader,
                "..\\shaders\\simple.vert", "..\\shaders\\simple.frag");

        unsigned int waveShader;
        generation::generate_shader_program(
                waveShader,
                "..\\shaders\\simple.vert", "..\\shaders\\scrolling.frag");

        unsigned int smokeShader;
        generation::generate_shader_program(
                smokeShader,
                "..\\shaders\\smoke.vert", "..\\shaders\\smoke.frag");

        unsigned int backgroundShader;
        generation::generate_shader_program(
                backgroundShader,
                "..\\shaders\\background.vert", "..\\shaders\\simple.frag");

        unsigned int waveVao, waveVbo, waveIbo, waveTexture;
        generation::generate_wave_model(waveVao, waveVbo, waveIbo);
        generation::generate_texture(waveTexture, "..\\assets\\water.bmp");

        unsigned int houseVao, houseVbo;
        unsigned int woodTexture, wallFrontTexture, wallSideTexture, darkWoodTexture,
                roofFrontTexture, tilesTexture, stoneTexture;
        generation::generate_house_model(houseVao, houseVbo);
        generation::generate_texture(woodTexture, "..\\assets\\wood_light.bmp");
        generation::generate_texture(wallFrontTexture, "..\\assets\\house_door.bmp");
        generation::generate_texture(wallSideTexture, "..\\assets\\house_window.bmp");
        generation::generate_texture(darkWoodTexture, "..\\assets\\wood_dark.bmp");
        generation::generate_texture(roofFrontTexture, "..\\assets\\roof_front.bmp");
        generation::generate_texture(tilesTexture, "..\\assets\\tiles.bmp");
        generation::generate_texture(stoneTexture, "..\\assets\\cobble.bmp");

        unsigned int smokeVao, smokeVbo;
        generation::generate_smoke_model(smokeVao, smokeVbo);

        unsigned int backgroundVao, backgroundVbo, backgroundTexture;
        generation::generate_background_model(backgroundVao, backgroundVbo);
        generation::generate_texture(backgroundTexture, "..\\assets\\night_sky.bmp");

        // ==== common matrix declaration ====

        glm::mat4 identity = glm::mat4(1.0f);

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

            handleKeyInput(window, *camera);

            double time = glfwGetTime();

            // -------------------------------------------------------
            // ==== WAVES ====

            // generate and store the new sets of heights
            float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
            wave::update_heights(float(time));
            std::vector<float> waveVertexStream = wave::get_vertex_stream();
            std::copy(waveVertexStream.begin(), waveVertexStream.end(), waveData);

            glBindVertexArray(waveVao);

            glUseProgram(waveShader);
            glUniformMatrix4fv(glGetUniformLocation(waveShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
            glUniformMatrix4fv(glGetUniformLocation(waveShader, "viewMat"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
            glUniformMatrix4fv(glGetUniformLocation(waveShader, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));
            glUniform1f(glGetUniformLocation(waveShader, "time"), float(time/10));

            // texture
            glBindTexture(GL_TEXTURE_2D, waveTexture);

            // drawing
            glBindBuffer(GL_ARRAY_BUFFER, waveVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(waveData), waveData); // update values in buffer with new ones
            glDrawElements(GL_TRIANGLE_STRIP, wave::INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

            // -------------------------------------------------------
            // ==== HOUSE ====

            glBindVertexArray(houseVao);

            // uniforms
            glUseProgram(stdShader);

            glm::mat4x4 houseModelMat = glm::translate(glm::mat4(1), glm::vec3(10, 0, 8));
            houseModelMat = glm::rotate(houseModelMat, glm::radians(-90.0f), glm::vec3(0, 1, 0));

            glUniformMatrix4fv(glGetUniformLocation(stdShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(houseModelMat));
            glUniformMatrix4fv(glGetUniformLocation(stdShader, "viewMat"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
            glUniformMatrix4fv(glGetUniformLocation(stdShader, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

            // refer to the generation of the house mesh for the order of vertices
            // this is such a dumb system - write wrapper functions next time
            // better yet, don't manually write hundreds of vertices and just import a model instead
            glBindTexture(GL_TEXTURE_2D, woodTexture);
            glDrawArrays(GL_TRIANGLES, 0, 72);
            glBindTexture(GL_TEXTURE_2D, wallSideTexture);
            glDrawArrays(GL_TRIANGLES, 72, 18);
            glBindTexture(GL_TEXTURE_2D, wallFrontTexture);
            glDrawArrays(GL_TRIANGLES, 90, 18);
            glBindTexture(GL_TEXTURE_2D, darkWoodTexture);
            glDrawArrays(GL_TRIANGLES, 108, 252);
            glBindTexture(GL_TEXTURE_2D, roofFrontTexture);
            glDrawArrays(GL_TRIANGLES, 360, 6);
            glBindTexture(GL_TEXTURE_2D, tilesTexture);
            glDrawArrays(GL_TRIANGLES, 366, 12);
            glBindTexture(GL_TEXTURE_2D, stoneTexture);
            glDrawArrays(GL_TRIANGLES, 378, 36);

            // -------------------------------------------------------
            // ==== BACKGROUND ====
            // by rendering the background as late as we can, we reduce the number of fragments that the shader has to run on.
            // this does, however, mean we have to trick opengl into thinking the background is always at maximum depth.
            // this is done in its vertex shader, but in case anything is already at depth 1.0 we change the depth test condition here
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
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDepthFunc(GL_LESS); // reset to normal

            // -------------------------------------------------------
            // ==== SMOKE ====
            // we draw the smoke after the background because the transparency fails otherwise

            glBindVertexArray(0);

            // generation
            float smokeData[smoke::POINTS_HEIGHT * 2 * 3];
            smoke::update(float(time));
            std::vector<float> smokeVertexStream = smoke::get_vertex_stream();
            std::copy(smokeVertexStream.begin(), smokeVertexStream.end(), smokeData);

            // uniforms
            glUseProgram(smokeShader);
            glm::mat4 smokeModelMat = glm::translate(houseModelMat, glm::vec3(1.2f, 2.5f, 2.1f));
            smokeModelMat = glm::rotate(smokeModelMat, glm::radians(-45.0f), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(glGetUniformLocation(smokeShader, "modelMat"), 1, GL_FALSE, glm::value_ptr(smokeModelMat));
            glUniformMatrix4fv(glGetUniformLocation(smokeShader, "viewMat"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
            glUniformMatrix4fv(glGetUniformLocation(smokeShader, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

            // drawing
            glBindBuffer(GL_ARRAY_BUFFER, smokeVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(smokeData), smokeData);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);

            // -------------------------------------------------------
            // ==== END OF LOOP ====

            int errorCode = glGetError();
            if (errorCode != 0) { std::cerr << "GL ERROR " << errorCode << std::endl; }

            glfwPollEvents();
            glfwSwapBuffers(window);

        }

        glfwDestroyWindow(window);
        glfwTerminate();

    }

}
