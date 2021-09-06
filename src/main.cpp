#include "nami/graphics.h"

/**
 * TODO: - add audio
 *       - add the house
 *          plus a boat and a jetty - refer to dad's sketch
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::core::init(window);
    graphics::core::start_render_loop(window);

}
