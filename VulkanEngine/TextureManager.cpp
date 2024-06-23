#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {

	if (instance == nullptr)
		instance = new TextureManager();

	return instance;
}

TextureManager::~TextureManager() {
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