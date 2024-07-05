#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager* GameObjectManager::GetInstance() {

	if (instance == nullptr)
		instance = new GameObjectManager();

	return instance;

}

GameObjectManager::~GameObjectManager() {

	delete instance;
}

GameObject& GameObjectManager::GetGameObjectFromQueue(int index) {

	return gameObjectQueue[index];
}

int GameObjectManager::GetGameObjectQueueSize() {
	return gameObjectQueue.size();

}

uint16_t GameObjectManager::AppendGameObjectToQueue(GameObject& gameObject) {

	gameObjectQueue.push_back(gameObject);
	return gameObjectQueue.size()-1;
}