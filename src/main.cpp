#include "nami/graphics.h"

/**
 * TODO | ADD:
 *  - audio
 *  - smoke from chimney
 *  - fade-in intro
 *  - add some flowers...?
 */

/**
 * TODO | FIX:
 *  - smoke implementation
 */

/**
 * ==== CONTROLS ====
 * WASD - lateral movement
 * SPACE/LSHIFT - up/down
 * Q - print camera location info
 * E - toggle wireframe drawing
 * TAB - release cursor
 */

int main() {

    GLFWwindow* window = nullptr;
    graphics::core::init(window);
    graphics::core::start_render_loop(window);

}
