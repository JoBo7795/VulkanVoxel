#include "Buffers.h"
#include "../CommandPool.h"
#include "../VulkanDevices.h"

BufferManager* BufferManager::instance = nullptr;

BufferManager::BufferManager() {
    //uniformBuffers.resize(100);
    //uniformBuffersMemory.resize(100);
    //uniformBuffersMapped.resize(100);
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

    for (size_t i = 0; i < uniformBuffers.size(); i++) {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
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

void BufferManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    copyRegion.dstOffset = dstOffset;
    copyRegion.srcOffset = srcOffset;
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

uint16_t BufferManager::CreateVertexBuffer(std::vector<Vertex>& vertices, VkDeviceSize bSize) {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();
    VkPhysicalDevice physicalDevice = VulkanDevices::GetInstance()->GetPhysicalDevice();

    VkDeviceSize bufferSize = bSize;

    if(bSize == VK_WHOLE_SIZE)
        bufferSize = sizeof(vertices[0]) * vertices.size();



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

    return vertexBuffers.size() - 1;
}

uint16_t BufferManager::CreateIndexBuffer(std::vector<uint32_t>& indices, VkDeviceSize bSize) {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();
    VkPhysicalDevice physicalDevice = VulkanDevices::GetInstance()->GetPhysicalDevice();

    VkDeviceSize bufferSize = bSize;

    if (bSize == VK_WHOLE_SIZE)
        bufferSize = sizeof(indices[0]) * indices.size();


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

    return indexBuffers.size() - 1;
}

void BufferManager::UpdateVertexBuffer(uint32_t index, void* newData, size_t dataSize) {
   

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, VulkanDevices::GetInstance()->GetPhysicalDevice());

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();

    // Daten in den Staging-Buffer kopieren
    void* data;
    VkResult result = vkMapMemory(device, stagingBufferMemory, 0, dataSize, 0, &data);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to map memory!");
    }
    memcpy(data, newData, dataSize);
    vkUnmapMemory(device, stagingBufferMemory);

    // Daten in den Ziel-Buffer kopieren
    CopyBuffer(stagingBuffer, vertexBuffers[index], dataSize, VulkanQueueManager::GetInstance()->GetGraphicsQueue());

    // Staging-Buffer freigeben
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

}


void BufferManager::UpdateIndexBuffer(uint32_t index, void* newData, size_t dataSize) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, VulkanDevices::GetInstance()->GetPhysicalDevice());

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();

    // Daten in den Staging-Buffer kopieren
    void* data;
    VkResult result = vkMapMemory(device, stagingBufferMemory, 0, dataSize, 0, &data);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to map memory!");
    }
    memcpy(data, newData, dataSize);
    vkUnmapMemory(device, stagingBufferMemory);

    // Daten in den Ziel-Buffer kopieren
    CopyBuffer(stagingBuffer, indexBuffers[index], dataSize, VulkanQueueManager::GetInstance()->GetGraphicsQueue());

    // Staging-Buffer freigeben
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

void BufferManager::CreateUniformBuffer() {

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(uniformBuffers.size() + 1);
    uniformBuffersMemory.resize(uniformBuffersMemory.size() + 1);
    uniformBuffersMapped.resize(uniformBuffersMapped.size() + 1);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers.back(), uniformBuffersMemory.back(), VulkanDevices::GetInstance()->GetPhysicalDevice());

    vkMapMemory(VulkanDevices::GetInstance()->GetDevice(), uniformBuffersMemory.back(), 0, bufferSize, 0, &uniformBuffersMapped.back());


}



void BufferManager::UpdateUniformBuffer(glm::vec3& position,Camera& camera, uint32_t currentImage, float swapChainExtentWidth, float swapChainExtentHeight) {

    static auto startTime = std::chrono::high_resolution_clock::now();
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    UniformBufferObject ubo{};
    ubo.model = glm::translate(glm::mat4(1.0f), position);// *glm::rotate(glm::mat4(1), time, glm::vec3(1, 0, 0));
    ubo.view = camera.LookAt();
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtentWidth / swapChainExtentHeight, 0.1f, 1000.0f);
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

void BufferManager::DeleteVertexBuffer(uint32_t vertexBufferId) {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();

    vkQueueWaitIdle(VulkanQueueManager::GetInstance()->GetGraphicsQueue());
   
    vkDestroyBuffer(device, vertexBuffers[vertexBufferId], nullptr);
    vkFreeMemory(device, vertexBufferMemoryVector[vertexBufferId], nullptr);

}

void BufferManager::DeleteIndexBuffer(uint32_t indexbufferId) {

    VkDevice device = VulkanDevices::GetInstance()->GetDevice();
    vkQueueWaitIdle(VulkanQueueManager::GetInstance()->GetGraphicsQueue());

    vkDestroyBuffer(device, indexBuffers[indexbufferId], nullptr);
    vkFreeMemory(device, indexBufferMemoryVector[indexbufferId], nullptr);
}

void BufferManager::DeleteBufferQueueSync() {


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

std::vector<VkBuffer>& BufferManager::GetVertexBuffers() {

    return vertexBuffers;
}

VkBuffer& BufferManager::GetVertexBuffer(uint32_t id) {

    return vertexBuffers[id];
}

void BufferManager::SetVertexBuffers(std::vector<VkBuffer>& vertexBuffers) {
    this->vertexBuffers = vertexBuffers;
}


std::vector<VkDeviceMemory>& BufferManager::GetVertexBufferMemoryVector() {
    return vertexBufferMemoryVector;
}

void BufferManager::SetVertexBufferMemoryVector(std::vector<VkDeviceMemory>& vertexBufferMemoryVector) {
    this->vertexBufferMemoryVector = vertexBufferMemoryVector;
}


std::vector<VkBuffer>& BufferManager::GetIndexBuffers() {
    return indexBuffers;
}

void BufferManager::SetIndexBuffers(std::vector<VkBuffer>& indexBuffers) {

    this->indexBuffers = indexBuffers;
}


std::vector<VkDeviceMemory>& BufferManager::GetIndexBufferMemoryVector() {
    return indexBufferMemoryVector;
}

void BufferManager::SetIndexBufferMemoryVector(std::vector<VkDeviceMemory>& indexBufferMemoryVector) {
    this->indexBufferMemoryVector = indexBufferMemoryVector;
}