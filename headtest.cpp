#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
#include "/usr/include/glm/glm.hpp"
#include "/usr/include/GLFW/glfw3.h"
//#include "/usr/include/GLFW/glfw3native.h"

#include "headtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void vulkantest::run(){

	initwindow();

	getxlib();

	//validationdebug();
	createinstance();
	devicestructs();
	commandbuffers();
	surfacecreation();
	swapchaincreation();
	imagecreation();
	imageviewcreation();
	uniformbuffercreation();
	creategraphicspipeline();
	createdescriptorset();

	mainloop();
	cleanup();
}

void vulkantest::initwindow(){

	if (vkinfo ==  NULL){
			std::cout << "struct not allocated" << std::endl;
	}
	else{
			std::cout << "allocation succesful" << std::endl;
	}

	if (!glfwInit()){
			std::cout << "failed to initialize GLFW" << std::endl;
	}
	else{
			std::cout << "initialization of GLFW successful" << std::endl;
	}

	if (glfwVulkanSupported()){
			std::cout << "Vulkan is supported" << std::endl;
	}
	else{
			std::cout << "Vulkan is not supported" << std::endl;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	vkinfo->window = glfwCreateWindow(vkinfo->Width, vkinfo->Height, "Vulkan", nullptr, nullptr);

	std::cout << "window width " << vkinfo->Width << std::endl;
	std::cout << "window height " << vkinfo->Height << std::endl;
	std::cout << "\n";
}

void vulkantest::getxlib(){

	glfwGetMonitors(vkinfo->monitorcount);
	std::cout << "glfw monitor count: " << *vkinfo->monitorcount << std::endl;
	//monitorname = glfwGetMonitorName(monitor);
	//std::cout << "monitor name: " << *monitorname << std::endl;

	std::cout << "glfw display: " << vkinfo->glfwdisplay << std::endl;
	std::cout << "X11 display: " << xdisplay << std::endl;
	screencount = XScreenCount(xdisplay);
	std::cout << "X screen count: " << screencount << std::endl;
	screen = XDefaultScreenOfDisplay(xdisplay);
	screenindex = XDefaultScreen(xdisplay);
	std::cout << "X Screen Index: " << screenindex << std::endl;
	depth = XDefaultDepthOfScreen(screen);
	std::cout << "X depth: " << depth << std::endl;
	visualtype = XDefaultVisual(xdisplay, screenindex);
	std::cout << "X visual: " << visualtype << std::endl;
	XMatchVisualInfo(xdisplay, screenindex, depth, TrueColor, &vinfo);
	visualid = vinfo.visualid;
	Visual *vis = vinfo.visual;
	std::cout << "vinfo.visual: " << &vis << std::endl;
	std::cout << "vinfo.visualid: " << vinfo.visualid << std::endl;
	std::cout << "vinfo.screen: " << vinfo.screen << std::endl;
	//std::cout << "vinfo.class: " << vinfo.class << std::endl;
	std::cout << "vinfo.depth: " << vinfo.depth << std::endl;

	std::cout << "\n";
}

/*
void vulkantest::validationdebug(){

	VkDebugUtilsMessengerCreateInfoEXT debuginfo = {};
	debuginfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debuginfo.flags = 0;
	debuginfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debuginfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debuginfo.pfnUserCallback = debugcallback;
	//debuginfo.pUserData = ;

	//seg fault here and I don't know why
	auto vkcreatedebugutilsmessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkinfo->inst, "vkCreateDebugUtilsMessengerEXT");
	VkResult debugresult = vkcreatedebugutilsmessenger(vkinfo->inst, &debuginfo, NULL, &vkinfo->debugmessenger); 
	std::cout << "segfault" << std::endl;

	testresult(debugresult, "validation layer creation");
}
*/

void vulkantest::populatedebugmessenger(VkDebugUtilsMessengerCreateInfoEXT* createinfo){

	createinfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createinfo->flags = 0;
	createinfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createinfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createinfo->pfnUserCallback = debugcallback;
	//createinfo->pUserData = ;
	
}

void vulkantest::createinstance(){

	//Validation Section. Will clean up soon
	//=======================================================
	VkDebugUtilsMessengerCreateInfoEXT debuginfo = {};
	populatedebugmessenger(&debuginfo);
	//=======================================================

	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pNext = NULL;
	appinfo.pApplicationName = "testing app";
	appinfo.applicationVersion = VK_MAKE_VERSION(1,1,0);
	appinfo.pEngineName = "testing engine";
	appinfo.engineVersion = VK_MAKE_VERSION(1,1,0);
	appinfo.apiVersion = VK_API_VERSION_1_1;


	vkEnumerateInstanceExtensionProperties(NULL, &propertycount, NULL);
	std::cout << "Property Count: " << propertycount << std::endl;

	vkinfo->extensionproperties.resize(propertycount);
	std::cout <<  vkinfo->extensionproperties.size() << std::endl;

	vkEnumerateInstanceExtensionProperties(NULL, &propertycount, vkinfo->extensionproperties.data());

	//Prints Instance extensions
	for (uint32_t i = 0; i < propertycount; i++){

		std::cout << vkinfo->extensionproperties[i].extensionName << std::endl;
	}
	
	vkEnumerateInstanceLayerProperties(&vkinfo->defaultlayercount, NULL); 
	vkinfo->layerproperties.resize(vkinfo->defaultlayercount);
	vkEnumerateInstanceLayerProperties(&vkinfo->defaultlayercount, vkinfo->layerproperties.data());

	for (uint32_t i = 0; i < vkinfo->defaultlayercount; i++){
		std::cout << "\t" << vkinfo->layerproperties[i].layerName << std::endl;
		std::cout << vkinfo->layerproperties[i].description << std::endl;
	}
	
	VkInstanceCreateInfo instanceCI = {};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pNext = NULL;
	//instanceCI.flags
	instanceCI.pApplicationInfo = &appinfo;
	instanceCI.enabledLayerCount = vkinfo->layercount;
	instanceCI.ppEnabledLayerNames = vkinfo->validationlayers.data();

	std::cout << "enabled glfw extension count: "<< vkinfo->glfwExtensionCount << std::endl;

	if (false){
		instanceCI.enabledExtensionCount = vkinfo->glfwExtensionCount;
		instanceCI.ppEnabledExtensionNames = vkinfo->glfwExtensions;
	}
	else {
		instanceCI.enabledExtensionCount = vkinfo->extensioncount;
		instanceCI.ppEnabledExtensionNames = vkinfo->extensionvec.data();
	}

	VkResult resultinstance = vkCreateInstance(&instanceCI, NULL, &vkinfo->inst);

	std::cout << "Enabled extention count: " << instanceCI.enabledExtensionCount << std::endl;

	testresult(resultinstance, "instance creation");

	//Validation section. Will clean up soon
	//=======================================================
	auto vkcreatedebugutilsmessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkinfo->inst, "vkCreateDebugUtilsMessengerEXT");
	VkResult debugresult = vkcreatedebugutilsmessenger(vkinfo->inst, &debuginfo, NULL, &vkinfo->debugmessenger); 
	//=======================================================
	
	testresult(debugresult, "validation layer creation");

	std::cout << "\n";
}

