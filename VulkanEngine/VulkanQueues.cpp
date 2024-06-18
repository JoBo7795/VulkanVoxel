#include "VulkanQueues.h"

VulkanQueueManager* VulkanQueueManager::instance;

VulkanQueueManager::VulkanQueueManager() {

}

VulkanQueueManager::~VulkanQueueManager() {

}

VulkanQueueManager* VulkanQueueManager::GetInstance() {

    if (instance == nullptr) {
        instance = new VulkanQueueManager();
    }
    return instance;

}

QueueFamilyIndices VulkanQueueManager::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}


void VulkanQueueManager::SetGraphicsQueue(VkQueue& in_graphicsQueue) {

    graphicsQueue = in_graphicsQueue;
}

VkQueue& VulkanQueueManager::GetGraphicsQueue() {

    return graphicsQueue;
}

void VulkanQueueManager::SetPresentQueue(VkQueue& in_presentQueue) {

    presentQueue = in_presentQueue;
}

VkQueue& VulkanQueueManager::GetPresentQueue() {

    return presentQueue;
}