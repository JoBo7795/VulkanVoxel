#pragma once

#include "Includes.h"
#include "VulkanQueues.h"
#include "VulkanDevices.h"

class CommandPoolManager {
public:

    VkCommandPool& GetCommandPool();
    void CreateCommandPool(VkSurfaceKHR surface);

    static CommandPoolManager* GetInstance();
    ~CommandPoolManager();
private:


    static CommandPoolManager* instance;
    VkCommandPool commandPool;

    CommandPoolManager();

};
