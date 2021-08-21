#ifndef NAMI_WAVE_H
#define NAMI_WAVE_H

#include <vector>
#include <iterator>

namespace wave {

    // settings

    const float SPACING = 1.0; // distance between each vertex in a row
    static const int GRID_SIZE = 20; // number of rows and columns

    extern int INDEX_COUNT;

    const float X_OFFSET = -0.8f;
    const float Y_OFFSET = -0.8f;

    const int PRIMITIVE_RESTART_INDEX = 99;

    extern float heights[GRID_SIZE*GRID_SIZE];

    /**
     * Generates a series of floats which represent the vertices of the wave mesh, as
     * described by the constants in the namespace.
     * Each vertex is of the form (x,y,z)(t_0,t_1).
     * @return the 'vector stream' which the VBO wants
     */
    std::vector<float> get_vector_stream();

    /**
     * Generates the order in which indexed drawing should take place
     * @param indices the array which will be populated
     */
    void generate_indices(int indices[]);

}

#endif
