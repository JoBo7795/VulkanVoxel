#pragma once
#include "Includes.h"

struct Light {

	glm::vec4 position;
	glm::vec4 color;
	float intensity;
};

class LightManager
{
public:

	size_t AddLightToQueue(Light light);
	Light GetLightFromQueue(size_t lightIndex);

	static LightManager* GetInstance();
	size_t LightQueueSize();

private:
	std::vector<Light> lightQueue;
	static LightManager* instance;


};

