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

struct vkvariables{
		
	const int Width = 800;
	const int Height = 400;

	VkInstance inst;

	//Validation Layers
	//***?????????
	//PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = VK_NULL_HANDLE;
	//CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	//***?????????

	uint32_t defaultlayercount;
	std::vector<VkLayerProperties> layerproperties;
	std::vector<const char*> validationlayers = {"VK_LAYER_LUNARG_standard_validation"};
	uint32_t layercount = static_cast<uint32_t>(validationlayers.size());
	VkDebugUtilsMessengerEXT messenger;

	//PFN_vkDebugUtilsMessengerCallbackEXT debugCallback = VK_NULL_HANDLE;
	/*(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cout << "Validation Layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
	*/

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
	std::vector<const char*> extensionvec = {"VK_KHR_surface", "VK_KHR_xlib_surface", "VK_KHR_xcb_surface", "VK_EXT_debug_report", "VK_EXT_debug_utils"};
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
	std::vector<uint32_t> qfamindex; 	//vector of graphical bit queue family index
	std::vector<uint32_t> qfamgraphicsbitindex;
	uint32_t usablequeues; 				// = static_cast<uint32_t>(qfamindex.size());
	std::vector<VkQueue> vkqueue;
	std::vector<VkQueueFamilyProperties> qfamilypropertystruct;
	uint32_t familypropertycount;
	VkDeviceQueueInfo2 queueinfo2;

	std::vector<VkCommandBuffer> cmdbuffers;

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

	//image view
	VkImageViewType imageviewtype = VK_IMAGE_VIEW_TYPE_2D;
};

class vulkantest {
public:
	
	//vkvectors* vkinfo = (vkvectors*)(malloc(sizeof(vkvectors)));
	vkvariables* vkinfo = new vkvariables;

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

	//call back function. Improve it later
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugcallback(VkDebugUtilsMessageSeverityFlagBitsEXT 		messageSeverity, 
												VkDebugUtilsMessageTypeFlagsEXT		 			messageType, 
												const VkDebugUtilsMessengerCallbackDataEXT* 	pCallbackData, 
												void* 											pUserData)
	{
		std::cout << "Validation Layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	void run();

private:

	//==============================================

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

	//===========================================
	
	void initwindow();

	void getxlib();

	void createinstance();
	void validationdebug();
	void devicestructs();
	void commandbuffers();
	void surfacecreation();
	void swapchaincreation();
	void imagebuffercreation();
	void imagecreation();
	void imageviewcreation(); 

	void mainloop();
	void cleanup();
};
