#include "nami/Graphics.h"

int main() {

    GLFWwindow* window = nullptr;
    Graphics::init(window);
    Graphics::start_render_loop(window);

}