void vulkantest::devicestructs(){

	VkResult holder = vkEnumeratePhysicalDevices(vkinfo->inst, &devicecount, nullptr); //puts a number into devicecount

	if (holder == VK_SUCCESS) {
		std::cout << "Physical Device Enumeration is a success" << std::endl;
		std::cout << "Number of devices: " << devicecount << std::endl;
	}
	else if( devicecount == 0){
		std::cout << "No device detected" << std::endl;
	}
	else{
		std::cout << "Physical Device Enumeration failed" << std::endl;
		std::cout << "physical device error code: " << holder << std::endl;
	}


	VkResult holder2 = vkEnumeratePhysicalDevices(vkinfo->inst, &devicecount, &physicaldevice);

	if (holder2 == VK_SUCCESS) {
		std::cout << "Physical Device Enumeration2 is a success" << std::endl;
		std::cout << "Number of devices: " << devicecount << std::endl;
	}
	else if( devicecount == 0){
		std::cout << "No device detected" << std::endl;
	}
	else{
		std::cout << "Physical Device Enumeration failed" << std::endl;
		std::cout << "physical device error code: " << holder << std::endl;
	}

	vkEnumerateDeviceExtensionProperties(physicaldevice, NULL, &vkinfo->deviceextensioncount, NULL);
   	vkinfo->deviceextensionproperties.resize(vkinfo->deviceextensioncount);

	vkEnumerateDeviceExtensionProperties(physicaldevice, NULL, &vkinfo->deviceextensioncount, vkinfo->deviceextensionproperties.data());

	std::cout << "device extension count: " << vkinfo->deviceextensioncount << std::endl;

	//Prints extension names for device
	/*
	for (uint32_t i; i < vkinfo->deviceextensioncount; i++){
		std::cout << vkinfo->deviceextensionproperties[i].extensionName << std::endl;
	}
	*/

	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &vkinfo->familypropertycount, nullptr);
	std::cout << "queue familypropertycount: " << vkinfo->familypropertycount << std::endl;
	vkinfo->qfamilypropertystruct.resize(vkinfo->familypropertycount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &vkinfo->familypropertycount, vkinfo->qfamilypropertystruct.data());
	std::cout << "queue family property count: " << vkinfo->familypropertycount << std::endl;
	std::cout << "queue family struct queueflags: " << vkinfo->qfamilypropertystruct[0].queueFlags << std::endl;
	std::cout << "queue family struct queuecount: " << vkinfo->qfamilypropertystruct[0].queueCount << std::endl;

	for (uint32_t i = 0; i < vkinfo->familypropertycount; i++){

		if (vkinfo->qfamilypropertystruct[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
			std::cout << "graphics bit success" << std::endl;
		}
		else if (vkinfo->qfamilypropertystruct[i].queueFlags & VK_QUEUE_TRANSFER_BIT){
			std::cout << "Queue family " << i << " is a transfer bit" << std::endl;
		}
		else {
			std::cout << "what are you doing?" << std::endl;
		}

	}
	//float qpriorities[2] = {0.5, 0.6};
	std::vector<VkDeviceQueueCreateInfo> qcreateinfo = {};

	for (uint32_t i=0; i < vkinfo->familypropertycount; i++){
		  vkinfo->glfwdisplay = glfwGetX11Display();
			if (vkGetPhysicalDeviceXlibPresentationSupportKHR(physicaldevice, i, vkinfo->glfwdisplay, visualid) == VK_TRUE){
					std::cout << "queue family index: " << i << " supports Xlib" << std::endl;
					vkinfo->qfamindex.push_back(i);
			}
			else{
					std::cout << "queue family index: " << i << " does not support Xlib" << std::endl;
			}
	}

	vkinfo->usablequeues = static_cast<uint32_t>(vkinfo->qfamindex.size());
	std::cout << vkinfo->usablequeues << std::endl;
	qcreateinfo.resize(vkinfo->usablequeues);

	for (uint32_t i = 0; i < vkinfo->usablequeues; i++){

					qcreateinfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					qcreateinfo[i].pNext = NULL;
					qcreateinfo[i].flags = 0;
					qcreateinfo[i].queueFamilyIndex = i;
					qcreateinfo[i].queueCount = vkinfo->usablequeues;
					qcreateinfo[i].pQueuePriorities = vkinfo->queuepriorities.data(); 
	}
	/*
	for (std::size_t i = 0; vkinfo->qfamindex.size(); i++){
		vkGetDeviceQueue(device, vkinfo->qfamindex[i], 0, vkinfo->vkqueue.data());
		//vkGetDeviceQueue(device, 0, 0, vkinfo->vkqueue.data());
	}
	*/

	VkDeviceCreateInfo deviceinfo = {};
	deviceinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceinfo.pNext = NULL;
	deviceinfo.queueCreateInfoCount = vkinfo->usablequeues;
	deviceinfo.pQueueCreateInfos = qcreateinfo.data();
	deviceinfo.enabledExtensionCount = vkinfo->deviceextcount;
	deviceinfo.ppEnabledExtensionNames = vkinfo->deviceext.data();
	deviceinfo.enabledLayerCount = 0;
	deviceinfo.ppEnabledLayerNames = NULL;
	deviceinfo.pEnabledFeatures = NULL;

	VkResult resultdevice = vkCreateDevice(physicaldevice, &deviceinfo, nullptr, &device);

	testresult(resultdevice, "vk device creation");

	//VkResult result = vkAcquireXlibDisplayEXT(physicaldevice, &vkinfo->glfwdisplay, vkinfo->display);
	//testresult(result, "acquire Xlib display creation");

	std::cout << "\n";
}

