#pragma once

#include "../Includes.h"
#include "../Memory.h"
//#include "../CommandPool.h"
//#include "../VulkanDevices.h"
#include "../VulkanQueues.h"

class BufferManager
{
public:
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkPhysicalDevice  physicalDevice);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CreateFramebuffers(VkRenderPass& renderPass, std::array<VkImageView, 2>& attachments, float extentWidth, float extentHeight, VkFramebuffer& frameBuffer);
		
	void CreateCommandBuffers();
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue& queue);

	void UpdateUniformBuffer(uint32_t currentImage, float swapChainExtentWidth, float swapChainExtentHeight);

	void CreateVertexBuffer(std::vector<Vertex>& vertices);
	void CreateIndexBuffer(std::vector<uint32_t>& indices);
	void CreateUniformBuffers();


	std::vector<VkBuffer>& GetUniformBuffers();
	std::vector<VkDeviceMemory>& GetUniformBuffersMemory();
	std::vector<void*>& GetUniformBuffersMapped();
	std::vector<VkCommandBuffer>& GetCommandBuffers();

	// const std::vector<VkFramebuffer>& GetSwapChainFramebuffers();
	// void SetSwapChainFramebuffers(const std::vector<VkFramebuffer>& framebuffers);

	static BufferManager* GetInstance();

	BufferManager();
	~BufferManager();

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
private:
	static BufferManager* instance;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	std::vector<VkCommandBuffer> commandBuffers;
};

