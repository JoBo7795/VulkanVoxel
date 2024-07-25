#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

void ModelManager::AppendModelToMap(Model model, uint16_t key) {
	modelMap[key] = model;
}

Model ModelManager::GetModelFromQueue(uint16_t key) {
	return modelMap[key];
}

ModelManager* ModelManager::GetInstance() {

	if (instance == nullptr)
		instance = new ModelManager();
	return instance;
}

ModelManager::~ModelManager() {
	delete instance;
}