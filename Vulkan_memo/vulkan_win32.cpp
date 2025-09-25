#include <windows.h>
#include <vulkan/vulkan.h>
#include <stdio.h>

// gcc -o vulkan_win32_example vulkan_win32_example.c -lvulkan-1 -luser32

// Global variables
VkInstance instance;
VkSurfaceKHR surface;
HWND hwnd;
HINSTANCE hInstance;

// Function to check Vulkan results
void check_vk_result(VkResult result, const char* message) {
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Vulkan error: %s\n", message);
        exit(-1);
    }
}

// Window procedure for handling messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            // Here we would render with Vulkan
            ValidateRect(hwnd, NULL);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Function to create a Win32 window
void create_win32_window(int width, int height) {
    const char* className = "VulkanWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,                        // Optional window styles
        className,                // Window class
        "Vulkan Window on Win32", // Window title
        WS_OVERLAPPEDWINDOW,      // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,                     // Parent window
        NULL,                     // Menu
        hInstance,                // Instance handle
        NULL                      // Additional application data
    );

    ShowWindow(hwnd, SW_SHOW);
}

// Function to create a Vulkan instance
void create_vulkan_instance() {
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Win32 Vulkan App",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    const char* extensions[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions,
    };

    check_vk_result(vkCreateInstance(&createInfo, NULL, &instance), "Failed to create Vulkan instance");
}

// Function to create a Vulkan surface for the Win32 window
void create_vulkan_surface() {
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = hInstance,
        .hwnd = hwnd,
    };

    check_vk_result(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface), "Failed to create Vulkan surface");
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance_, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInstance = hInstance_;
    
    // Initialize Win32 and Vulkan
    create_win32_window(800, 600);
    create_vulkan_instance();
    create_vulkan_surface();

    // Main message loop
    MSG msg;
    while (1) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Here, we would normally render with Vulkan.
        }
    }

    // Cleanup Vulkan and Win32
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
    DestroyWindow(hwnd);

    return 0;
}
