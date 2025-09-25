uint32_t queueFamilyCount = 0;
vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

VkQueueFamilyProperties* queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

for (uint32_t i = 0; i < queueFamilyCount; i++) {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        printf("Queue Family %d supports Graphics.\n", i);
    }
    if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
        printf("Queue Family %d supports Compute.\n", i);
    }
    if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
        printf("Queue Family %d supports Transfer.\n", i);
    }
    if (queueFamilies[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
        printf("Queue Family %d supports Sparse Binding.\n", i);
    }
}

free(queueFamilies);
