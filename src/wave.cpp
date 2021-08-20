#include "nami/wave.h"

namespace wave {

    float heights[GRID_SIZE * GRID_SIZE] = {
            0.4f, 0.2f, 0.3f, 0.1f,
            0.3f, 0.5f, 0.8f, 0.2f,
            0.7f, 1.0f, 1.0f, 0.6f,
            0.4f, 0.6f, 0.8f, 0.3f
    };

    std::vector<float> get_vector_stream() {

        std::vector<float> vectorStream;

        for (int i = 0; i < GRID_SIZE*GRID_SIZE; ++i) {

            int column = i % GRID_SIZE;
            int row = (i-column)/GRID_SIZE;

            std::vector<float> point = {
                    (float(row)     * SPACING) + X_OFFSET, // x
                    (float(column)  * SPACING) + Y_OFFSET, // y
                    heights[i],                            // z (which has become height somehow)
                    0, 1                                   // texture
            };

            vectorStream.insert(end(vectorStream), begin(point), end(point));

        }

        return vectorStream;
        
    }


}