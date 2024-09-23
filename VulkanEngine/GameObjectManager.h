#pragma once

#include "GameObject.h"
#include "Descriptors.h"

#include <vector>
#include <algorithm>
#include <map>


class GameObjectManager
{

public:

	GameObject& GetGameObjectFromQueue(int index);
	int GetGameObjectQueueSize();
	uint16_t AppendGameObjectToQueue(GameObject& gameObject);
	void DeleteGameObjectToQueue(GameObject& gameObject);
	void CreateGameObjectUBOs();
	void UpdateGameObjectUBOs();
	static GameObjectManager* GetInstance();

	GameObjectManager();
	~GameObjectManager();

private:

	bool uboUpdate;
	static GameObjectManager* instance;
	std::vector<GameObject> gameObjectQueue;
	std::unordered_map<size_t,bool> creationStatus;

};
