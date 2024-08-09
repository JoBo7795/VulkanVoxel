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
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER; ;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding textureArrayLayoutBinding = {};
    textureArrayLayoutBinding.binding = 2;
    textureArrayLayoutBinding.descriptorCount = 2;
    textureArrayLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    textureArrayLayoutBinding.pImmutableSamplers = nullptr;
    textureArrayLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, samplerLayoutBinding, textureArrayLayoutBinding };
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

    auto goManagerRef = GameObjectManager::GetInstance();
    int size = goManagerRef->GetGameObjectQueueSize();
    GameObject gameObject;
    Texture texture;
    auto managerRef = TextureManager::GetInstance();
    
        
    for (size_t j = 0; j < size; j++) {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {


                gameObject = goManagerRef->GetGameObjectFromQueue(j);
                texture = managerRef->GetTextureFromQueue(gameObject.textureId);



                int index = j * MAX_FRAMES_IN_FLIGHT + i;

                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = bufferManagerRef->GetUniformBuffers()[index];
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(UniformBufferObject);
                std::cout << index << std::endl;

                const uint16_t drawTextureSize = managerRef->GetTextureQueueSize();

                std::vector<VkDescriptorImageInfo> descriptorImageInfos;
                
                descriptorImageInfos.reserve(drawTextureSize);

                for (uint32_t k = 0; k < drawTextureSize; k++)
                {

                    VkDescriptorImageInfo imageInfo{};
                    descriptorImageInfos.push_back(imageInfo);
                    descriptorImageInfos[k].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    //descriptorImageInfos[k].imageView = managerRef->imageArray[k]; // View 0
                    descriptorImageInfos[k].imageView = managerRef->GetTextureFromQueue(k).textureImageView; // View 0

                }

                VkDescriptorImageInfo samplerInfo{};
                samplerInfo.sampler = texture.textureSampler;

                std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = descriptorSets[index];
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = descriptorSets[index];
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pImageInfo = &samplerInfo;

                descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[2].dstSet = descriptorSets[index];
                descriptorWrites[2].dstBinding = 2;
                descriptorWrites[2].dstArrayElement = 0;
                descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                descriptorWrites[2].descriptorCount = 2;
                descriptorWrites[2].pImageInfo = &descriptorImageInfos[0];

                vkUpdateDescriptorSets(VulkanDevices::GetInstance()->GetDevice(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

        }
    }
}

void Descriptors::CleanUp() {

    auto device = VulkanDevices::GetInstance()->GetDevice();

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);   
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