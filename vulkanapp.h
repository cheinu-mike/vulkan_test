#pragma once

#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"

#include <iostream>

class vulkanapp {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	const int WIDTH = 800;
	const int HEIGHT = 600;
	
	VkInstance instance;

	GLFWwindow* window;

	void initWindow();
	void pickPhysicalDevice();
	void initVulkan();
	void mainLoop ();
	void cleanup();
	void createInstance();
	bool checkValidationLayerSupport();
};
