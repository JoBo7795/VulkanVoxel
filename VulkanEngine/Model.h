#pragma once

#include "Defs.h"
#include <string>

struct Model
{

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void LoadModelFromObjFile(const std::string& filename);
};

