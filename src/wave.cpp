#include "nami/wave.h"

namespace wave {

    // the number of indices we want to define is given by
    // (num of strips) * (points per strip) + (number of primitive-return indices)
    int INDEX_COUNT =
            (GRID_SIZE-1) * (GRID_SIZE*2) + (GRID_SIZE-2);

    std::vector<float> get_vector_stream() {

        std::vector<float> vectorStream;

        for (int i = 0; i < GRID_SIZE*GRID_SIZE; ++i) {

            int column = i % GRID_SIZE;
            int row = (i-column)/GRID_SIZE;

            std::vector<float> point = {
                    (float(row)     * SPACING) + X_OFFSET,                         // x
                    (float(column)  * SPACING) + Y_OFFSET,                         // y
                    static_cast<float>(rand()) / static_cast<float>(RAND_MAX),     // z (which has become height somehow)
                    0, 1                                                           // texture
            };

            vectorStream.insert(end(vectorStream), begin(point), end(point));

        }

        return vectorStream;
        
    }

    void generate_indices(int indices[]) {

        /**
         * the sequence alternates -(gridsize-1), +gridsize applied to the previous value
         * if our position in the strip:
         *      is twice the grid size:
         *          we have hit the end of the strip
         *          write the restart index and reset our position
         *      is even:
         *          we are on the top row
         *          write previous - (gridsize-1)
         *      is odd:
         *          we are on the bottom row
         *          write previous + gridsize
         */

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