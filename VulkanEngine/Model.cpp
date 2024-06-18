#include "Model.h"
#include "LoadModel.h"

void Model::LoadModelFromObjFile() {

	ObjModelData data = LoadModel::LoadObjModel();

	vertices = data.vertices;
	indices = data.indices;
}