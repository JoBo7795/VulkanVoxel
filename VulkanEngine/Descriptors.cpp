#include "Descriptors.h"
#include "VulkanDevices.h"

void Descriptors::CreateDescriptorPool() {

    uint16_t descriptorCount = BufferManager::GetInstance()->GetUniformBuffers().size();

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(descriptorCount);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(descriptorCount);


    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(descriptorCount);

    if (vkCreateDescriptorPool(VulkanDevices::GetInstance()->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Descriptors::CreateDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(VulkanDevices::GetInstance()->GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}


void Descriptors::CreateDescriptorSets() {
    auto bufferManagerRef = BufferManager::GetInstance();

    std::vector<VkDescriptorSetLayout> layouts(bufferManagerRef->GetUniformBuffers().size(), descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(bufferManagerRef->GetUniformBuffers().size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(bufferManagerRef->GetUniformBuffers().size());
    if (vkAllocateDescriptorSets(VulkanDevices::GetInstance()->GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

        //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT * bufferManagerRef->uboCount; i++) {
    for (size_t i = 0; i < bufferManagerRef->GetUniformBuffers().size(); i++) {
        for (size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = bufferManagerRef->GetUniformBuffers()[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            auto managerRef = TextureManager::GetInstance();
            uint16_t size = TextureManager::GetInstance()->GetTextureQueueSize();

            Texture texture;

            for (uint16_t i = 0; i < size; i++) {
                texture = managerRef->GetTextureFromQueue(i);
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = texture.textureImageView;
                imageInfo.sampler = texture.textureSampler;
            }

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(VulkanDevices::GetInstance()->GetDevice(), 2, descriptorWrites.data(), 0, nullptr);
        }
    }
}

void Descriptors::SetDescriptorPool(VkDescriptorPool& in_descriptorPool) {

    descriptorPool = in_descriptorPool;
}

VkDescriptorPool& Descriptors::GetDescriptorPool() {

    return descriptorPool;
}

void Descriptors::SetDescriptorSets(std::vector<VkDescriptorSet>& in_descriptorSets) {

    descriptorSets = in_descriptorSets;
}

std::vector<VkDescriptorSet>& Descriptors::GetDescriptorSets() {

    return descriptorSets;
}

void Descriptors::SetDescriptorSetLayout(VkDescriptorSetLayout& in_descriptorSetLayout) {

    descriptorSetLayout = in_descriptorSetLayout;
}

VkDescriptorSetLayout& Descriptors::GetDescriptorSetLayout() {

    return descriptorSetLayout;
}