void vulkantest::commandbuffers(){

	std::vector<VkCommandPoolCreateInfo> cmdpoolinfo = {};
	cmdpoolinfo.resize(vkinfo->usablequeues);

	for (uint32_t i = 0; i < vkinfo->usablequeues; i++){

		cmdpoolinfo[i].sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdpoolinfo[i].pNext = NULL;
		cmdpoolinfo[i].queueFamilyIndex = vkinfo->qfamindex[i];
		cmdpoolinfo[i].flags = 0;
	}

	VkResult commandpoolresult = vkCreateCommandPool(device, cmdpoolinfo.data(), nullptr, &commandpool);

	testresult(commandpoolresult,"vk command pool creation");

	//cmdbuffers.resize(15);
	VkCommandBufferAllocateInfo cmd = {};
	cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd.pNext = NULL;
	cmd.commandPool = commandpool;
	cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd.commandBufferCount = 2; //(uint32_t) cmdbuffers.size();

	std::cout << "cmd.commandbuffercount " << cmd.commandBufferCount << std::endl;
	std::cout << "size of cmdbuffer vector" << vkinfo->cmdbuffers.size() << std::endl;

	/*
	VkCommandBufferBeginInfo begininfo = {};
	begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begininfo.pNext = NULL;
	begininfo.flags = 0;
	*/

	//VkResult begincmdbuff = vkBeginCommandBuffer(cmdbuffers[0], &begininfo);

	//testresult(begincmdbuff, "vk begin command buffer thing");

	VkResult cmdallocate = vkAllocateCommandBuffers(device, &cmd, &cmdbuffer2);

	//VkResult cmdallocate = vkAllocateCommandBuffers(device, &cmd, nullptr);

	testresult(cmdallocate, "vk command allocation");

	std::cout << "\n";
}

