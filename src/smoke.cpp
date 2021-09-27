#include "nami/smoke.h"

namespace smoke {

    std::vector<std::array<float, 3>> points;

}

void smoke::update(float offset) {

    float signal[LENGTH];
    points = {};

    for (int i = 0; i < LENGTH; ++i) {
        signal[i] = float(
                sin( (1/WAVELENGTH)*(i*M_PI/2) + (offset*SPEED) ) * AMPLITUDE
        );
    }

    for (int i = 0; i < LENGTH*2; ++i) {
        if (i%2 == 0) {
            points.push_back(
                    std::array<float, 3> {float(i)/2, float(i), 0}
            );
        } else {
            points.push_back(
                    std::array<float, 3> {float(-i)/2, float(i), 0}
            );
        }
    }

}

std::vector<float> smoke::get_vertex_stream() {

    std::vector<float> vertexStream;

    for (auto point : points) {
        vertexStream.insert(end(vertexStream), begin(point), end(point));
    }


    return vertexStream;

}