#include "nami/graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace graphics::generation {

    char* read_shader_source(const std::string& path) {
        std::stringstream content;
        std::ifstream source (path);

        if (!source.is_open()) { fprintf(stderr, "couldn't read shader file at %s\n", path.c_str()); }

        content << source.rdbuf();
        source.close();

        return strdup(content.str().c_str());
    }

    void generate_shader_program(unsigned int &program, const char *vLocation, const char *fLocation) {

        GLint compileStatus = GL_FALSE;

        // load and compile vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        char* vertexShaderSource = read_shader_source(vLocation);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        // check compile status
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE) {
            std::cerr << "VERTEX SHADER AT "<< vLocation << " FAILED TO COMPILE" << std::endl;
        }

        // load and compile fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        char* fragmentShaderSource = read_shader_source(fLocation);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        // check compile status
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE) {
            std::cerr << "FRAGMENT SHADER AT "<< fLocation << " FAILED TO COMPILE" << std::endl;
        }

        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void generate_texture(unsigned int &texture, const char* location) {

        stbi_set_flip_vertically_on_load(1);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(location, &width, &height, &nrChannels, 0);

        if (!data) { fprintf(stderr, "!! couldn't load texture at %s\n", location); }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    void buffer_cuboid(std::vector<float> &buffer, glm::vec3 scale, glm::vec3 offset, bool shouldTile) {

        glm::vec3 texScale;
        if (shouldTile) {
            texScale = {scale.x, scale.y, scale.z};
        } else {
            texScale = {1.0f, 1.0f, 1.0f};
        }

        // one 'group' per face
        std::vector<float> data = {

                // xy front
                0       + offset.x, 0        + offset.y, 0        + offset.z,      0.0f,       0.0f,
                scale.x + offset.x, 0        + offset.y, 0        + offset.z,      texScale.x, 0.0f,
                scale.x + offset.x,  scale.y + offset.y, 0        + offset.z,      texScale.x, texScale.y,
                scale.x + offset.x,  scale.y + offset.y, 0        + offset.z,      texScale.x, texScale.y,
                0       + offset.x,  scale.y + offset.y, 0        + offset.z,      0.0f,       texScale.y,
                0       + offset.x, 0        + offset.y, 0        + offset.z,      0.0f,       0.0f,

                // xy back
                0       + offset.x, 0       + offset.y,  scale.z + offset.z,       0.0f,       0.0f,
                scale.x + offset.x, 0       + offset.y,  scale.z + offset.z,       texScale.x, 0.0f,
                scale.x + offset.x, scale.y + offset.y,  scale.z + offset.z,       texScale.x, texScale.y,
                scale.x + offset.x, scale.y + offset.y,  scale.z + offset.z,       texScale.x, texScale.y,
                0       + offset.x, scale.y + offset.y,  scale.z + offset.z,       0.0f,       texScale.y,
                0       + offset.x, 0       + offset.y,  scale.z + offset.z,       0.0f,       0.0f,

                // yz front
                scale.x + offset.x,  scale.y + offset.y,  scale.z + offset.z,      0.0f,       0.0f,
                scale.x + offset.x,  scale.y + offset.y, 0        + offset.z,      texScale.z, 0.0f,
                scale.x + offset.x, 0        + offset.y, 0        + offset.z,      texScale.z, texScale.y,
                scale.x + offset.x, 0        + offset.y, 0        + offset.z,      texScale.z, texScale.y,
                scale.x + offset.x, 0        + offset.y,  scale.z + offset.z,      0.0f,       texScale.y,
                scale.x + offset.x,  scale.y + offset.y,  scale.z + offset.z,      0.0f,       0.0f,

                // yz back
                0       + offset.x,  scale.y + offset.y,  scale.z + offset.z,      0.0f,       0.0f,
                0       + offset.x,  scale.y + offset.y, 0        + offset.z,      texScale.z, 0.0f,
                0       + offset.x, 0        + offset.y, 0        + offset.z,      texScale.z, texScale.y,
                0       + offset.x, 0        + offset.y, 0        + offset.z,      texScale.z, texScale.y,
                0       + offset.x, 0        + offset.y,  scale.z + offset.z,      0.0f,       texScale.y,
                0       + offset.x,  scale.y + offset.y,  scale.z + offset.z,      0.0f,       0.0f,

                // TOP
                0       + offset.x, 0        + offset.y, 0        + offset.z,      0.0f,       0.0f,
                scale.x + offset.x, 0        + offset.y, 0        + offset.z,      texScale.x, 0.0f,
                scale.x + offset.x, 0        + offset.y,  scale.z + offset.z,      texScale.x, texScale.z,
                scale.x + offset.x, 0        + offset.y,  scale.z + offset.z,      texScale.x, texScale.z,
                0       + offset.x, 0        + offset.y,  scale.z + offset.z,      0.0f,       texScale.z,
                0       + offset.x, 0        + offset.y, 0        + offset.z,      0.0f,       0.0f,

                // BOTTOM
                0       + offset.x,  scale.y + offset.y, 0        + offset.z,      0.0f,       0.0f,
                scale.x + offset.x,  scale.y + offset.y, 0        + offset.z,      texScale.x, 0.0f,
                scale.x + offset.x,  scale.y + offset.y,  scale.z + offset.z,      texScale.x, texScale.z,
                scale.x + offset.x,  scale.y + offset.y,  scale.z + offset.z,      texScale.x, texScale.z,
                0       + offset.x,  scale.y + offset.y,  scale.z + offset.z,      0.0f,       texScale.z,
                0       + offset.x,  scale.y + offset.y, 0        + offset.z,      0.0f,       0.0f
        };

        buffer.insert(buffer.end(), data.begin(), data.end());

    }

    void generate_wave_model(unsigned int &vao, unsigned int &vbo, unsigned int &ibo) {

        // populate heightmap with initial values
        wave::update_heights(0);

        float waveData[wave::GRID_SIZE * wave::GRID_SIZE * 5];
        std::vector<float> vecStream = wave::get_vector_stream();
        std::copy(vecStream.begin(), vecStream.end(), waveData); // seems expensive

        // indexed drawing - the triangles have some overlapping vertices,
        // so we can tell OpenGL to pick the existing vertices that we want instead of writing them out again.
        int indices[wave::INDEX_COUNT];
        wave::generate_indices(indices);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        // ==== buffer data ====
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waveData), waveData, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(wave::PRIMITIVE_RESTART_INDEX);

        // ==== specify attributes ====
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(0); // vertex positions
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // texture coordinates
    }

    void generate_house_model(unsigned int &vao, unsigned int &vbo) {

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        std::vector<float> buffer;

        // main platform [0+36]
        buffer_cuboid(buffer, glm::vec3(2.5, 0.1, 3.5), glm::vec3(0, 0.5, 0));

        // jetty platform [36+36]
        buffer_cuboid(buffer, glm::vec3(2, 0.1, 1.5), glm::vec3(-2, 0.5, 1));

        // main building body [72+36]
        buffer_cuboid(buffer, glm::vec3(2, 1.3, 2.2), glm::vec3(0.3, 0.6, 0.65), false);

        // main supports [108+144]
        buffer_cuboid(buffer, glm::vec3(0.3, 2, 0.3), glm::vec3(0, -1.5, 0));
        buffer_cuboid(buffer, glm::vec3(0.3, 2, 0.3), glm::vec3(0, -1.5, 3.2));
        buffer_cuboid(buffer, glm::vec3(0.3, 2, 0.3), glm::vec3(2.2, -1.5, 3.2));
        buffer_cuboid(buffer, glm::vec3(0.3, 2, 0.3), glm::vec3(2.2, -1.5, 0));

        // jetty supports [252+72]
        buffer_cuboid(buffer, glm::vec3(0.1, 1.3, 0.1), glm::vec3(-1.9, -0.5, 1.1));
        buffer_cuboid(buffer, glm::vec3(0.1, 1.3, 0.1), glm::vec3(-1.9, -0.5, 2.3));

        // windowsill [324+36]
        buffer_cuboid(buffer, glm::vec3(0.52, 0.1, 0.07), glm::vec3(1.04, 0.89, 2.85));

        const float ROOF_HEIGHT = 2.4;

        // roof [360+16]
        std::vector<float> roof = {
                //front-facing triangle
                0.3, 1.9, 0.65,            0,0,
                0.3, ROOF_HEIGHT, 1.75,    0,1,
                0.3, 1.9, 2.85,            1,1,
                //back-facing triangle
                2.3, 1.9, 0.65,            0,0,
                2.3, ROOF_HEIGHT, 1.75,    0,1,
                2.3, 1.9, 2.85,            1,1,
                //right-side rect
                0.15, 1.741, 3.2,          2,0, // left triangle
                2.45, 1.741, 3.2,          0,0,
                0.15, ROOF_HEIGHT, 1.75,   2,2,
                0.15, ROOF_HEIGHT, 1.75,   2,2, // right triangle
                2.45, ROOF_HEIGHT, 1.75,   0,2,
                2.45, 1.741, 3.2,          0,0,
                //left-side rect
                0.15, ROOF_HEIGHT, 1.75,   2,2, // left triangle
                2.45, ROOF_HEIGHT, 1.75,   0,2,
                2.45, 1.741, 0.3,          0,0,
                0.15, 1.741, 0.3,          2,0, // right triangle
                2.45, 1.741, 0.3,          0,0,
                0.15, ROOF_HEIGHT, 1.75,   2,2,
        };
        buffer.insert(buffer.end(), roof.begin(), roof.end());

        // chimney [376+32]
        buffer_cuboid(buffer, glm::vec3(0.5, 1, 0.5), glm::vec3(1.1, 1.5, 2));

        float data[buffer.size()];
        std::copy(buffer.begin(), buffer.end(), data);

        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void generate_background_model(unsigned int &vao, unsigned int &vbo, unsigned int &ibo) {

        float data[] = {
                -1, -1, 0,  0,0,
                 1,  1, 0,  1,1,
                -1,  1, 0,  0,1,

                -1, -1, 0,  0,0,
                 1, -1, 0,  1,0,
                 1,  1, 0,  1,1,
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        // ==== specify attributes ====
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(0); // vertex positions
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // texture coordinates

    }

}