void vulkantest::surfacecreation(){

	VkXlibSurfaceCreateInfoKHR surfaceinfo = {};
	surfaceinfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceinfo.pNext = NULL;
	surfaceinfo.flags = 0;
	surfaceinfo.dpy = vkinfo->glfwdisplay;
	surfaceinfo.window = vkinfo->glfwx11window;

	/*
	VkXcbSurfaceCreateInfoKHR xcbsurfaceinfo = {};
	xcbsurfaceinfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	xcbsurfaceinfo.pNext = NULL;
	xcbsurfaceinfo.flags = 0;
	//xcbsurfaceinfo.connection = ;
	//xcbsurfaceinfo.window = ;
	*/

	VkResult surfaceresult = vkCreateXlibSurfaceKHR(vkinfo->inst, &surfaceinfo, nullptr, &vkinfo->surface);
	VkResult surfaceresult2 = glfwCreateWindowSurface(vkinfo->inst, vkinfo->window, nullptr, &vkinfo->surface2);

	testresult(surfaceresult, "x11 surface creation");
	testresult(surfaceresult2, "glfw surface creation");

	std::cout << "\n";
}

void vulkantest::swapchaincreation(){

	//surface capabilities
	VkResult surcapres = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicaldevice, vkinfo->surface2, &vkinfo->surfacecapabilities);
	testresult(surcapres, "surface capabilities creation");

	//present mode
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicaldevice, vkinfo->surface2, &vkinfo->presentmodecount, nullptr);
	std::cout << "number of present mode count: " << vkinfo->presentmodecount << std::endl;

	vkinfo->presentmode.resize(vkinfo->presentmodecount);
	VkResult surpresres = vkGetPhysicalDeviceSurfacePresentModesKHR(physicaldevice, vkinfo->surface2, &vkinfo->presentmodecount, vkinfo->presentmode.data());
	testresult(surpresres, "surface present mode creation");

	//Surface Support
	VkResult sursupport = vkGetPhysicalDeviceSurfaceSupportKHR(physicaldevice, 0, vkinfo->surface2, &vkinfo->surfacesupport);
	testresult(sursupport, "surface support creation");

	//surface formats
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicaldevice, vkinfo->surface2, &vkinfo->surfaceformatcount, NULL);
	vkinfo->surfaceformats.resize(vkinfo->surfaceformatcount);
	VkResult surformatres = vkGetPhysicalDeviceSurfaceFormatsKHR(physicaldevice, vkinfo->surface2, &vkinfo->surfaceformatcount, vkinfo->surfaceformats.data());
	std::cout << "Number of formats: " << vkinfo->surfaceformatcount << std::endl;

	for (uint32_t i=0; i < vkinfo->surfaceformatcount; i++){
			std::cout << "Supported Format: " << vkinfo->surfaceformats[i].format << std::endl;
			std::cout << "Supported Color Space: " << vkinfo->surfaceformats[i].colorSpace << std::endl;
	}

	testresult(surformatres, "Device surface format creation");

	std::cout << "surface min imagecount " << vkinfo->surfacecapabilities.minImageCount << std::endl;
	std::cout << "surface max imagecount " << vkinfo->surfacecapabilities.maxImageCount << std::endl;
	std::cout << "Present mode " << vkinfo->presentmode[0] << std::endl;
	std::cout << "Present mode " << vkinfo->presentmode[1] << std::endl;
	std::cout << "Present mode " << vkinfo->presentmode[2] << std::endl;
	std::cout << "surface formats " << vkinfo->surfaceformats[0].format << std::endl;
	//std::cout << "image extent count " << vkinfo->surfacecapabilities.currentExtent << std::endl;
	std::cout << "image array layers (must be greater than 0) " << vkinfo->surfacecapabilities.maxImageArrayLayers << std::endl;

	std::cout << "surface usage flag " << vkinfo->surfacecapabilities.supportedUsageFlags << std::endl;
	VkSwapchainCreateInfoKHR swapcreate = {};
	swapcreate.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapcreate.pNext = NULL;
	swapcreate.surface = vkinfo->surface2;
	swapcreate.minImageCount = vkinfo->surfacecapabilities.minImageCount +1;
	swapcreate.imageFormat = vkinfo->surfaceformats[1].format;
	swapcreate.imageColorSpace = vkinfo->surfaceformats[1].colorSpace;
	swapcreate.imageExtent= vkinfo->surfacecapabilities.currentExtent;
	swapcreate.imageArrayLayers = vkinfo->surfacecapabilities.maxImageArrayLayers;
	swapcreate.imageUsage = vkinfo->surfacecapabilities.supportedUsageFlags;
	swapcreate.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; //VK_SHARING_MODE_CONCURRENT;
	swapcreate.queueFamilyIndexCount = 0; //fix later must be graphics and present bit queue. set to zero if it is already initialized?
	swapcreate.pQueueFamilyIndices = NULL; //must be graphics bit queue
	swapcreate.preTransform = vkinfo->surfacecapabilities.currentTransform;
	swapcreate.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapcreate.presentMode = vkinfo->presentmode[0];
	swapcreate.clipped = VK_TRUE;
	swapcreate.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(device, &swapcreate, NULL, &vkinfo->swapchain);

	testresult(result, "swapchain creation");

	vkGetSwapchainImagesKHR(device, vkinfo->swapchain, &vkinfo->imagecount, NULL); //get another one
	std::cout << "Number of swapchain images: " << vkinfo->imagecount << std::endl;
	vkinfo->image.resize(vkinfo->imagecount);
	vkinfo->imageview.resize(vkinfo->imagecount);
	std::cout << "\n";

}

