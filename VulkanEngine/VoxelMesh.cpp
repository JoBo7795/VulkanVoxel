#include "VoxelMesh.h"

VoxelMesh::VoxelMesh() {

	gridLength = 5;
	gridHeight = 5;
	gridDepth  = 5;

	

	voxelGrid.reserve(gridDepth * gridHeight * gridLength);

	for (int i = 0; i < gridLength * gridHeight * gridDepth; i++) {
		if(i == 65  || i == 66)
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
	for (int32_t i = 0; i < size; i++) {

		if (voxelGrid[i] == 1) {

			int32_t val = i;
			int32_t x = val / (gridLength * gridHeight);
			val -= x * (gridLength * gridHeight);
			int32_t y = val / gridLength;
			val -= y * (gridHeight);
			int32_t z = val;

			glm::vec3 gridPos = glm::vec3(x,y, z);
			if (i - 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.RIGHT, gridPos);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i + 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i + 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.LEFT, gridPos);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}

			if (i - gridLength - 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i - gridLength - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.BOTTOM, gridPos);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i + gridLength - 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i + gridLength - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.TOP, gridPos);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			std::cout << gridLength << std::endl;
			std::cout << gridHeight << std::endl;
			std::cout << (int32_t)(i - gridLength * gridHeight - 1) << std::endl;


			if ((i - gridLength * gridHeight - 1 < gridLength * gridHeight * gridDepth) || ((int32_t)(i - gridLength * gridHeight - 1) < 0)) {
				if (voxelGrid[i - gridLength * gridHeight - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.FRONT, gridPos);
					voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
					auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
					indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
					offset += sideData.size();
				}
			}
			if (i + gridLength * gridHeight - 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i + gridLength * gridHeight - 1] == 0) {
					auto sideData = cube.getSideAsVertexArray(cube.BACK, gridPos);
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