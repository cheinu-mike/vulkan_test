#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_XLIB_KHR
#include "/usr/include/GLFW/glfw3.h"
//#include "/usr/include/GLFW/glfw3native.h"

#include "/home/cheinu/Documents/vulkanapp/headtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void vulkantest::run(){

	initwindow();

	getxlib();

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

	std::cout << "glfw display: " << vkinfo->display << std::endl;
	std::cout << "X11 display: " << display2 << std::endl;
	screencount = XScreenCount(display2);
	std::cout << "X screen count: " << screencount << std::endl;
	screen = XDefaultScreenOfDisplay(display2); 
	screenindex = XDefaultScreen(display2);
	std::cout << "X Screen Index: " << screenindex << std::endl;
	depth = XDefaultDepthOfScreen(screen);
	std::cout << "X depth: " << depth << std::endl;
	visualtype = XDefaultVisual(display2, screenindex);
	std::cout << "X visual: " << visualtype << std::endl;
	XMatchVisualInfo(display2, screenindex, depth, TrueColor, &vinfo);
	visualid = vinfo.visualid;
	Visual *vis = vinfo.visual;
	std::cout << "vinfo.visual: " << &vis << std::endl;
	std::cout << "vinfo.visualid: " << vinfo.visualid << std::endl;
	std::cout << "vinfo.screen: " << vinfo.screen << std::endl;
	//std::cout << "vinfo.class: " << vinfo.class << std::endl;
	std::cout << "vinfo.depth: " << vinfo.depth << std::endl;

	std::cout << "\n";
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


	vkEnumerateInstanceExtensionProperties(NULL, &propertycount, NULL);
	std::cout << "Property Count: " << propertycount << std::endl;

	vkinfo->extensionproperties.resize(propertycount);
	std::cout <<  vkinfo->extensionproperties.size() << std::endl;

	vkEnumerateInstanceExtensionProperties(NULL, &propertycount, vkinfo->extensionproperties.data());
	for (uint32_t i; i < propertycount; i++){
		std::cout << vkinfo->extensionproperties[i].extensionName << std::endl;
	}
	
	VkInstanceCreateInfo instanceCI = {};
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pNext = NULL;
	//instanceCI.flags
	instanceCI.pApplicationInfo = &appinfo;
	instanceCI.enabledLayerCount = 0;
	//instanceCI.ppEnabledLayerNames
	
	std::cout << "enabled glfw extension count: "<< vkinfo->glfwExtensionCount << std::endl;

	if (false){
		instanceCI.enabledExtensionCount = vkinfo->glfwExtensionCount;
		instanceCI.ppEnabledExtensionNames = vkinfo->glfwExtensions;
	}
	else {
		instanceCI.enabledExtensionCount = 1;
		instanceCI.ppEnabledExtensionNames = vkinfo->extensionnames; 
	}

	VkResult resultinstance = vkCreateInstance(&instanceCI, NULL, &inst);

	std::cout << "Enabled extention count: " << instanceCI.enabledExtensionCount << std::endl;
	std::cout << "Enabled Extention names: " << instanceCI.ppEnabledExtensionNames << std::endl;

	//std::cout << "glfwgeterror: " << code << std::endl;

	testresult(resultinstance, "instance creation");

	std::cout << "\n";
}

