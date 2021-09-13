#include "nami/graphics.h"

/**
 * TODO: PROJECT
 *       - add audio
 *       - add the house
 *          plus a boat and a jetty - refer to dad's sketch
 */

/**
 * TODO: CAMERA
 *      make a constructor which can take a specific point to look at
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::core::init(window);
    graphics::core::start_render_loop(window);

}
