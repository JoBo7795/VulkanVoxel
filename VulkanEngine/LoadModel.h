#pragma once

#include "Defs.h"
#include <string>

namespace LoadModel {
	ObjModelData LoadObjModel(const std::string& filename);
	std::vector<char> ReadFile(const std::string& filename);
}