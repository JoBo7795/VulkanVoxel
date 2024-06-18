#pragma once

#include "Defs.h"


struct Model
{

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void LoadModelFromObjFile();
};

