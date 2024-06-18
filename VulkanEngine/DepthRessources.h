#pragma once

#include "Image.h"

class DepthRessources
{
public:

	VkFormat FindDepthFormat();
	void CreateDepthResources(float extentWidth, float extentHeight);
	bool HasStencilComponent(VkFormat format);

	VkImageView& GetImageView();

	~DepthRessources();

private:
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
};

