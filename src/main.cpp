#include "nami/graphics.h"

/**
 * TODO: - tinker with the perspective
 *       - add audio
 *       - add the house
 *       - add skybox (clouds etc)
 *       - shift all dependencies into a deps folder??
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::core::init(window);
    graphics::core::start_render_loop(window);

}
