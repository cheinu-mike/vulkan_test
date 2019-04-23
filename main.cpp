//g++ -lglfw main.cpp
#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"
#include "/home/cheinu/Documents/vulkanapp/vulkanapp.h"

#include <iostream>

#include <stdexcept>
#include <cstdlib>

int main() {
    vulkanapp app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
