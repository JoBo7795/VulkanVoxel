#include "TextureManager.h"
#include "VulkanDevices.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {

	if (instance == nullptr) {
		
		instance = new TextureManager();
		std::cout << "create textureManager instance: " << instance << std::endl;
	}

	return instance;
}

TextureManager::TextureManager() { std::cout << "create"; }

TextureManager::~TextureManager() {

	auto device = VulkanDevices::GetInstance()->GetDevice();

	for (Texture texture : textureQueue) {
		vkDestroySampler(device, texture.textureSampler, nullptr);
		vkDestroyImageView(device, texture.textureImageView, nullptr);
		
		vkDestroyImage(device, texture.textureImage, nullptr);
		vkFreeMemory(device, texture.textureImageMemory, nullptr);
		std::cout << " destroy texture: " << std::endl;
	}
	std::cout << "destroy textureManager instance: " << instance << std::endl;
	delete instance;
}

Texture& TextureManager::GetTextureFromQueue(int index) {

	return textureQueue[index];

}

uint32_t TextureManager::AppendTextureToQueue(Texture texture) {
	
	textureQueue.push_back(texture);
	return textureQueue.size() - 1;
}

uint8_t TextureManager::GetTextureQueueSize() {

	return textureQueue.size();
}