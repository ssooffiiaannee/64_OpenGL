CFLAGS = -std=c++17 -O2

INCLUDES = 	-I/home/sofiane/Desktop/repos/tricks/Vulkan_memo/vulkan_linux/vulkansdk-linux-x86_64-1.4.313.0/1.4.313.0/x86_64/include \
			-I/home/sofiane/Desktop/repos/tricks/Vulkan_memo/vulkan_linux/glfw/include

LIBS = 	-L/home/sofiane/Desktop/repos/tricks/Vulkan_memo/vulkan_linux/glfw/build/src \
		-L/home/sofiane/Desktop/repos/tricks/Vulkan_memo/vulkan_linux/vulkansdk-linux-x86_64-1.4.313.0/1.4.313.0/x86_64/lib

LDFLAGS = $(LIBS) $(INCLUDES) -lglfw3 -lVkLayer_khronos_validation -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS)  main.cpp -o VulkanTest $(LDFLAGS) 

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
