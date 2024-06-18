#pragma once

#include "Defs.h"

namespace LoadModel {
	ObjModelData LoadObjModel();
	std::vector<char> ReadFile(const std::string& filename);
}