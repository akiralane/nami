#ifndef NAMI_WAVE_H
#define NAMI_WAVE_H

#include <vector>
#include <iterator>

/**
 * so what I'm thinking is to have a member variable 'points' which is a matrix of 3d vectors \
 * this gets initialised properly in the constructor
 * there's then a 'propagate' function which moves all the points accordingly
 * and then a getter which flattens the matrix into the triangles which opengl wants
 *
 * first things first - hardcode the points to make sure it works like you think it does
 */

namespace wave {

    const float SPACING = 1.0; // distance between each vertex in a row
    static const int GRID_SIZE = 4; // number of rows and columns

    const float X_OFFSET = -0.8f;
    const float Y_OFFSET = -0.8f;

    extern float heights[GRID_SIZE*GRID_SIZE];
    extern int indices[];

    std::vector<float> get_vector_stream();

};

#endif //NAMI_WAVE_H
