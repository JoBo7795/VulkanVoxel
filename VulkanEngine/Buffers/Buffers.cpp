#include "Buffers.h"
#include "../CommandPool.h"
#include "../VulkanDevices.h"

BufferManager* BufferManager::instance = nullptr;

BufferManager::BufferManager() {

}

BufferManager::~BufferManager() {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();

    for(const auto& indexBuffer : indexBuffers)
        vkDestroyBuffer(device, indexBuffer, nullptr);

    for (const auto& indexBufferMemory : indexBufferMemoryVector)
        vkFreeMemory(device, indexBufferMemory, nullptr);

    for (const auto& vertexBuffer : vertexBuffers)
        vkDestroyBuffer(device, vertexBuffer, nullptr);

    for (const auto& vertexBufferMemory : vertexBufferMemoryVector)
    vkFreeMemory(device, vertexBufferMemory, nullptr);
}


BufferManager* BufferManager::GetInstance() {

    if (instance == nullptr) {
        instance = new BufferManager();
    }

    return instance;
}

void BufferManager::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory,VkPhysicalDevice  physicalDevice) {
    
    VkDevice device = VulkanDevices::GetInstance()->GetDevice();
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Memory::FindMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

    if (vkAllocateMemory(VulkanDevices::GetInstance()->GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void BufferManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue) {
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer, queue);
}

void BufferManager::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    EndSingleTimeCommands(commandBuffer, VulkanQueueManager::GetInstance()->GetGraphicsQueue());
}

void BufferManager::CreateFramebuffers(VkRenderPass& renderPass, std::array<VkImageView, 2>& attachments, float extentWidth, float extentHeight,VkFramebuffer& frameBuffer) {


     VkFramebufferCreateInfo framebufferInfo{};
     framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
     framebufferInfo.renderPass = renderPass;
     framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
     framebufferInfo.pAttachments = attachments.data();
     framebufferInfo.width = extentWidth;
     framebufferInfo.height = extentHeight;
     framebufferInfo.layers = 1;
     
     if (vkCreateFramebuffer(VulkanDevices::GetInstance()->GetDevice(),
         &framebufferInfo,
         nullptr,
         &frameBuffer) != VK_SUCCESS) {
         throw std::runtime_error("failed to create framebuffer!");
     }
}

VkCommandBuffer BufferManager::BeginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = CommandPoolManager::GetInstance()->GetCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(VulkanDevices::GetInstance()->GetDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void BufferManager::EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue& queue) {  
    
    
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);

    vkFreeCommandBuffers(VulkanDevices::GetInstance()->GetDevice(), CommandPoolManager::GetInstance()->GetCommandPool(), 1, &commandBuffer);
}

void BufferManager::CreateVertexBuffer(std::vector<Vertex>& vertices) {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();
    VkPhysicalDevice physicalDevice = VulkanDevices::GetInstance()->GetPhysicalDevice();

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    vertexBuffers.push_back(VkBuffer());
    vertexBufferMemoryVector.push_back(VkDeviceMemory());

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffers.back(), vertexBufferMemoryVector.back(), physicalDevice);

    CopyBuffer(stagingBuffer, vertexBuffers.back(), bufferSize, VulkanQueueManager::GetInstance()->GetGraphicsQueue());

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void BufferManager::CreateIndexBuffer(std::vector<uint32_t>& indices) {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();
    VkPhysicalDevice physicalDevice = VulkanDevices::GetInstance()->GetPhysicalDevice();

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    indexBuffers.push_back(VkBuffer());
    indexBufferMemoryVector.push_back(VkDeviceMemory());

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffers.back(), indexBufferMemoryVector.back(), physicalDevice);

    CopyBuffer(stagingBuffer, indexBuffers.back(), bufferSize, VulkanQueueManager::GetInstance()->GetGraphicsQueue());

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void BufferManager::CreateUniformBuffers() {

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);    

    uniformBuffers.resize(uniformBuffers.size() + MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(uniformBuffersMemory.size() + MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(uniformBuffersMapped.size() + MAX_FRAMES_IN_FLIGHT);

    int size = uniformBuffers.size();
    //for (size_t i = 0; i < size + MAX_FRAMES_IN_FLIGHT; i++) {
    for (size_t i = 0; i < size; i++) {
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i], VulkanDevices::GetInstance()->GetPhysicalDevice());

        vkMapMemory(VulkanDevices::GetInstance()->GetDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void BufferManager::UpdateUniformBuffer(glm::vec3& position, uint32_t currentImage, float swapChainExtentWidth, float swapChainExtentHeight) {

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    UniformBufferObject ubo{};
    ubo.model = glm::translate(glm::mat4(1.0f), position);// *glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtentWidth / swapChainExtentHeight, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void BufferManager::CreateCommandBuffers() {

    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = CommandPoolManager::GetInstance()->GetCommandPool();        
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(VulkanDevices::GetInstance()->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

std::vector<VkBuffer>& BufferManager::GetUniformBuffers() {

    return uniformBuffers;
}

std::vector<VkDeviceMemory>& BufferManager::GetUniformBuffersMemory() {

    return uniformBuffersMemory;
}

std::vector<void*>& BufferManager::GetUniformBuffersMapped() {

    return uniformBuffersMapped;
}

std::vector<VkCommandBuffer>& BufferManager::GetCommandBuffers() {

    return commandBuffers;
}

