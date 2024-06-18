#pragma once

#include "Defs.h"


class VulkanQueueManager
{
public:
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    void SetGraphicsQueue(VkQueue& in_graphicsQueue);
    VkQueue& GetGraphicsQueue();

    void SetPresentQueue(VkQueue& in_presentQueue);
    VkQueue& GetPresentQueue();

    static VulkanQueueManager* GetInstance();

    VulkanQueueManager();
    ~VulkanQueueManager();

private:
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    static VulkanQueueManager* instance;

};

