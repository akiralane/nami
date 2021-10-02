#include "nami/smoke.h"

std::vector<std::array<float, 3>> smoke::points;

void smoke::update(float offset) {

    points = {}; // clear old points

    // generate signal
    // we make the array one larger because we've offset the right side's wave by adding 1 to its index
    float signal[POINTS_HEIGHT+1];
    for (int i = 0; i < POINTS_HEIGHT+1; ++i) {
        signal[i] = float(
                sin( (1/WAVELENGTH)*(i*M_PI/2) + (offset*SPEED) ) * AMPLITUDE
        );
    }

    // create and push the points
    for (int i = 0; i < POINTS_HEIGHT; ++i) {
        points.push_back(
                std::array<float, 3>{signal[i], float(i), 0}
        );
        points.push_back(
                std::array<float, 3>{signal[i+1] + WIDTH, float(i), 0}
        );

    }

}

std::vector<float> smoke::get_vertex_stream() {

    std::vector<float> vertexStream;

    for (auto point : points) {
        vertexStream.insert(end(vertexStream), begin(point), end(point));
    }

    return vertexStream;

}