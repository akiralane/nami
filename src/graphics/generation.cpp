#include "nami/graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace graphics::generation {

    char* read_shader_source(const std::string& path) {
        std::stringstream content;
        std::ifstream source (path);

        if (!source.is_open()) { fprintf(stderr, "couldn't read shader file at %s", path.c_str()); }

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

    void generate_texture(unsigned int &texture, const char* location) {

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(location, &width, &height, &nrChannels, 0);

        if (!data) { fprintf(stderr, "!! couldn't load texture at %s", location); }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    void generate_wave_model(unsigned int &vao, unsigned int &vbo, unsigned int &texture) {

        // the element buffer object stores indices that OpenGL uses to decide which vertices to draw
        // see "indexed drawing" below
        unsigned int ebo;

        // create the initial values for the heightmap
        wave::update_heights(0);

        float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
        std::vector<float> vecStream = wave::get_vector_stream();
        std::copy(vecStream.begin(), vecStream.end(), waveData); // seems expensive

        // indexed drawing - the triangles have some overlapping vertices,
        // so we can tell OpenGL to pick the existing vertices that we want instead of writing them out again.
        int indices[wave::INDEX_COUNT];
        wave::generate_indices(indices);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waveData), waveData, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(wave::PRIMITIVE_RESTART_INDEX);

        generate_texture(texture, "..\\assets\\water.bmp");
    }

    // https://stackoverflow.com/questions/25252512/how-can-i-pass-multiple-textures-to-a-single-shader
    // https://stackoverflow.com/questions/55800355/how-can-i-add-multiple-textures-to-my-opengl-program !! BETTER ONE !!
    void generate_skybox(unsigned int &vao, unsigned int &vbo, unsigned int &texture) {

        unsigned int ebo;

        float data[] = {
                /* TODO fill with data */
        };

        int indices[] = {
                /* TODO also fill with data */
        };

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        generate_texture(texture, "..\\assets\\cat.bmp");
    }

}