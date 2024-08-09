#include "VoxelMesh.h"


VoxelMesh::VoxelMesh() {

	gridLength =	200;
	gridHeight =	20;
	gridDepth =		200;

	int center = (gridDepth) * (gridHeight) * (gridLength / 2 + 1);

	voxelGrid.reserve(gridDepth * gridHeight * gridLength);

	const siv::PerlinNoise::seed_type seed = 78394279843u;

	const siv::PerlinNoise perlin{ seed };


	for (int i = 0; i < gridLength * gridHeight * gridDepth; i++) {
		voxelGrid.push_back(0);
	}

	int num = 0;

	std::vector<double> heightMap;

	for (int x = 0; x < gridLength; x++)
	{
		for (int z = 0; z < gridDepth; z++)

		{
			num++;
			auto val = perlin.octave2D_01(x * .1, z * .1, 4,.2);
			int perlinY = round(val * (gridHeight - 1));
			int arrIndex = PositionToArrayIndex(glm::vec3(x, perlinY, z));

			std::random_device rd;  // Zufallsgerät
			std::mt19937 gen(rd()); // Mersenne-Twister-Generator
			std::uniform_int_distribution<> dis(1, 2); // Gleichverteilte Zufallszahlen zwischen 0 und 1

			uint8_t type = dis(gen);
			
			heightMap.push_back(round(val * (gridLength - 1)));
			voxelGrid[arrIndex] = type;

			for (int i = 0; i < perlinY;i++) {
				voxelGrid[PositionToArrayIndex(glm::vec3(x, i, z)) ] = type;
			}
		}

		std::cout << '\n';
	}
	std::cout << num << std::endl;

	auto cam = Renderer::GetInstance()->GetCamera();
	cam.SetPosition(glm::vec3(gridLength / 2, gridHeight/2, gridDepth / 2));
	Renderer::GetInstance()->SetCamera(cam);

}

void VoxelMesh::LoadVoxelMesh() {

	std::cout << "create Voxel Mesh" << std::endl;


	Model model;
	GameObject gameObject;

	std::vector<Vertex> cubeArr;

	int size = voxelGrid.size();

	int num = 0;
	uint32_t offset = 0;
	for (int32_t i = 0; i < size; i++) {

		if (voxelGrid[i] > 0) {
			num++;
			glm::vec3 gridPos = ArrayIndexToPosition(i);

			if (i - 1 < gridLength * gridHeight * gridDepth) {

				if (voxelGrid[i - 1] == 0) {
					DrawCubeSideFront(gridPos,offset, voxelGrid[i]);
				}
			}
			if (i + 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i + 1] == 0) {
					DrawCubeSideBack(gridPos, offset, voxelGrid[i]);
				}
			}

			if (i - gridLength < gridLength * gridHeight * gridDepth) {


				if (i - gridLength < 0) {
					DrawCubeSideTop(gridPos, offset, voxelGrid[i]);
					continue;
				}

				if (voxelGrid[i - gridLength] == 0) {
					DrawCubeSideTop(gridPos, offset, voxelGrid[i]);
				}
			}
			if (i + gridLength < gridLength * gridHeight * gridDepth) {


				if (i + gridLength < 0) {
					DrawCubeSideBottom(gridPos, offset, voxelGrid[i]);
					continue;
				}

				if (voxelGrid[i + gridLength] == 0) {
					DrawCubeSideBottom(gridPos, offset, voxelGrid[i]);
				}
			}

			if ((i - gridLength * gridHeight< gridLength * gridHeight * gridDepth) ) {

				if (i - gridLength * gridHeight< 0) {
					DrawCubeSideLeft(gridPos, offset, voxelGrid[i]);
					continue;
				}

				if (voxelGrid[i - gridLength * gridHeight] == 0) {
					
					DrawCubeSideLeft(gridPos, offset, voxelGrid[i]);

				}
			}
			if (i + gridLength * gridHeight< gridLength * gridHeight * gridDepth) {

				if (i + gridLength * gridHeight < 0) {
					DrawCubeSideRight(gridPos, offset, voxelGrid[i]);
					continue;
				}

				if (voxelGrid[i + gridLength * gridHeight] == 0) {

					DrawCubeSideRight(gridPos, offset, voxelGrid[i]);
				}
			}
		}
	}

	std::cout << "drawn voxel: " << num << std::endl;

	model.vertices = voxelDrawSides;
	model.indices = indiceDrawSides;
	model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
	model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
	ModelManager::GetInstance()->AppendModelToMap(model, VOXEL_ENV);

	gameObject.modelId = VOXEL_ENV;
	gameObject.position = glm::vec3(0.0);
	GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);
}

void VoxelMesh::DrawCubeSideLeft(glm::vec3 gridPos, uint32_t& offset, uint8_t type)
{
	auto sideData = cube.getSideAsVertexArray(cube.LEFT, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideRight(glm::vec3 gridPos, uint32_t& offset, uint8_t type)
{
	auto sideData = cube.getSideAsVertexArray(cube.RIGHT, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideFront(glm::vec3 gridPos, uint32_t& offset, uint8_t type)
{
	auto sideData = cube.getSideAsVertexArray(cube.FRONT, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBack(glm::vec3 gridPos, uint32_t& offset, uint8_t type)
{
	auto sideData = cube.getSideAsVertexArray(cube.BACK, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBottom(glm::vec3 gridPos, uint32_t& offset, uint8_t type)
{
	auto sideData = cube.getSideAsVertexArray(cube.BOTTOM, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideTop(glm::vec3 gridPos, uint32_t& offset, uint8_t type)
{
	auto sideData = cube.getSideAsVertexArray(cube.TOP, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

int32_t VoxelMesh::PositionToArrayIndex(glm::vec3 position) {

	return (position.x * gridLength * gridHeight) + (position.y * gridLength) + position.z;

	int32_t val = 0;
	int32_t x = position.x * (gridLength * gridHeight);
	val += x;
	int32_t y = position.y * (gridLength);
	val += y;
	int32_t z = position.z;
	val += z;
	return val;

	return (position.x * gridLength * gridHeight) + (position.y * gridLength) + position.z;
}

glm::vec3 VoxelMesh::ArrayIndexToPosition(int32_t arrayIndex) {

	int32_t x = arrayIndex / (gridLength * gridHeight);
	arrayIndex -= (x * gridLength * gridHeight);
	int32_t y = arrayIndex / gridLength;
	arrayIndex -= (y * gridLength);
	int32_t z = arrayIndex;

	return glm::vec3(x, y, z);
}
