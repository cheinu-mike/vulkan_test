#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_XLIB_KHR
#include "/usr/include/GLFW/glfw3.h"

#include "/home/cheinu/Documents/vulkanapp/headtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <X11/Xlib.h>

void vulkantest::run(){

	initwindow();
	createinstance();
	devicestructs();
	commandbuffers();
	surfacecreation();
	swapchaincreation();
	imageviewcreation();

	mainloop();
	cleanup();
}

void vulkantest::initwindow(){
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(Width, Height, "Vulkan", nullptr, nullptr);
}

void vulkantest::createinstance(){

	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pNext = NULL;
	appinfo.pApplicationName = "testing app";
	appinfo.applicationVersion = VK_MAKE_VERSION(1,1,0);
	appinfo.pEngineName = "testing engine";
	appinfo.engineVersion = VK_MAKE_VERSION(1,1,0);
	appinfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo instanceCI = {};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pNext = NULL;
	//instanceCI.flags
	instanceCI.pApplicationInfo = &appinfo;
	instanceCI.enabledLayerCount = 0;
	//instanceCI.ppEnabledLayerNames
	instanceCI.enabledExtensionCount = glfwExtensionCount;
	instanceCI.ppEnabledExtensionNames = glfwExtensions;

	VkResult resultinstance = vkCreateInstance(&instanceCI, NULL, &inst);

	testresult(resultinstance, "instance creation");
}

void vulkantest::devicestructs(){

	float qpriorities[1] = {0.0};
	VkDeviceQueueCreateInfo qcreateinfo = {};
	qcreateinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	qcreateinfo.pNext = NULL;
	qcreateinfo.queueCount = 1;
	qcreateinfo.pQueuePriorities = qpriorities;
		
	
	VkDeviceCreateInfo deviceinfo = {};
	deviceinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceinfo.pNext = NULL;
	deviceinfo.queueCreateInfoCount = 1;
	deviceinfo.pQueueCreateInfos = &qcreateinfo;
	deviceinfo.enabledExtensionCount = 0;
	deviceinfo.ppEnabledExtensionNames = NULL;
	deviceinfo.enabledLayerCount = 0;
	deviceinfo.ppEnabledLayerNames = NULL;
	deviceinfo.pEnabledFeatures = NULL;

	VkResult holder = vkEnumeratePhysicalDevices(inst, &devicecount, nullptr); //puts a number into devicecount

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


	VkResult holder2 = vkEnumeratePhysicalDevices(inst, &devicecount, &physicaldevice);

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

	VkResult resultdevice = vkCreateDevice(physicaldevice, &deviceinfo, nullptr, &device);

	testresult(resultdevice, "vk device creation");
	
	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &familypropertycount, nullptr);
	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &familypropertycount, &qfamilystruct);
	std::cout << "family property count: " << familypropertycount << std::endl;
	std::cout << "family struct queueflags: " << qfamilystruct.queueFlags << std::endl;
	std::cout << "family struct queuecount: " << qfamilystruct.queueCount << std::endl;
	
	vkGetDeviceQueue(device, 0, 0, &vkqueue);
	vkGetDeviceQueue2(device, &queueinfo2, &vkqueue);
	//get some more info on queue2;
	std::cout << "queueinfo2.queueFamilyIndex: " << queueinfo2.queueFamilyIndex << std::endl;
	std::cout << "queueinfo2.queueIndex: " << queueinfo2.queueIndex << std::endl;

}

