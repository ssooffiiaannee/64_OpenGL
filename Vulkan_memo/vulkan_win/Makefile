CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw3 -lvulkan-1  #-ldl -lpthread #-lX11 -lXxf86vm -lXrandr -lXi

INCLUDES = -I"D:\VulkanSDK\1.3.296.0\Include" -I"D:\glfw-3.4.bin.WIN64\glfw-3.4.bin.WIN64\include" -I"D:\glm" -L"D:\VulkanSDK\1.3.296.0\Lib" -L"D:\tricks\Vulkan_memo\vulkan_win"

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS) $(INCLUDES)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
