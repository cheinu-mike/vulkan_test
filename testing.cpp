#define GLFW_INCLUDE_VULKAN
#include "/usr/include/GLFW/glfw3.h"

#include <iostream>
#include <string>
#include <vector>

class testing {
public:
	void run(){
	}

private:
	void createinstance() {
	}

};

void testresult(VkResult result, std::string fname){
	if (result == VK_SUCCESS){
		std::cout << fname << " successful" << std::endl;
	}
	else {
		std::cout << fname << " failed" << std::endl;
		std::cout << fname << " error code: " << result << std::endl;
	}
}

int main() {

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkInstance inst;
	VkDevice device;
	uint32_t devicecount = 0;
	VkPhysicalDevice physicaldevice = VK_NULL_HANDLE;
	//VkQueue vkqueue;
	VkCommandPool commandpool;

	VkQueueFamilyProperties qfamilystruct;

	std::vector<VkCommandBuffer> cmdbuffers = {};

	VkApplicationInfo appinfo = {};
	VkInstanceCreateInfo instanceCI = {};
	
	//struct for VkapplicationInfo	
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pNext = NULL;
	appinfo.pApplicationName = "testing app";
	appinfo.applicationVersion = VK_MAKE_VERSION(1,1,0);
	appinfo.pEngineName = "testing engine";
	appinfo.engineVersion = VK_MAKE_VERSION(1,1,0);
	appinfo.apiVersion = VK_API_VERSION_1_1;
	
	//struct for vkCreateinfo
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pNext = NULL;
	//instanceCI.flags
	instanceCI.pApplicationInfo = &appinfo;
	instanceCI.enabledLayerCount = 0;
	//instanceCI.ppEnabledLayerNames
	instanceCI.enabledExtensionCount = glfwExtensionCount;
	instanceCI.ppEnabledExtensionNames = glfwExtensions;

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

	VkResult resultinstance = vkCreateInstance(&instanceCI, NULL, &inst);

	if (resultinstance == VK_SUCCESS){
		std::cout << "instance created" << std::endl;
	}
	else {
		std::cout << "instance creation failed" << std::endl;
		std::cout << "instance error code: " << resultinstance << std::endl;
	}
		
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
	
	//Command Buffer Section
	//first get the queue family number count
	uint32_t familypropertycount; //family property count amount of queue families for queueFamilyIndex;

	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &familypropertycount, nullptr);
	vkGetPhysicalDeviceQueueFamilyProperties(physicaldevice, &familypropertycount, &qfamilystruct);
	std::cout << "family property count: " << familypropertycount << std::endl;
	std::cout << "family struct queueflags: " << qfamilystruct.queueFlags << std::endl;
	std::cout << "family struct queuecount (amount of queue families): " << qfamilystruct.queueCount << std::endl;

	//uint32_t numofqueues;

	//vkGetDeviceQueue(device, 1, 1, &vkqueue);

	//vkGetDeviceQueue(device, )

	VkCommandPoolCreateInfo cmdpoolinfo = {};
	cmdpoolinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdpoolinfo.pNext = NULL;
	cmdpoolinfo.queueFamilyIndex = 1;
	cmdpoolinfo.flags = 0;

	VkResult commandpoolresult = vkCreateCommandPool(device, &cmdpoolinfo, nullptr, &commandpool);
	
	testresult(commandpoolresult,"vk command pool creation");	

	
	cmdbuffers.resize(1);
	VkCommandBufferAllocateInfo cmd = {};
	cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd.pNext = NULL;
	cmd.commandPool = commandpool;
	cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd.commandBufferCount = (uint32_t) cmdbuffers.size();
	
	std::cout << "size of commandbuffercount " << cmd.commandBufferCount << std::endl;
	std::cout << "size of " << cmdbuffers.size() << std::endl;
	//VkResult cmdallocate = vkAllocateCommandBuffers(device, &cmd, &commandbuffers[0]);

	//testresult(cmdallocate, "vk command allocation");

	vkDestroyInstance(inst, nullptr);
	//vkDestroyDevice(device, nullptr);
	glfwTerminate();
}