void vulkantest::commandbuffers(){

	VkCommandPoolCreateInfo cmdpoolinfo = {};
	cmdpoolinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdpoolinfo.pNext = NULL;
	cmdpoolinfo.queueFamilyIndex = 15;
	cmdpoolinfo.flags = 0;

	VkResult commandpoolresult = vkCreateCommandPool(device, &cmdpoolinfo, nullptr, &commandpool);
	
	testresult(commandpoolresult,"vk command pool creation");

	std::cout << &commandpool << std::endl;
	
	//cmdbuffers.resize(15);
	VkCommandBufferAllocateInfo cmd = {};
	cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd.pNext = NULL;
	cmd.commandPool = commandpool;
	cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd.commandBufferCount = 1; //(uint32_t) cmdbuffers.size();
	
	std::cout << "cmd.commandbuffercount " << cmd.commandBufferCount << std::endl;
	std::cout << "size of cmdbuffer vector" << cmdbuffers.size() << std::endl;

	VkCommandBufferBeginInfo begininfo = {};
	begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begininfo.pNext = NULL;
	begininfo.flags = 0;

	//segfaulting with the VkCommandBuffer vector bullshit
	//VkResult begincmdbuff = vkBeginCommandBuffer(cmdbuffers[0], &begininfo);

	//testresult(begincmdbuff, "vk begin command buffer thing");

	std::cout << "before segfault" << std::endl;

	VkResult cmdallocate = vkAllocateCommandBuffers(device, &cmd, &cmdbuffer2); 

	//VkResult cmdallocate = vkAllocateCommandBuffers(device, &cmd, nullptr); 

	std::cout << "you are here" << std::endl;

	testresult(cmdallocate, "vk command allocation");
}

void vulkantest::surfacecreation(){
	
	VkXlibSurfaceCreateInfoKHR surfaceinfo = {};
	surfaceinfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceinfo.pNext = NULL;
	surfaceinfo.flags = 0;
	surfaceinfo.dpy = display;
	surfaceinfo.window = x11window;	

}

void vulkantest::swapchaincreation(){
	
	/*
	VkSwapchainCreateInfoKHR swapcreate = {};
	swapcreate.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapcreate.pNext = NULL;
	swapcreate.surface = surface;
	swapcreate.minImageCount = 1;
	swapcreate.imageformat = format;
	//swapcreate.imageColorSpace = ;
	swapcreate.imageExtent.width = Width;
	swapcreate.imageExtent.height = Height;
	swapcreate.imageArrayLayers = ;
	swapcreate.imageUsage = ;
	swapcreate.imageSharingMode = ;
	swapcreate.queueFamilyIndexCount = 2;
	swapcreate.pQueueFamilyIndices = ;
	swapcreate.preTransform = ;
	swapcreate.compositeAlpha = ;
	swapcreate.presentMode = ;
	swapcreate.clipped = ;
	swapcreate.oldSwapchain = ;
	*/
	
}

void vulkantest::imageviewcreation(){
	
	VkImageViewCreateInfo imageviewinfo = {};
	imageviewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageviewinfo.pNext = NULL;
	imageviewinfo.flags = 0;
	imageviewinfo.image = image;
	imageviewinfo.viewType = imageviewtype;
	imageviewinfo.format = format;
	imageviewinfo.components.r = VK_COMPONENT_SWIZZLE_R;
	imageviewinfo.components.g = VK_COMPONENT_SWIZZLE_G;
	imageviewinfo.components.b = VK_COMPONENT_SWIZZLE_B;
	imageviewinfo.components.a = VK_COMPONENT_SWIZZLE_A;
	imageviewinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	imageviewinfo.subresourceRange.baseMipLevel = 0;
	imageviewinfo.subresourceRange.levelCount = 1;
	imageviewinfo.subresourceRange.baseArrayLayer = 0;
	imageviewinfo.subresourceRange.layerCount = 1;
	
	std::cout << "before segfault" << std::endl;

	VkResult imageviewcreate = vkCreateImageView(device, &imageviewinfo, nullptr, &imageview);

	testresult(imageviewcreate, "vk image view creation");	
}

void vulkantest::mainloop(){
	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
	}	
}

void vulkantest::cleanup(){

	vkDestroySurfaceKHR(inst, surface, nullptr);
	vkDestroyCommandPool(device, commandpool, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(inst, nullptr);
	glfwTerminate();
}
