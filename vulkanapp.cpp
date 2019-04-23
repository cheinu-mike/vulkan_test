#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"
#include "/home/cheinu/Documents/vulkanapp/vulkanapp.h"

#include <iostream>
#include <stdexcept>

VkResult vkCreateInstance(
		const VkInstanceCreateInfo* pCreateInfo, 
		const VkAllocationCallbacks* pAllocator, 
		VkIntance* instance) {
	if (pCreateInfo == nullptr || instance == nullptr) {
		log("null pointer passed to required parameter");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	return real_vkCreateInstance(pCreateInfo, pAllocator, instance);
}
void vulkanapp::initWindow (){
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void vulkanapp::initVulkan (){
	createInstance();
}

void vulkanapp::mainLoop() {
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

}

void vulkanapp::cleanup() {
	glfwDestroyWindow(window);

	glfwTerminate();
}


void vulkanapp::createInstance(){
	VkApplicationInfo appInfo ={};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1,1,0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1,1,0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}
}


