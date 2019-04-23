#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"
#include "/home/cheinu/Documents/vulkanapp/vulkanapp.h"

#include <iostream>

void vulkanapp::initWindow (){
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void vulkanapp::initVulkan (){
	createInstance();
}

void vulkanapp::mainLoop() {
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

}

void vulkanapp::cleanup() {
	glfwDestroyWindow(window);

	glfwTerminate();
}


