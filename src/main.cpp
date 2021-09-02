#include "nami/graphics.h"

/**
 * TODO: - https://learnopengl.com/Advanced-OpenGL/Cubemaps !! do this one
 *          plus add a separate shader for the skybox - we might want the sea to be reflective
 *       - add skybox (clouds etc)
 *       - add audio
 *       - add the house
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::core::init(window);
    graphics::core::start_render_loop(window);

}
