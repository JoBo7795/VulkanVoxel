#pragma once
#include "Model.h"


class ModelManager
{

public:
	uint16_t AppendModelToQueue(Model model);
	Model GetModelFromQueue(uint16_t index);

	static ModelManager* GetInstance();
	~ModelManager();

private:
	static ModelManager* instance;
	std::vector<Model> modelQueue;
};

