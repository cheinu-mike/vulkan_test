#pragma once

#define _GLFW_X11
#define _GLFW_HAS_XINPUT
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#include "/usr/include/GLFW/glfw3.h"
#include "/usr/include/GLFW/glfw3native.h"

#include <iostream>
#include <string>
#include <vector>
#include <X11/Xlib.h>

struct vkvectors {
		
	const int Width = 800;
	const int Height = 400;

	//xlib stuff
	int initcount = 0;	
	int* monitorcount = &initcount;

	GLFWwindow* window;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const char* monitorname;

	PFN_vkCreateInstance pfnCreateInstance = (PFN_vkCreateInstance)
    	glfwGetInstanceProcAddress(NULL, "vkCreateInstance");

	uint32_t glfwExtensionCount = 2; //assuming it's 2 but use vkEnumerateDeviceExtentionProperties
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	//int code = glfwGetError(NULL);
	
	Display* display = glfwGetX11Display();
	Window x11window = glfwGetX11Window(window);

	//device creation
	const char* initextensionname = "VK_KHR_xlib_surface";
	const char* const* extensionnames = &initextensionname; 
	std::vector<VkPhysicalDevice> physicaldevicevec;
	//queue variables;
	std::vector<uint32_t> qfamindex;
	std::vector<VkQueue> vkqueue;

	std::vector<VkExtensionProperties> extensionproperties;
	std::vector<VkCommandBuffer> cmdbuffers;

	//surface
	VkSurfaceCapabilitiesKHR surfacecapabilities;
	uint32_t presentmodecount;
	VkPresentModeKHR presentmode;

	//images
	std::vector<VkImage> image; 
	std::vector<VkImageView> imageview;
};

class vulkantest {
public:
	
	//vkvectors* vkinfo = (vkvectors*)(malloc(sizeof(vkvectors)));
	vkvectors* vkinfo = new vkvectors;

	bool testresult(VkResult result, std::string fname){
		if (result == VK_SUCCESS){
			std::cout << "\t ==="  << fname << " successful ===" << std::endl;
			return true;
		}
		else {
			std::cout << "\t ===" << fname << " failed ===" << std::endl;
			std::cout << "\t ===" << fname << " error code: " << result << " ===" << std::endl;
			return false;
		}
	}

	void run();

private:

	//==============================================
	const int Width = 800;
	const int Height = 600;


	VkInstance inst;
	VkDevice device;
	uint32_t devicecount = 0;
	VkPhysicalDevice physicaldevice = VK_NULL_HANDLE;
	VkCommandPool commandpool = VK_NULL_HANDLE;

	uint32_t propertycount;

	//queue variables
	//VkQueue vkqueue;
	VkQueueFamilyProperties qfamilystruct;
	uint32_t familypropertycount;
	uint32_t queuecountnum;
	VkDeviceQueueInfo2 queueinfo2;
	const uint32_t qindexcount = 15;
	
	VkCommandBuffer cmdbuffer2;

	VkSurfaceKHR surface;

	//Xlib Surface Stuff
	Display *display2 = XOpenDisplay(0);

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
