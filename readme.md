# Vulkan Code Testing

g++ *.cpp -lX11 -lglfw -lvulkan

g++ -g -Wall -std=c++17 -lvulkan -lglfw -lX11 $vulkanpath/testing.cpp $vulkanpath/headtest.h $vulkanpath/headtest.cpp
where $vulkanpath is the folder where the files are

 works for compilation on ubuntu 19, with the aforementioned libraries installed of course. 

## problems to fix
1. The number of imageview structs should correspond to the amount of imagecount that you get from vkGetSwapchainImagesKHR()????
2. Figure out what is meant by compiling the shaders, creating shader modules, etc
3. Then get graphics pipeline set up
