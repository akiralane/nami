#include "nami/Graphics.h"
#include <cstdio>

int main() {

    printf("wahey");

    static GLFWwindow* window = nullptr;
    Graphics::init(window);
    Graphics::start_render_loop(window);

}
