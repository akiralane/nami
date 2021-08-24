#include "nami/graphics.h"

/**
 * TODO: - bifurcate graphics namespace into 'core' and 'generation'(?)
 *       - tinker with the perspective
 *       - add audio
 *       - add the house
 *       - add skybox (clouds etc)
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::init(window);
    graphics::start_render_loop(window);

}
