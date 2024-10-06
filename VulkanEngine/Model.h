#pragma once

#include "Defs.h"
#include <string>

struct Model
{

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	size_t verticeCount = 0, indiceCount = 0;

	uint16_t indexBufferId, verticeBufferId;
	
	void LoadModelFromObjFile(const std::string& filename);

};

