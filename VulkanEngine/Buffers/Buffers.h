#pragma once

#include "../Defs.h"
#include "../Memory.h"
#include "../VulkanQueues.h"
#include "../GameObject.h"
#include "../Camera.h"

class BufferManager
{
public:
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkPhysicalDevice  physicalDevice);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue, VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CreateFramebuffers(VkRenderPass& renderPass, std::array<VkImageView, 2>& attachments, float extentWidth, float extentHeight, VkFramebuffer& frameBuffer);
		
	void CreateCommandBuffers();
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue& queue);

	void UpdateUniformBuffer(glm::vec3& position, Camera& camera, uint32_t currentImage, float swapChainExtentWidth, float swapChainExtentHeight);

	uint16_t CreateVertexBuffer(std::vector<Vertex>& vertices, VkDeviceSize bSize = VK_WHOLE_SIZE);
	uint16_t CreateIndexBuffer(std::vector<uint32_t>& indices, VkDeviceSize bSize = VK_WHOLE_SIZE);

	void UpdateVertexBuffer(uint32_t index, void* newData, size_t dataSize);
	void UpdateIndexBuffer(uint32_t index, void* newData, size_t dataSize);

	void CreateUniformBuffers();
	void CreateUniformBuffer();
	void DeleteVertexBuffer(uint32_t vertexBufferId);
	void DeleteIndexBuffer(uint32_t indexBufferId);

	void DeleteBufferQueueSync();

	std::vector<VkBuffer>& GetUniformBuffers();
	std::vector<VkDeviceMemory>& GetUniformBuffersMemory();

	std::vector<void*>& GetUniformBuffersMapped();
	std::vector<VkCommandBuffer>& GetCommandBuffers();

	std::vector<VkBuffer>& GetVertexBuffers();
	VkBuffer& GetVertexBuffer(uint32_t id);
	void SetVertexBuffers(std::vector<VkBuffer>& vertexBuffers);


	std::vector<VkDeviceMemory>& GetVertexBufferMemoryVector();
	void SetVertexBufferMemoryVector(std::vector<VkDeviceMemory>& vertexBufferMemoryVector);


	std::vector<VkBuffer>& GetIndexBuffers();
	void SetIndexBuffers(std::vector<VkBuffer>& indexBuffers);


	std::vector<VkDeviceMemory>& GetIndexBufferMemoryVector();
	void SetIndexBufferMemoryVector(std::vector<VkDeviceMemory>& indexBufferMemoryVector);

	static BufferManager* GetInstance();

	BufferManager();
	~BufferManager();



private:
	
	static BufferManager* instance;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkBuffer> vertexBuffers;
	std::vector<VkDeviceMemory> vertexBufferMemoryVector;

	std::vector<VkBuffer> indexBuffers;
	std::vector<VkDeviceMemory> indexBufferMemoryVector;
};

