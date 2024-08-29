#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager* GameObjectManager::GetInstance() {

	if (instance == nullptr)
		instance = new GameObjectManager();

	return instance;

}

GameObjectManager::GameObjectManager() {
	uboUpdate = false;
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
	uboUpdate = true;
	gameObject.gameObjectId = gameObjectQueue.size() - 1;
	return gameObjectQueue.size()-1;
}

void GameObjectManager::DeleteGameObjectToQueue(GameObject& gameObject) {



}

void GameObjectManager::CreateGameObjectUBOs() {

	if (uboUpdate) {
		int size = gameObjectQueue.size();
		for (int i = 0; i < size; i++) {
			if (!creationStatus[i]) {
				BufferManager::GetInstance()->CreateUniformBuffer();

				creationStatus[i] = true;
			}
		}
		uboUpdate = false;
	}
}

void GameObjectManager::UpdateGameObjectUBOs() {

	if (uboUpdate) {
		int size = gameObjectQueue.size();
		for (int i = 0; i < size; i++) {
			if (!creationStatus[i]) {
				std::cout << "descriptors pointer: " << Descriptors::GetInstance() << std::endl;

				for (int j = 0; j < MAX_FRAMES_IN_FLIGHT; j++) {
					BufferManager::GetInstance()->CreateUniformBuffer();
				}

				// create ubo and feed with dummy data
				Camera placeholdCam(glm::vec3(0));
				BufferManager::GetInstance()->UpdateUniformBuffer(gameObjectQueue[i].position, placeholdCam, i * MAX_FRAMES_IN_FLIGHT, 100, 200);

				Descriptors::GetInstance()->CreateDescriptorSet(gameObjectQueue[i],MAX_FRAMES_IN_FLIGHT);

				creationStatus[i] = true;
			}
		}
		uboUpdate = false;
	}
}