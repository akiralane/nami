#ifndef NAMI_WAVE_H
#define NAMI_WAVE_H

#include <vector>
#include <iterator>
#include <cmath>
#include <array>

namespace wave {

    // ==== settings ====

    const float SPACING = 1.0; // distance between each vertex in a row
    static const int GRID_SIZE = 14; // number of rows and columns

    extern std::array<std::array<float, GRID_SIZE>, GRID_SIZE> heights;

    // these offsets translate the grid's vertices in world space
    // should probably delete these and translate the camera instead TODO
    const float X_OFFSET = -5;
    const float Y_OFFSET = -5;

    extern int INDEX_COUNT; // the number of indices with which the grid is drawn -  a function of the GRID_SIZE
    const int PRIMITIVE_RESTART_INDEX = 99; // the index which will cause OGL to begin drawing a new primitive

    // ==== functions ====

    /**
     * Updates the heights float 'matrix' with data according to the trig offset given.
     * If offset is increasing in small enough increments, the waves appear to move.
     * @param offset The value to offset Theta by, shifting sin's values
     */
    void update_heights(float offset);

    /**
     * Generates a series of floats which represent the vertices of the wave mesh, as
     * described by the settings constants in this namespace.
     * Each vertex is of the form (x,y,z)(t_0,t_1).
     * @return the 'vector stream' which the VBO wants
     */
    std::vector<float> get_vector_stream();

    /**
     * Generates the order in which indexed drawing should take place
     * @param indices the array which will be populated with the indices
     */
    void generate_indices(int indices[]);

}

#endif // NAMI_WAVE_H
