#include "nami/graphics.h"
#include "nami/wave.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGHT = 600;


namespace graphics {

    namespace {

        bool isWireframe = false;

        void error_callback(int error, const char *desc) {
            fprintf(stderr, "something's gone wrong! error %i: %s", error, desc);
        }

        void toggle_wireframe() {
            if (isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            isWireframe = !isWireframe;
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

        char* read_shader_source(const std::string& path) {
            std::stringstream content;
            std::ifstream source (path);

            if (!source.is_open()) { fprintf(stderr, "couldn't read file at %s", path.c_str()); }

            content << source.rdbuf();
            source.close();

            return strdup(content.str().c_str());
        }

        void generate_shader_program(unsigned int &program) {
            char* vertexShaderSource = read_shader_source("..\\shaders\\simple.vert");
            char* fragmentShaderSource = read_shader_source("..\\shaders\\simple.frag");

            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
            glCompileShader(vertexShader);

            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
            glCompileShader(fragmentShader);

            program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        void generate_texture(unsigned int &texture) {

            // texture parameters for this texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            int width, height, nrChannels;
            unsigned char *data = stbi_load("..\\assets\\water.bmp", &width, &height, &nrChannels, 0);

            if (!data) { fprintf(stderr, "couldn't load texture!"); }

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);

        }

        void generate_model(unsigned int &vao, unsigned int &texture) {

            // the element buffer object stores indices that OpenGL uses to decide which vertices to draw
            // see "indexed drawing" below
            unsigned int vbo, ebo;

            float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
            std::vector<float> vecStream = wave::get_vector_stream();
            std::copy(vecStream.begin(), vecStream.end(), waveData); // seems expensive - is there a better way?

            // indexed drawing - the rectangle's triangles have some overlapping vertices,
            // so we can tell OpenGL to pick the existing vertices that we want instead of writing them out again.
            // this is significantly more important when models become more complicated than two triangles
            // also, it seems like a pain to generate programmatically
            int indices[] = {
                    0, 4,  1, 5,  2,  6,  3,  7,     /* RESTART */ 16,
                    4, 8,  5, 9,  6,  10, 7,  11,    /* RESTART */ 16,
                    8, 12, 9, 13, 10, 14, 11, 15     /* NO RESTART */
            };

            glGenVertexArrays(1, &vao);

            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(waveData), waveData, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(16);

            generate_texture(texture);

            // remember that the attribute stride has to be 5*sizeof(float) because we added the texture coordinates
            // (so that's the 3 from the 3d vertex positions plus the 2 from the 2d texture coords)
            // it's easier to see if you just look at the vertices array
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
            glEnableVertexAttribArray(0); // vertex positions

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
            glEnableVertexAttribArray(1); // texture coordinates

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

        gladLoadGL();

    }

    void start_render_loop(GLFWwindow* window) {

        // a model matrix transforms the object's vertices into the world space
        // TODO: this is an example rotation!! you'll need something different probably
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::rotate(modelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // initially, the camera is at the world space origin
        // the view matrix transforms this to wherever you need it to be
        glm::mat4 viewMat = glm::mat4(1.0f);
        viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -5.0f));

        // the projection matrix determines the perspective of the view
        // FOV, ortho vs perspective, etc.
        glm::mat4 projectionMat;
        projectionMat = glm::perspective(
                glm::radians(45.0f), float(DEFAULT_WINDOW_WIDTH) / float(DEFAULT_WINDOW_HEIGHT),
                0.1f, 100.f
        );

        unsigned int shaderProgram;
        generate_shader_program(shaderProgram);

        unsigned int vao, texture;
        generate_model(vao, texture);

        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.2, 0.3, 0.3, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, texture);

            // send the matrices to the vertex shader
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "modelMat");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "viewMat");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projectionMat");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));

            // draw the rectangle (glDrawElements because we're using an EBO)
            // TODO: CHANGE THIS NUMBER DEPENDING ON HOW MANY INDICES THERE ARE
            glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window);
            glfwPollEvents();

        }

        glfwDestroyWindow(window);
        glfwTerminate();

    }

}
