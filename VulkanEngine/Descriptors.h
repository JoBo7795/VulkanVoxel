#pragma once

#include "Defs.h"
//#include "VulkanDevices.h"
#include "Buffers/Buffers.h"
#include "Texture.h"

struct Texture;

class Descriptors
{
public:

	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets(Texture& texture);

	void SetDescriptorPool(VkDescriptorPool& in_descriptorPool);
	VkDescriptorPool& GetDescriptorPool();

	void SetDescriptorSets(std::vector<VkDescriptorSet>& in_descriptorSets);
	std::vector<VkDescriptorSet>& GetDescriptorSets();

	void SetDescriptorSetLayout(VkDescriptorSetLayout& in_descriptorSetLayout);
	VkDescriptorSetLayout& GetDescriptorSetLayout();

private:

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
};

