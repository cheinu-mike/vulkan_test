#pragma once

#define _GLFW_X11
#define _GLFW_HAS_XINPUT
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
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

	Display* glfwdisplay = glfwGetX11Display();
	Window glfwx11window = glfwGetX11Window(window);
	VkDisplayKHR display;

	//device creation
	uint32_t glfwExtensionCount = 2; //assuming it's 2 but use vkEnumerateDeviceExtentionProperties
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	//int code = glfwGetError(NULL);
	
	std::vector<VkExtensionProperties> extensionproperties;
	//instance extension not device extension
	std::vector<const char*> extensionvec = {"VK_KHR_surface", "VK_KHR_xlib_surface", "VK_KHR_xcb_surface"};
	uint32_t extensioncount = static_cast<uint32_t>(extensionvec.size());
	const char* initextensionname = "VK_KHR_xlib_surface";
	const char* const* extensionnames = &initextensionname; 

	std::vector<VkPhysicalDevice> physicaldevicevec;

	//device extensions
	uint32_t deviceextensioncount;
	std::vector<VkExtensionProperties> deviceextensionproperties;
	std::vector<const char*> deviceext = {"VK_KHR_swapchain"};
	uint32_t deviceextcount = static_cast<uint32_t>(deviceext.size());

	//queue variables;
	std::vector<uint32_t> qfamindex; //vector of graphical bit queue family index
	uint32_t usablequeues; // = static_cast<uint32_t>(qfamindex.size());
	std::vector<VkQueue> vkqueue;

	std::vector<VkCommandBuffer> cmdbuffers;

	//VkQueue vkqueue;
	VkQueueFamilyProperties qfamilystruct;
	uint32_t familypropertycount;
	VkDeviceQueueInfo2 queueinfo2;

	//surface
	VkSurfaceKHR surface;
	VkSurfaceKHR surface2;
	VkSurfaceCapabilitiesKHR surfacecapabilities;

	uint32_t surfaceformatcount;
	std::vector<VkSurfaceFormatKHR> surfaceformats;

	//swapchaincreation()
	VkSwapchainKHR swapchain;
	uint32_t presentmodecount;
	std::vector<VkPresentModeKHR> presentmode;
	
	//imagebuffer
	VkMemoryRequirements memrequirements;

	//images
	uint32_t imagecount;
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

	VkInstance inst;
	VkDevice device;
	uint32_t devicecount = 0;
	VkPhysicalDevice physicaldevice = VK_NULL_HANDLE;
	VkCommandPool commandpool = VK_NULL_HANDLE;

	uint32_t propertycount;

	VkCommandBuffer cmdbuffer2;

	//Xlib Surface Stuff
	Display *xdisplay = XOpenDisplay(0);

	int screencount = 0;
	int screenindex = 0;
	Screen* screen;
	int depth = 0;
	int classvalue = 0;
	Visual *visualtype;
	XVisualInfo vinfo;
	VisualID visualid;

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
	void imagebuffercreation();
	void imageviewcreation(); 

	void mainloop();
	void cleanup();
};
