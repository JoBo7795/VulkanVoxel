#include "Model.h"
#include "LoadModel.h"


void Model::LoadModelFromObjFile(const std::string& filename) {

	ObjModelData data = LoadModel::LoadObjModel(filename);

	vertices = data.vertices;
	indices = data.indices;
}

