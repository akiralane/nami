#ifndef NAMI_SMOKE_H
#define NAMI_SMOKE_H

#include <vector>
#include <array>
#include <cmath>

namespace smoke {

    const int POINTS_HEIGHT = 10; // how many points "tall" the smoke should be
    const float WIDTH = 0.4f; // the horizontal space between the left and right stacks

    const float SPEED = 1.2f;
    const float AMPLITUDE = 0.13f;
    const float WAVELENGTH = 1.2f;

    extern std::vector<std::array<float, 3>> points;

    void update(float offset);
    std::vector<float> get_vertex_stream();

}

#endif // NAMI_SMOKE_H