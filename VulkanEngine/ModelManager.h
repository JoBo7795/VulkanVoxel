#pragma once
#include "Model.h"
#include <map>


class ModelManager
{

public:
	void AppendModelToMap(Model& model, uint16_t key);
	Model& GetModelFromQueue(uint16_t key);

	static ModelManager* GetInstance();
	~ModelManager();

private:
	static ModelManager* instance;
	std::map<uint16_t, Model> modelMap;
};

