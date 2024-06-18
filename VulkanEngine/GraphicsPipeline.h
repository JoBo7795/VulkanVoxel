#pragma once

#include "Includes.h"
#include "DepthRessources.h"
#include "SyncObjects.h"
#include "Descriptors.h"
#include "Window.h"
#include "Model.h"

class GraphicsPipeline
{
public:
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,  Model& model);

	void CreateRenderPass();
	void CreateGraphicsPipeline();

	void DrawFrame(Window& windowRef, Model& model);

	VkRenderPass& GetRenderPass();

	void CleanUp();
	GraphicsPipeline();
	GraphicsPipeline(Texture& texture);
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

