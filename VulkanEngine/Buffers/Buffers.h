#pragma once

#include "../Includes.h"
#include "../Memory.h"
#include "../VulkanQueues.h"
#include "../GameObject.h"
#include "../Camera.h"

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

	void UpdateUniformBuffer(glm::vec3& position, Camera& camera, uint32_t currentImage, float swapChainExtentWidth, float swapChainExtentHeight);

	uint16_t CreateVertexBuffer(std::vector<Vertex>& vertices);
	uint16_t CreateIndexBuffer(std::vector<uint32_t>& indices);
	void CreateUniformBuffers();


	std::vector<VkBuffer>& GetUniformBuffers();
	std::vector<VkDeviceMemory>& GetUniformBuffersMemory();
	std::vector<void*>& GetUniformBuffersMapped();
	std::vector<VkCommandBuffer>& GetCommandBuffers();

	static BufferManager* GetInstance();

	BufferManager();
	~BufferManager();

	std::vector<VkBuffer> vertexBuffers;
	std::vector<VkDeviceMemory> vertexBufferMemoryVector;

	std::vector<VkBuffer> indexBuffers;
	std::vector<VkDeviceMemory> indexBufferMemoryVector;

private:
	
	static BufferManager* instance;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	std::vector<VkCommandBuffer> commandBuffers;
};