void vulkantest::imagecreation(){
	VkImageCreateInfo imagecreateinfo = {};
	imagecreateinfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imagecreateinfo.pNext = NULL;
	imagecreateinfo.imageType = VK_IMAGE_TYPE_2D;
	imagecreateinfo.format = vkinfo->surfaceformats[0].format;
	imagecreateinfo.extent.width = vkinfo->Width;
	imagecreateinfo.extent.height = vkinfo->Height;
	imagecreateinfo.extent.depth = 1;
	imagecreateinfo.mipLevels = 1;
	imagecreateinfo.arrayLayers = 1;
	imagecreateinfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imagecreateinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imagecreateinfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	imagecreateinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imagecreateinfo.queueFamilyIndexCount = 0;
	imagecreateinfo.pQueueFamilyIndices = NULL;
	imagecreateinfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED ;

	VkResult rescreateimage = vkCreateImage(device, &imagecreateinfo, NULL, &vkinfo->image[0]); 
	testresult(rescreateimage, "image creation");

	vkGetImageMemoryRequirements(device, vkinfo->image[0], &vkinfo->memrequirements); 
	vkGetPhysicalDeviceMemoryProperties(physicaldevice, &vkinfo->physicalmemprop);
	std::cout << "VkPhysicalDeviceMemoryProperties memorytypecount: " << vkinfo->physicalmemprop.memoryTypeCount << std::endl;
	std::cout << "VkMemoryType struct size: " << vkinfo->physicalmemprop.memoryTypes[0].propertyFlags << std::endl;

	for(uint32_t i = 0; i < vkinfo->physicalmemprop.memoryTypeCount; ++i){
		if(vkinfo->physicalmemprop.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT){
				vkinfo->localbitmemoryindex.push_back(i);
				std::cout << "Local memory index is: " << i << std::endl;
		}	
		else if(vkinfo->physicalmemprop.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT){
				vkinfo->visiblebitmemoryindex.push_back(i);
				std::cout << "Visible memory index is: " << i << std::endl;
		}
	}
	//uint32_t memindex = getmemoryindex(vkinfo->physicalmemprop.memoryTypeCount, vkinfo->physicalmemprop.memoryTypes);
	//std::cout << "optimum memory index is: " << memindex << std::endl;

	VkMemoryAllocateInfo memallocinfo = {};
	memallocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memallocinfo.pNext = NULL;
	memallocinfo.allocationSize = vkinfo->memrequirements.size;
	memallocinfo.memoryTypeIndex = vkinfo->localbitmemoryindex[0]; //create function that will output proper memtype index. Read "device Memory" section in vulkan docs.

	VkResult resallocate = vkAllocateMemory(device, &memallocinfo, NULL, &vkinfo->devicememory); 
	testresult(resallocate, "Allocate memory creation");

	VkResult resbindmemory = vkBindImageMemory(device, vkinfo->image[0], vkinfo->devicememory, 0);
	testresult(resbindmemory, "binding memory");
}

