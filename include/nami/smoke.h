#ifndef NAMI_SMOKE_H
#define NAMI_SMOKE_H

#include <vector>
#include <array>
#include <cmath>

namespace smoke {

    const int LENGTH = 10;
    const float SPEED = 1.2f;
    const float AMPLITUDE = 0.15f;
    const float WAVELENGTH = 1.2f;

    extern std::vector<std::array<float, 3>> points;

    void update(float offset);
    std::vector<float> get_vertex_stream();

}

#endif // NAMI_SMOKE_H