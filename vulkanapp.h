#pragma once

#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"

#include <iostream>

void createInstance(){
	VkApplicationInfo appInfo ={};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1,1,0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1,1,0);
	appInfo.apiVersion = VK_API_VERSION_1_1;
}

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
	void initVulkan();
	void mainLoop ();
	void cleanup();

};
