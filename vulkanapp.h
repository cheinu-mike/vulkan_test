#pragma once

#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"

#include <iostream>
#include <optional>
#include <vector>
#include <set>

struct QueueFamilyIndices {
 	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool isComplete() {
       		return graphicsFamily.has_value();
    	}
};

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
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	GLFWwindow* window;

	void initWindow();
	void initVulkan();
	void createSurface();
	void mainLoop ();
	void cleanup();
	void pickPhysicalDevice();
	void createInstance();
	void createLogicalDevice();
	//bool checkValidationLayerSupport();

	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
};
