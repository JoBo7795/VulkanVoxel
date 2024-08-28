#pragma once

#include "Defs.h"
#include "Buffers/Buffers.h"
#include "TextureManager.h"

struct Texture;

class Descriptors
{
public:

	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	size_t CreateDescriptorPool(VkDescriptorPoolCreateInfo& poolInfo, std::vector<VkDescriptorPoolSize>& poolSizes);
	void CreateDescriptorSets();
	void CreateDescriptorSet(GameObject& gameObject,const uint32_t uboCount);
	void DeleteDescriptorSet();
	void UpdateDescriptorSets();

	void SetDescriptorPool(VkDescriptorPool& in_descriptorPool);
	VkDescriptorPool& GetDescriptorPool(uint32_t index);

	void SetDescriptorSets(std::vector<VkDescriptorSet>& in_descriptorSets);
	std::vector<VkDescriptorSet>& GetDescriptorSets();

	void SetDescriptorSetLayout(VkDescriptorSetLayout& in_descriptorSetLayout);
	VkDescriptorSetLayout& GetDescriptorSetLayout();

	void CleanUp();

	Descriptors();

	static Descriptors* GetInstance();

private:

	static Descriptors* instance;

	uint32_t poolSize;
	uint8_t updateSize;
	std::unordered_map<size_t, bool> goCreationMap;

	std::vector<VkDescriptorPool> descriptorPools;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
};

