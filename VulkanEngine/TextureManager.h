#pragma once

#include "Texture.h"


class TextureManager
{

public:

	Texture& GetTextureFromQueue(int index);
	uint16_t AppendTextureToQueue(Texture texture);
	uint16_t GetTextureQueueSize();
	void CreateTextureArray(std::vector<Texture>& textureArr);

	static TextureManager* GetInstance();	

	TextureManager();
	~TextureManager();

private:

	static TextureManager* instance;
	std::vector<Texture> textureQueue;
	std::vector<VkImageView> imageArray;

};

