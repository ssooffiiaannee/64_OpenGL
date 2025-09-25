#include <vulkan/vulkan.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

// X11 globals
Display* display;
Window window;
int screen;

// Vulkan globals
VkInstance instance;
VkSurfaceKHR surface;

// Error handling function
void check_vk_result(VkResult result, const char* message) {
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Vulkan error: %s\n", message);
        exit(-1);
    }
}

// Function to create an X11 window
void create_x11_window(int width, int height) {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Failed to open X display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    int blackColor = BlackPixel(display, screen);
    int whiteColor = WhitePixel(display, screen);

    // Create window
    window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                                 10, 10, width, height, 1, blackColor, whiteColor);

    // Set window attributes
    XSetWindowAttributes attributes;
    attributes.event_mask = ExposureMask | KeyPressMask;
    XChangeWindowAttributes(display, window, CWEventMask, &attributes);

    // Map (show) the window
    XMapWindow(display, window);
    XStoreName(display, window, "Vulkan Window on X11");
}

// Function to create a Vulkan instance
void create_vulkan_instance() {
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "X11 Vulkan App",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    const char* extensions[] = { "VK_KHR_surface", "VK_KHR_xlib_surface" };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions,
    };

    check_vk_result(vkCreateInstance(&createInfo, NULL, &instance), "Failed to create Vulkan instance");
}

// Function to create a Vulkan surface for the X11 window
void create_vulkan_surface() {
    VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .dpy = display,
        .window = window,
    };

    check_vk_result(vkCreateXlibSurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface), "Failed to create Vulkan surface");
}

// Main function
int main() {
    // Initialize X11 and Vulkan
    create_x11_window(800, 600);
    create_vulkan_instance();
    create_vulkan_surface();

    // Event loop
    XEvent event;
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            printf("Window exposed\n");
            // Here we would normally render with Vulkan.
        }

        if (event.type == KeyPress) {
            printf("Key pressed, exiting\n");
            break;
        }
    }

    // Cleanup Vulkan and X11
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
