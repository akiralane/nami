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

            vectorStream.push_back((float(row)     * SPACING) + X_OFFSET); // x
            vectorStream.push_back((float(column)  * SPACING) + Y_OFFSET); // y
            vectorStream.push_back(heights[i]);                            // z, which has become height somehow

        }

        return vectorStream;
        
    }


}