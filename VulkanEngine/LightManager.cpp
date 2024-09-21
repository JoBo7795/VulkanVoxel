#include "LightManager.h"

LightManager* LightManager::instance = nullptr;

size_t LightManager::AddLightToQueue(Light light) {

	lightQueue.push_back(light);
	return lightQueue.size()-1;
}

Light LightManager::GetLightFromQueue(size_t lightIndex) {
	return lightQueue[lightIndex];
}

LightManager* LightManager::GetInstance() {

	if (instance == nullptr)
		instance = new LightManager();

	return instance;
}

size_t  LightManager::LightQueueSize() {

	return lightQueue.size();
}