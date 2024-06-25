#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

uint16_t ModelManager::AppendModelToQueue(Model model){
	modelQueue.push_back(model);
	return modelQueue.size() - 1;
}

Model ModelManager::GetModelFromQueue(uint16_t index) {

	return modelQueue[index];
}

ModelManager* ModelManager::GetInstance() {

	if (instance == nullptr)
		instance = new ModelManager();
	return instance;
}

ModelManager::~ModelManager() {

	delete instance;
}