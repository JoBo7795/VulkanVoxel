#pragma once

#include "Image.h"
//#include "Buffers/Buffers.h"
#include "VulkanQueues.h"

struct Texture
{
public:
		 
	void CreateTextureRessources();

	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();

	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);


	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

};

