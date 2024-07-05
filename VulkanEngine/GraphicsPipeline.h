#pragma once

#include "Includes.h"
#include "DepthRessources.h"
#include "SyncObjects.h"
#include "Descriptors.h"
#include "Window.h"
#include "GameObjectManager.h"
#include "ModelManager.h"

class GraphicsPipeline
{
public:
	void RecordCommandBuffer(uint16_t modelIndex, VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void SetupGraphicsPipeline();


	void DrawFrame(Window& windowRef, Model& model);

	VkRenderPass& GetRenderPass();

	void CleanUp();

	GraphicsPipeline();
	~GraphicsPipeline();
private:

	uint32_t currentFrame = 0;

	SyncObjects syncObjects;
	DepthRessources depthRessources;
	Descriptors descriptors;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;	
};

