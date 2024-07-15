#pragma once

#include "Texture.h"


class TextureManager
{

public:

	Texture& GetTextureFromQueue(int index);
	uint32_t AppendTextureToQueue(Texture texture);
	uint8_t GetTextureQueueSize();

	static TextureManager* GetInstance();

	TextureManager();
	~TextureManager();

private:

	static TextureManager* instance;
	std::vector<Texture> textureQueue;

};

