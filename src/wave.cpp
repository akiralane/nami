#include "nami/wave.h"

namespace wave {

    float heights[GRID_SIZE * GRID_SIZE] = {
            0.4f, 0.2f, 0.3f, 0.1f,
            0.3f, 0.5f, 0.8f, 0.2f,
            0.7f, 1.0f, 1.0f, 0.6f,
            0.4f, 0.6f, 0.8f, 0.3f
    };

    // (num of strips) * (points per strip) + (number of primitive-return indices)
    int INDEX_COUNT =
            (GRID_SIZE-1) * (GRID_SIZE*2) + (GRID_SIZE-2);

    std::vector<float> get_vector_stream() {

        std::vector<float> vectorStream;

        for (int i = 0; i < GRID_SIZE*GRID_SIZE; ++i) {

            int column = i % GRID_SIZE;
            int row = (i-column)/GRID_SIZE;

            std::vector<float> point = {
                    (float(row)     * SPACING) + X_OFFSET, // x
                    (float(column)  * SPACING) + Y_OFFSET, // y
                    static_cast <float> (rand()) / static_cast <float> (RAND_MAX),                            // z (which has become height somehow) NOLINT(cert-msc50-cpp)
                    0, 1                                   // texture
            };

            vectorStream.insert(end(vectorStream), begin(point), end(point));

        }

        return vectorStream;
        
    }

    void generate_indices(int indices[]) {

        int previous = GRID_SIZE - 1;
        bool isEven = true; // modulo is unreliable because of the restart index

        int stripPosition = 0;
        for (int i = 0; i < INDEX_COUNT; i++) {

            if (stripPosition == GRID_SIZE * 2) {
                indices[i] = PRIMITIVE_RESTART_INDEX;
                stripPosition = 0;
            } else if (isEven) {
                indices[i] = previous - (GRID_SIZE - 1);
                previous = indices[i];
                isEven = !isEven;
                stripPosition++;
            } else {
                indices[i] = previous + GRID_SIZE;
                previous = indices[i];
                isEven = !isEven;
                stripPosition++;
            }

        }

    }


}