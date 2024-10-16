#pragma once
#include "Texture.h"
#include "Model.h"

struct GameObject
{
	size_t gameObjectId = 0, textureId = 0, modelId = 0;

	glm::vec3 position, rotation;

};

