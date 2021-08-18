#include "nami/graphics.h"

/**
 * TODO: -write a wave class and figure out how the matrix stuff is going to work
 *       -make the generation code more generic so that different objects can be generated with
 *        the same functions
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::init(window);
    graphics::start_render_loop(window);

}
