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
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const char* monitorname;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	int * monitorcount;

	VkInstance inst;
	VkDevice device;
	uint32_t devicecount = 0;
	VkPhysicalDevice physicaldevice = VK_NULL_HANDLE;
	VkCommandPool commandpool = VK_NULL_HANDLE;

	//queue variables
	VkQueue vkqueue;
	VkQueueFamilyProperties qfamilystruct;
	uint32_t familypropertycount;
	uint32_t queuecountnum;
	VkDeviceQueueInfo2 queueinfo2;
	const uint32_t qindexcount = 15;
	
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
	VkCommandBuffer cmdbuffer2;

	VkSurfaceKHR surface;

	//Xlib Surface Stuff
	Display *display2 = XOpenDisplay(0);

	Display* display = glfwGetX11Display();
	Window x11window = glfwGetX11Window(window);
	int screencount = 0;
	int screenindex = 0;
	Screen* screen;
	int depth = 0;
	int classvalue = 0;
	Visual *visualtype;
	XVisualInfo vinfo;
	VisualID visualid;

	VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

	//Image variables
	std::vector<VkImage> image = std::vector<VkImage> (1);
	std::vector<VkImageView> imageview = std::vector<VkImageView> (1);
	VkImageViewType imageviewtype = VK_IMAGE_VIEW_TYPE_2D;
	VkComponentMapping componentmapping;
	VkImageSubresourceRange imagesubresourcerange;
	//===========================================
	
	void initwindow();

	void getxlib();

	void createinstance();
	void devicestructs();
	void commandbuffers();
	void surfacecreation();
	void swapchaincreation();
	void imageviewcreation(); 

	void mainloop();
	void cleanup();
};
