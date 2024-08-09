#pragma once

#include "Includes.h"

#include "Memory.h"
#include "Buffers/Buffers.h"

namespace Image
{

	//void CreateImageViews(SwapChainManager& swapChain);
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t layerCount);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	VkImageView CreateImageArrayView(VkImage& imageArray, VkImageView& imageViewArray, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, uint32_t layerCount);
	void CopyTextureDataToLayer(uint32_t width, uint32_t height, VkImage srcImage, VkImage dstImage, uint32_t layerIndex, VkCommandBuffer commandBuffer);
};