void vulkantest::imageviewcreation(){

	VkImageViewCreateInfo imageviewinfo = {};
	imageviewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageviewinfo.pNext = NULL;
	imageviewinfo.flags = 0;
	imageviewinfo.image = vkinfo->image[0];
	imageviewinfo.viewType = vkinfo->imageviewtype;
	imageviewinfo.format = vkinfo->surfaceformats[0].format;
	imageviewinfo.components.r = VK_COMPONENT_SWIZZLE_R;
	imageviewinfo.components.g = VK_COMPONENT_SWIZZLE_G;
	imageviewinfo.components.b = VK_COMPONENT_SWIZZLE_B;
	imageviewinfo.components.a = VK_COMPONENT_SWIZZLE_A;
	imageviewinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageviewinfo.subresourceRange.baseMipLevel = 0;
	imageviewinfo.subresourceRange.levelCount = 1;
	imageviewinfo.subresourceRange.baseArrayLayer = 0;
	imageviewinfo.subresourceRange.layerCount = 1;

	VkResult imageviewcreate = vkCreateImageView(device, &imageviewinfo, nullptr, &vkinfo->imageview[0]);

	testresult(imageviewcreate, "vk image view creation");

	std::cout << "\n";
}

void vulkantest::uniformbuffercreation(){

	VkBufferCreateInfo bufferinfo = {};
	bufferinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferinfo.pNext = NULL;
	bufferinfo.flags = 0;
	bufferinfo.size = sizeof(glminfo->MVP); 
	bufferinfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferinfo.queueFamilyIndexCount = 0;
	bufferinfo.pQueueFamilyIndices = NULL;

	VkResult resbuffercreate = vkCreateBuffer(device, &bufferinfo, NULL, &vkinfo->buffer); 
	testresult(resbuffercreate, "Uniform Buffer creation");

	vkGetBufferMemoryRequirements(device, vkinfo->buffer, &vkinfo->unimemrequirements);

	VkMemoryAllocateInfo uniallocinfo = {};
	uniallocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	uniallocinfo.pNext = NULL;
	uniallocinfo.allocationSize = vkinfo->unimemrequirements.size;	
	uniallocinfo.memoryTypeIndex = vkinfo->visiblebitmemoryindex[0]; // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT. Read more about it in Device Memory section of Vulkan Documentation

	VkResult resuniallocate = vkAllocateMemory(device, &uniallocinfo, NULL, &vkinfo->devicememory);
	testresult(resuniallocate, "Uniform Buffer Memory Allocation");

	VkResult resmapmemory = vkMapMemory(device, vkinfo->devicememory, 0, vkinfo->unimemrequirements.size, 0, &vkinfo->pData);  
	testresult(resmapmemory, "Uniform Buffer Mapping");

	memcpy(vkinfo->pData, &glminfo->MVP, sizeof(glminfo->MVP));	

	vkUnmapMemory(device, vkinfo->devicememory);

	VkResult resbindunibuffer = vkBindBufferMemory(device, vkinfo->buffer, vkinfo->devicememory, 0);
	testresult(resbindunibuffer, "Binding Uniform Buffer");

	std::cout << "\n";
}