void vulkantest::devicestructs(){
	
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

	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &familypropertycount, nullptr);
	std::cout << "familypropertycount: " << familypropertycount << std::endl;
	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &familypropertycount, &qfamilystruct);
	std::cout << "family property count: " << familypropertycount << std::endl;
	std::cout << "family struct queueflags: " << qfamilystruct.queueFlags << std::endl;
	std::cout << "family struct queuecount: " << qfamilystruct.queueCount << std::endl;
	
	if (qfamilystruct.queueFlags & VK_QUEUE_GRAPHICS_BIT){
			std::cout << "graphics bit success" << std::endl;
	}
	else {
			std::cout << "what are you doing?" << std::endl;
	}

	float qpriorities[1] = {0.5};
	VkDeviceQueueCreateInfo qcreateinfo = {};
	qcreateinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	qcreateinfo.pNext = NULL;
	qcreateinfo.flags = 0;
	qcreateinfo.queueFamilyIndex = 0; //create function that will find the appropritate queue family
	qcreateinfo.queueCount = queuecountnum;
	qcreateinfo.pQueuePriorities = qpriorities;

	for (uint32_t i=0; i < familypropertycount; i++){
			if (vkGetPhysicalDeviceXlibPresentationSupportKHR(physicaldevice, i, vkinfo->display, visualid) == VK_TRUE){
					std::cout << "queue family index supported: " << i << std::endl;
					vkinfo->qfamindex.push_back(i);
			}
			else{
					std::cout << "queue index " << i << " does not support Xlib" << std::endl;
			}
	}

	/*
	for (std::size_t i = 0; vkinfo->qfamindex.size(); i++){
		vkGetDeviceQueue(device, vkinfo->qfamindex[i], 0, vkinfo->vkqueue.data());
		//vkGetDeviceQueue(device, 0, 0, vkinfo->vkqueue.data());
	}
	*/

	//vkGetDeviceQueue2(device, &queueinfo2, &vkqueue);
	//get some more info on queue2;
	//std::cout << "queueinfo2.queueFamilyIndex: " << queueinfo2.queueFamilyIndex << std::endl;
	//std::cout << "queueinfo2.queueIndex: " << queueinfo2.queueIndex << std::endl;

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

	VkResult resultdevice = vkCreateDevice(physicaldevice, &deviceinfo, nullptr, &device);

	testresult(resultdevice, "vk device creation");
	
	std::cout << "\n";
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
	std::cout << "size of cmdbuffer vector" << vkinfo->cmdbuffers.size() << std::endl;

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
	
	std::cout << "\n";
}

void vulkantest::surfacecreation(){
	
	VkXlibSurfaceCreateInfoKHR surfaceinfo = {};
	surfaceinfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceinfo.pNext = NULL;
	surfaceinfo.flags = 0;
	surfaceinfo.dpy = display2;
	surfaceinfo.window = vkinfo->x11window;	

	VkResult surfaceresult2 = glfwCreateWindowSurface(inst, vkinfo->window, nullptr, &surface);
	VkResult surfaceresult = vkCreateXlibSurfaceKHR(inst, &surfaceinfo, nullptr, &surface);

	testresult(surfaceresult, "glfw surface creation");
	
	std::cout << "\n";
}

void vulkantest::swapchaincreation(){
	
	VkResult surcapres = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicaldevice, surface, &vkinfo->surfacecapabilities);
	testresult(surcapres, "surface capabilities creation");
	
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicaldevice, surface, &vkinfo->presentmodecount, nullptr);
	std::cout << "number of present mode count: " << vkinfo->presentmodecount << std::endl;

	VkResult surpresres = vkGetPhysicalDeviceSurfacePresentModesKHR(physicaldevice, surface, &vkinfo->presentmodecount, &vkinfo->presentmode);
	testresult(surpresres, "surface present mode creation");
   		
	
	VkSwapchainCreateInfoKHR swapcreate = {};
	swapcreate.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapcreate.pNext = NULL;
	swapcreate.surface = surface;
	swapcreate.minImageCount = vkinfo->surfacecapabilities.minImageCount;
	swapcreate.imageFormat = format;
	//swapcreate.imageColorSpace = ;
	swapcreate.imageExtent.width = vkinfo->Width;
	swapcreate.imageExtent.height = vkinfo->Height;
	/*
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
	imageviewinfo.image = vkinfo->image[0];
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

	VkResult imageviewcreate = vkCreateImageView(device, &imageviewinfo, nullptr, &vkinfo->imageview[0]);

	testresult(imageviewcreate, "vk image view creation");	
	
	std::cout << "\n"; 
}

void vulkantest::mainloop(){
	while (!glfwWindowShouldClose(vkinfo->window)){
		glfwPollEvents();
	}	
}

void vulkantest::cleanup(){

	//free(vkinfo);
	delete vkinfo;

	vkDestroySurfaceKHR(inst, surface, nullptr);
	vkDestroyCommandPool(device, commandpool, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(inst, nullptr);
	glfwTerminate();
}
