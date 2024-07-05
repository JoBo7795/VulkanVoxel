#pragma once
#include "Texture.h"
#include "Model.h"

struct GameObject
{
	uint8_t gameObjectId, textureId, modelId;

	glm::vec3 position, rotation;

};

