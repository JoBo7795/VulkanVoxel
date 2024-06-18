#include "CommandPool.h"

CommandPoolManager* CommandPoolManager::instance;


void CommandPoolManager::CreateCommandPool(VkSurfaceKHR surface) {
    QueueFamilyIndices queueFamilyIndices = VulkanQueueManager::GetInstance()->FindQueueFamilies(VulkanDevices::GetInstance()->GetPhysicalDevice(),surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(VulkanDevices::GetInstance()->GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPoolManager* CommandPoolManager::GetInstance() {

    if (instance == nullptr) {
        instance = new CommandPoolManager();
    }

    return instance;
}

VkCommandPool& CommandPoolManager::GetCommandPool(){
    return commandPool;
}


CommandPoolManager::CommandPoolManager() {

}

CommandPoolManager::~CommandPoolManager() {
    vkDestroyCommandPool(VulkanDevices::GetInstance()->GetDevice(), commandPool, nullptr);
    delete instance;
}