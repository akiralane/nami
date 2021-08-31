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

    void generate_wave_model(unsigned int &vao, unsigned int &vbo, unsigned int &texture) {

        // the element buffer object stores indices that OpenGL uses to decide which vertices to draw
        // see "indexed drawing" below
        unsigned int ebo;

        // create the initial values for the heightmap
        wave::update_heights(0);

        float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
        std::vector<float> vecStream = wave::get_vector_stream();
        std::copy(vecStream.begin(), vecStream.end(), waveData); // seems expensive - is there a better way?

        // indexed drawing - the triangles have some overlapping vertices,
        // so we can tell OpenGL to pick the existing vertices that we want instead of writing them out again.
        int indices[wave::INDEX_COUNT];
        wave::generate_indices(indices);

        glGenVertexArrays(1, &vao);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waveData), waveData, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(wave::PRIMITIVE_RESTART_INDEX);

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