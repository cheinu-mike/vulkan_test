#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#include "/usr/include/GLFW/glfw3.h"
#include "/usr/include/GLFW/glfw3native.h"

#include <iostream>
#include <string>
#include <vector>
#include <X11/Xlib.h>


class vulkantest {
public:
	
	bool testresult(VkResult result, std::string fname){
		if (result == VK_SUCCESS){
			std::cout << fname << " successful" << std::endl;
			return true;
		}
		else {
			std::cout << fname << " failed" << std::endl;
			std::cout << fname << " error code: " << result << std::endl;
			return false;
		}
	}

	void run();

private:
	//==============================================
	const int Width = 800;
	const int Height = 600;

	GLFWwindow* window;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkInstance inst;
	VkDevice device;
	uint32_t devicecount = 0;
	VkPhysicalDevice physicaldevice = VK_NULL_HANDLE;
	//VkQueue vkqueue;
	VkCommandPool commandpool = VK_NULL_HANDLE;

	VkQueueFamilyProperties qfamilystruct;

	/*
	try{
		std::cout << "inside try block" << std::endl;
		std::vector<VkCommandBuffer> cmdbuffers = {VK_NULL_HANDLE};
	}
	catch(...){
		std::cout << "VkCommandBuffer vector not initialized" << std::endl;
	}
	*/

	std::vector <VkCommandBuffer> cmdbuffers = {};

	VkSurfaceKHR surface;

	//Xlib Surface Stuff
	//Display *display = XOpenDisplay(0);

	Display* display = glfwGetX11Display();
	Window x11window = glfwGetX11Window(window);
	//===========================================
	
	void initwindow();

	void createinstance();
	void devicestructs();
	void commandbuffers();
	void surfacecreation();
	void swapchaincreation();

	void mainloop();
	void cleanup();
};
