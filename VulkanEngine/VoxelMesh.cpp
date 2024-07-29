#include "VoxelMesh.h"

VoxelMesh::VoxelMesh() {

	for (int i = 0; i < gridLength * gridHeight * gridDepth; i++) {
		if(i == 18)
			voxelGrid.push_back(1);
		else
			voxelGrid.push_back(0);
	}

}

void VoxelMesh::LoadVoxelMesh() {

	std::cout << "create Voxel Mesh" << std::endl;

	Cube cube;
	Model model;
	GameObject gameObject;

	std::vector<Vertex> cubeArr;

	int size = voxelGrid.size();

	std::vector<Vertex> voxelDrawSides;
	std::vector<uint32_t> indiceDrawSides;
	uint32_t offset = 0;
	for (int i = 0; i < size; i++) {

		if (voxelGrid[i] == 1) {

			if (i - 1 < 27) {
				if (voxelGrid[i - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.RIGHT);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i + 1 < 27) {
				if (voxelGrid[i + 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.LEFT);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			 
			if (i - gridLength - 1 < 27) {
				if (voxelGrid[i - gridLength - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.BOTTOM);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i + gridLength - 1 < 27) {
				if (voxelGrid[i + gridLength - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.TOP);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i - gridLength * gridHeight - 1 < 27) {
				if (voxelGrid[i - gridLength * gridHeight - 1] == 0 ) {
					auto sideData = cube.getSideAsVertexArray(cube.FRONT);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i + gridLength * gridHeight - 1 < 27) {
				if (voxelGrid[i + gridLength * gridHeight - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.BACK);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
		}
	}


	model.vertices = voxelDrawSides;
	model.indices = indiceDrawSides;//cube.getSideIndices(indices);
	model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
	model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
	ModelManager::GetInstance()->AppendModelToMap(model, VOXEL_ENV);

	gameObject.modelId = VOXEL_ENV;
	gameObject.position = glm::vec3(0.0);
	GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);
}