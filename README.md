# Vulkan Code Testing

g++ *.cpp -lX11 -lglfw -lvulkan

g++ -g -Wall -std=c++17 -lvulkan -lglfw -lX11 $folder/testing.cpp $folder/headtest.h $folder/headtest.cpp

works for compilation on ubuntu 19, with the aforementioned libraries installed of course. 

## Problems to Fix
1. Set VkDevice to detect and take in multiple vulkan compatible devices
1. The number of Vkimage structs should correspond to the amount of imagecount that you get from vkGetSwapchainImagesKHR(VKdevice, Vkswapchainkhr, &Swapchain_ImageCount, VkImageCreateInfo.data())??
1. Same thing as above for VkImageViews????????
1. Figure out what is meant by compiling the shaders, creating shader modules, etc
1. Then get graphics pipeline set up
1. segfault when destorying stuff. Probably destorying more than what I need to.

1. get it running on windows

### misc notes
- The inclusion of glm library is possibly slowing down compilation
