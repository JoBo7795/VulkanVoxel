#pragma once

#include "Includes.h"
#include "DepthRessources.h"
#include "SyncObjects.h"
#include "Descriptors.h"
#include "Window.h"
#include "GameObjectManager.h"
#include "ModelManager.h"

class SwapChainManager;

class GraphicsPipeline
{
public:
	void RecordCommandBuffer(VkCommandBuffer& commandBuffer, uint32_t imageIndex);


	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void SetupGraphicsPipeline();


	void DrawFrame(Window& windowRef);

	VkRenderPass& GetRenderPass();
	Descriptors& GetDescriptors();

	void CleanUp();

	GraphicsPipeline();
	~GraphicsPipeline();

	
private:

	uint32_t currentFrame = 0;

	SyncObjects syncObjects;
	DepthRessources depthRessources;
	Descriptors* descriptorsRef;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	BufferManager* bufferManagerRef;
	VulkanQueueManager* vulkanQueueManagerRef;
	SwapChainManager*  swapChainManagerRef;
	VkExtent2D swapChainExtent;
	ModelManager* managerRef;
	GameObjectManager* gOManagerRef;
	VkDevice device;
	std::vector<VkCommandBuffer> commandBuffers;

	// Render variables
	VkCommandBufferBeginInfo beginInfo{};
	VkRenderPassBeginInfo renderPassInfo{};
	std::array<VkClearValue, 2> clearValues{};
	VkClearValue clearColor;
	VkViewport viewport{};
	VkRect2D scissor{};


};

