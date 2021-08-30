#ifndef NAMI_WAVE_H
#define NAMI_WAVE_H

#include <vector>
#include <iterator>
#include <cmath>
#include <array>

namespace wave {

    // ==== settings ====

    const float SPACING = 1.0; // distance between each vertex in a row
    static const int GRID_SIZE = 12; // number of rows and columns

    const float WAVELENGTH = 1.2;
    const float AMPLITUDE = 0.3;
    const float SPEED = 1.2;

    const int PRIMITIVE_RESTART_INDEX = -1; // the index which will cause OGL to begin drawing a new primitive
    extern const int INDEX_COUNT; // the number of indices with which the grid is drawn -  a function of the GRID_SIZE

    // ==== object declarations ====

    extern std::array<std::array<float, GRID_SIZE>, GRID_SIZE> heights;

    // ==== functions ====

    /**
     * Updates the heights 'matrix' according to the trig offset given.
     * If offset is increasing in small enough increments, the waves appear to move.
     * @param offset The value to offset Theta by, shifting sin's values
     */
    void update_heights(float offset);

    /**
     * Generates a series of floats which represent the vertices of the wave mesh, as
     * described by the settings constants.
     * Each vertex is of the form (x,y,z)(t_0,t_1).
     * @return the 'vector stream' which the VBO wants
     */
    std::vector<float> get_vector_stream();

    /**
     * Generates the order in which indexed drawing should take place.
     * @param indices the array which will be populated with the indices
     */
    void generate_indices(int indices[]);

}

#endif // NAMI_WAVE_H
