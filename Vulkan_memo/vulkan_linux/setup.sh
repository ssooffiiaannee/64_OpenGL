export VK_VALIDATION_VALIDATE_CORE=true
export VK_VALIDATION_VALIDATE_SYNC=true
export VK_VALIDATION_THREAD_SAFETY=true
export VK_VALIDATION_DEBUG_ACTION=VK_DBG_LAYER_ACTION_LOG_MSG
export VK_VALIDATION_REPORT_FLAGS=debug:error:perf:info:warn
export VK_VALIDATION_ENABLE_MESSAGE_LIMIT=true
export VK_VALIDATION_DUPLICATE_MESSAGE_LIMIT=3

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/sofiane/Desktop/repos/tricks/Vulkan_memo/vulkan_linux/vulkansdk-linux-x86_64-1.4.313.0/1.4.313.0/x86_64/lib VK_LAYER_PATH=/home/sofiane/Desktop/repos/tricks/Vulkan_memo/vulkan_linux/vulkansdk-linux-x86_64-1.4.313.0/1.4.313.0/x86_64/share/vulkan/explicit_layer.d/ ./VulkanTest
