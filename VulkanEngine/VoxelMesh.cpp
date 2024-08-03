#include "VoxelMesh.h"

std::vector<double> normalizeVector(const std::vector<double>& values) {
	double min = *std::min_element(values.begin(), values.end());
	double max = *std::max_element(values.begin(), values.end());
	std::vector<double> normalized_values(values.size());

	std::transform(values.begin(), values.end(), normalized_values.begin(),
		[min, max](double value){
			return (value - min) / (max - min);
		});

	return normalized_values;
}



VoxelMesh::VoxelMesh() {

	gridLength =	100;
	gridHeight =	100;
	gridDepth =		100;

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
			auto val = perlin.octave2D_01(x * 1., z * .1, 4);
			int arrIndex = PositionToArrayIndex(glm::vec3(x, val , z));
			heightMap.push_back(val);
			//voxelGrid[arrIndex] = 1;

			//std::cout << "x: " << x << " y: " << round(val * 5) << " z: " << z << " arrIndex: " << heightMap.back() << " val: " << val << '\t';
			//std::cout << "array Index: " << PositionToArrayIndex(glm::vec3(x, val * 5, z)) << '\t';
		}

		std::cout << '\n';
	}
	std::cout << num << std::endl;
	heightMap = normalizeVector(heightMap);
	int i = 0;
	for (int x = 0; x < gridLength; x++)
	{
		for (int z = 0; z < gridDepth; z++) {
			voxelGrid[PositionToArrayIndex(glm::vec3(x, z, heightMap[i]*5))] = 1;
			//std::cout << " arrIndex: " << PositionToArrayIndex(glm::vec3(x, heightMap[i] * 5, z)) << '\t';
			//std::cout << "x: " << x << " y: " << heightMap[i] * 5<< " z: " << z << std::endl;
			i++;
		}
	
	}

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

		if (voxelGrid[i] == 1) {
			num++;
			glm::vec3 gridPos = ArrayIndexToPosition(i);

			//std::cout << "gridPos: " << "x: " << gridPos.x << " y: " << gridPos.y << " z: " << gridPos.z << std::endl;

			if (i - 1 < gridLength * gridHeight * gridDepth) {

				if (voxelGrid[i - 1] == 0) {
					DrawCubeSideFront(gridPos,offset);
				}
			}
			if (i + 1 < gridLength * gridHeight * gridDepth) {
				if (voxelGrid[i + 1] == 0) {
					DrawCubeSideBack(gridPos, offset);
				}
			}

			if (i - gridLength < gridLength * gridHeight * gridDepth) {


				if (i - gridLength < 0) {
					DrawCubeSideTop(gridPos, offset);
					continue;
				}

				if (voxelGrid[i - gridLength] == 0) {
					DrawCubeSideTop(gridPos, offset);
				}
			}
			if (i + gridLength < gridLength * gridHeight * gridDepth) {


				if (i + gridLength < 0) {
					DrawCubeSideBottom(gridPos, offset);
					continue;
				}

				if (voxelGrid[i + gridLength] == 0) {
					DrawCubeSideBottom(gridPos, offset);						
				}
			}

			if ((i - gridLength * gridHeight< gridLength * gridHeight * gridDepth) ) {

				if (i - gridLength * gridHeight< 0) {
					DrawCubeSideLeft(gridPos, offset);
					continue;
				}

				if (voxelGrid[i - gridLength * gridHeight] == 0) {
					
					DrawCubeSideLeft(gridPos, offset);
				}
			}
			if (i + gridLength * gridHeight< gridLength * gridHeight * gridDepth) {

				if (i + gridLength * gridHeight < 0) {
					DrawCubeSideRight(gridPos, offset);
					continue;
				}

				if (voxelGrid[i + gridLength * gridHeight] == 0) {

					DrawCubeSideRight(gridPos, offset);
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

void VoxelMesh::DrawCubeSideLeft(glm::vec3 gridPos, uint32_t& offset)
{
	auto sideData = cube.getSideAsVertexArray(cube.LEFT, gridPos);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideRight(glm::vec3 gridPos, uint32_t& offset)
{
	auto sideData = cube.getSideAsVertexArray(cube.RIGHT, gridPos);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideFront(glm::vec3 gridPos, uint32_t& offset)
{
	auto sideData = cube.getSideAsVertexArray(cube.FRONT, gridPos);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBack(glm::vec3 gridPos, uint32_t& offset)
{
	auto sideData = cube.getSideAsVertexArray(cube.BACK, gridPos);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBottom(glm::vec3 gridPos, uint32_t& offset)
{
	auto sideData = cube.getSideAsVertexArray(cube.BOTTOM, gridPos);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideTop(glm::vec3 gridPos, uint32_t& offset)
{
	auto sideData = cube.getSideAsVertexArray(cube.TOP, gridPos);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

int32_t VoxelMesh::PositionToArrayIndex(glm::vec3 position) {
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

	int32_t val = arrayIndex;
	int32_t x = val / (gridLength * gridHeight);
	val -= (x * (gridLength * gridHeight));
	int32_t y = val / gridLength;
	val -= (y * (gridHeight));
	int32_t z = val;

	return glm::vec3(x,y,z);
}