void vulkantest::creategraphicspipeline(){
	VkDescriptorSetLayoutBinding layout_binding = {};
	layout_binding.binding = 0;
	layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layout_binding.descriptorCount = 1;
	layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layout_binding.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
	descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptor_layout.pNext = NULL;
	descriptor_layout.bindingCount = 1;
	descriptor_layout.pBindings = &layout_binding;

	VkResult resdescriptorlayout = vkCreateDescriptorSetLayout(device, &descriptor_layout, NULL, &vkinfo->descsetlayout); 
	testresult(resdescriptorlayout, "Descriptor Layout Creation");

	VkPipelineLayoutCreateInfo pipelayoutinfo = {};
	pipelayoutinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelayoutinfo.pNext = NULL;
	pipelayoutinfo.pushConstantRangeCount = 0;
	pipelayoutinfo.pPushConstantRanges = NULL;
	pipelayoutinfo.setLayoutCount = 1; //fix later to get inputs
	pipelayoutinfo.pSetLayouts = &vkinfo->descsetlayout; //.data() possibly needs to be initialized as a vector

	VkResult respipelayout = vkCreatePipelineLayout(device, &pipelayoutinfo, NULL, &vkinfo->pipelinelayout); 
	testresult(respipelayout, "Pipeline Layout Creation");

	std::cout << "\n";
}

void vulkantest::createdescriptorset(){
	VkDescriptorPoolSize type_count[1];
	type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	type_count[0].descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolcreateinfo = {};
	poolcreateinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolcreateinfo.pNext = NULL;
	poolcreateinfo.maxSets = 1;
	poolcreateinfo.poolSizeCount = 1;
	poolcreateinfo.pPoolSizes = type_count;

	VkResult resdescriptorpool = vkCreateDescriptorPool(device, &poolcreateinfo, NULL, &vkinfo->descriptorpool);
	testresult(resdescriptorpool, "Descriptor Pool Creation");

	std::cout << "\n";
}

void vulkantest::mainloop(){
	while (!glfwWindowShouldClose(vkinfo->window)){
		glfwPollEvents();
	}
}

void vulkantest::cleanup(){

	//vkReleaseDisplayEXT(physicaldevice, vkinfo->display);
	//free(vkinfo);

	auto vkdestroydebugutilsmessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkinfo->inst, "vkDestroyDebugUtilsMessenger");

	vkdestroydebugutilsmessenger(vkinfo->inst, vkinfo->debugmessenger, NULL); 
	vkDestroySurfaceKHR(vkinfo->inst, vkinfo->surface, nullptr);
	vkDestroyCommandPool(device, commandpool, nullptr);
	vkDestroySwapchainKHR(device, vkinfo->swapchain, NULL);
	vkDestroyImage(device, vkinfo->image[0], NULL); 
	vkDestroyImageView(device, vkinfo->imageview[0], NULL);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(vkinfo->inst, nullptr);
	delete vkinfo;
	delete glminfo;
	glfwTerminate();
}
