#pragma once

#include "Image.h"
#include "VulkanQueues.h"

struct Texture
{
public:
		 
	void CreateTextureRessources();

	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();

	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	std::string texturePath = TEXTURE_PATH;

	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	int32_t width, height;

};

