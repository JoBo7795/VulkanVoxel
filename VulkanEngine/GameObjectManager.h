#pragma once

#include "GameObject.h"

#include <vector>
#include <algorithm>



class GameObjectManager
{

public:

	GameObject& GetGameObjectFromQueue(int index);
	int GetGameObjectQueueSize();
	uint8_t AppendGameObjectToQueue(GameObject& gameObject);
	static GameObjectManager* GetInstance();

	~GameObjectManager();

private:

	static GameObjectManager* instance;
	std::vector<GameObject> gameObjectQueue;

};
