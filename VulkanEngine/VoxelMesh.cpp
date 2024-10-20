#include "VoxelMesh.h"


VoxelMesh::VoxelMesh() {

	gridLength = VOXEL_GRID_LENGTH;
	gridHeight = VOXEL_GRID_HEIGHT;
	gridDepth = VOXEL_GRID_DEPTH;


	const siv::PerlinNoise::seed_type seed = 78394279843u;
	const siv::PerlinNoise perlin{ seed };
	std::array<float, CHUNK_LENGTH * CHUNK_DEPTH> heightMap;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 2);

	ChunkManager* chunkManagerRef = ChunkManager::GetInstance();


	for (int chunkX = 0; chunkX < chunkManagerRef->GetLength(); chunkX++) {
	
			for (int chunkZ = 0; chunkZ < chunkManagerRef->GetDepth(); chunkZ++) {	

				Chunk chunkGen = Chunk(glm::vec3(chunkX, 0, chunkZ), CHUNK_SIZE, CHUNK_LENGTH, CHUNK_DEPTH, CHUNK_HEIGHT);
				glm::vec3 chunkPosition = chunkManagerRef->AppendChunkToChunkArr(chunkGen);
				Chunk& chunk = chunkManagerRef->GetChunkFromChunkArr(chunkPosition);

				int num = 0;
				float frequency = 0.005, octaves = 2, persistence = 0.05;

				for (size_t x = 0; x < CHUNK_LENGTH; x++) {
					for (size_t z = 0; z < CHUNK_DEPTH; z++) {
						
						float inputX = (x + chunkPosition.x * chunk.length) * frequency;
						float inputZ = (z + chunkPosition.z * chunk.depth) * frequency;
				
						float val = perlin.octave2D_01(inputX, inputZ, octaves, persistence);
				
						int perlinY = round(val * ((CHUNK_HEIGHT / 1.3) - 1));
				
						uint8_t type = dis(gen);
						heightMap[num] = perlinY;
				
				
						
						for (int i = 0; i < heightMap[num]; i++) {
							chunk.voxelGrid[PositionToArrayIndex(chunk, glm::vec3(x, i, z))] = type;
						}
						num++;
					}
				}
				
				
				num = 0;
				frequency = 0.0025, octaves = 6, persistence = 0.5;
				
				for (size_t x = 0; x < CHUNK_LENGTH; x++) {
					for (size_t z = 0; z < CHUNK_DEPTH; z++) {
						float inputX = (x + chunkPosition.x * chunk.length) * frequency;
						float inputZ = (z + chunkPosition.z * chunk.depth) * frequency;
				
						float val = perlin.octave2D_01(inputX, inputZ, octaves, persistence);
						int perlinY = round(val * ((CHUNK_HEIGHT / 1.3) - 1));
				
						uint8_t type = dis(gen);
						heightMap[num] = perlinY;
				
						
						for (int i = 0; i < heightMap[num]; i++) {
							chunk.voxelGrid[PositionToArrayIndex(chunk, glm::vec3(x, i, z))] = type;
						}
						num++;
					}
				}
	
			}
		
	}	

	//std::cout << "num loops: "<<num << "heightMapSize: " << heightMap.size() << "maximum Size: " << gridDepth * gridHeight * gridLength << std::endl;

	auto cam = Renderer::GetInstance()->GetCamera();
	cam.SetPosition(glm::vec3(gridLength / 2, gridHeight / 2, gridDepth / 2));
	Renderer::GetInstance()->SetCamera(cam);


}

size_t VoxelMesh::GetGridLength() {
	return gridLength;
}

size_t VoxelMesh::GetGridHeigth() {
	return gridHeight;
}

size_t VoxelMesh::GetGridDepth() {
	return gridDepth;
}

void VoxelMesh::LoadVoxelMesh() {

	std::cout << "create Voxel Mesh" << std::endl;
	
	Model model;
	GameObject gameObject;
	ChunkManager* chunkManagerRef = ChunkManager::GetInstance();
	
	int num = 0, numSkip = 0, chunkNum = 0, chunkArrSize = chunkManagerRef->GetChunkArrSize();

	for (size_t chunkCum = 0; chunkNum < chunkArrSize; chunkNum++) {

		Chunk& chunk = chunkManagerRef->GetChunkFromChunkArr(chunkNum);
		glm::vec3 chunkPosition = (chunkManagerRef->ChunkIdToPosition(chunkManagerRef->ChunkPositionToId(chunk.position)));
		size_t chunkId = chunkManagerRef->ChunkPositionToId(chunk.position);
		size_t offset = 0;

		for (size_t i = 0; i < chunk.voxelCount; i++) {

			if (chunk.voxelGrid[i] > 0) {

				num++;		

				glm::vec3 chunkGridPos = ChunkArrayIndexToRoomPosition(chunk,i);

				glm::vec3 relGridPos = ChunkArrayIndexToPosition(chunk, i);

				if (relGridPos.x - 1 >= 0 ) {
					glm::vec3 checkPos = chunkGridPos;
					checkPos.x--;

					if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideLeft(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else if ((relGridPos.x - 1 < 0) && (chunkPosition.x - 1) >= 0) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.x = CHUNK_LENGTH - 1;
					glm::vec3 neighborChunk = chunkPosition;
					neighborChunk.x--;

					if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideLeft(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else {
				
					numSkip++;
				}

				
				if (relGridPos.x + 1 < chunk.length) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.x += 1;

					if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideRight(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);						
					}
				}
				else if ((relGridPos.x + 1 >= chunk.length )&& (chunkPosition.x + 1) < chunkManagerRef->GetLength()) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.x = 0;
					glm::vec3 neighborChunk = chunkPosition;
					neighborChunk.x++;

					if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideRight(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else {
					numSkip++;
				}

				if (relGridPos.y - 1 >= 0 ) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.y -= 1;

					if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideTop(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);	
						
					}
				}
				else if (relGridPos.y - 1 < 0 && (chunkPosition.y - 1) >= 0) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.y = CHUNK_HEIGHT - 1;
					glm::vec3 neighborChunk = chunkPosition;
					neighborChunk.y --;

					if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideTop(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else {
					numSkip++;
				}
				
				if (relGridPos.y + 1 < chunk.height) {
					glm::vec3 checkPos = chunkGridPos;
					checkPos.y++;

					if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideBottom(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);

					}
				}
				else if (relGridPos.y + 1 >= chunk.length && (chunkPosition.y + 1) < chunkManagerRef->GetHeigth()) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.y = 0;
					glm::vec3 neighborChunk = chunkPosition;
					neighborChunk.y++;

					if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideBottom(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else {
					numSkip++;
				}
				
				if (relGridPos.z - 1 >= 0) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.z -= 1;

					if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						
						DrawCubeSideFront(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
						
					}
				}
				else if (relGridPos.z - 1 < 0 && chunkPosition.z - 1 >= 0) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.z = CHUNK_DEPTH - 1;
					glm::vec3 neighborChunk = chunkPosition;
					neighborChunk.z--;

					if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideFront(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else {
					numSkip++;
				}
				
				if (relGridPos.z + 1 < chunk.depth) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.z += 1;

					if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideBack(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
						
					}
				}
				else if (relGridPos.z + 1 >= chunk.length && (chunkPosition.z + 1) < chunkManagerRef->GetDepth()) {

					glm::vec3 checkPos = chunkGridPos;
					checkPos.z = 0;
					glm::vec3 neighborChunk = chunkPosition;
					neighborChunk.z++;

					if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
						DrawCubeSideBack(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
					}
				}
				else {
					numSkip++;
				}
			}

		}

		//std::cout << "drawn voxel: " << num << std::endl;
		//std::cout << "skipped voxel: " << numSkip << std::endl;

		int sizeInBytes = voxelGrid.size() * sizeof(Vertex);
		int totalMemInBytes = 0;

		//std::cout << "drawn voxel: " << num << "size in byte: " << sizeInBytes << "size in mb: " << static_cast<double>(sizeInBytes) / (1024 * 1024) << std::endl;
		
		totalMemInBytes += sizeInBytes;

		model.vertices = chunk.voxelDrawSides;

		chunk.voxelDrawSides.clear();
		chunk.voxelDrawSides.shrink_to_fit();

		//std::cout << "vertexbuffer: " << voxelDrawSides.size() << "size in byte: " << voxelDrawSides.size() * sizeof(model.vertices[0]) << "size in mb: " << static_cast<double>(voxelDrawSides.size() * sizeof(model.vertices[0])) / (1024 * 1024) << std::endl;

		totalMemInBytes += voxelDrawSides.size() * sizeof(model.vertices[0]);

		model.indices = chunk.indiceDrawSides;
		model.indiceCount = model.indices.size();

		chunk.indiceDrawSides.clear();
		chunk.indiceDrawSides.shrink_to_fit();

		//std::cout << "vertexbuffer: " << indiceDrawSides.size() << "size in byte: " << indiceDrawSides.size() * sizeof(model.vertices[0]) << "size in mb: " << static_cast<double>(indiceDrawSides.size() * sizeof(model.vertices[0])) / (1024 * 1024) << std::endl;
		
		totalMemInBytes += indiceDrawSides.size() * sizeof(model.vertices[0]);

		model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);

		model.vertices.clear();
		model.vertices.shrink_to_fit();

		model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);

		model.indices.clear();
		model.indices.shrink_to_fit();
		
		vertexBufferSize = sizeof(model.vertices[0]) * model.vertices.size();

		//std::cout << "vertexbuffer: " << model.vertices.size() << "size in byte: " << model.vertices.size() * sizeof(model.vertices[0]) << "size in mb: " << static_cast<double>(model.vertices.size() * sizeof(model.vertices[0])) / (1024 * 1024) << std::endl;

		totalMemInBytes += model.vertices.size() * sizeof(model.vertices[0]);



		indexBufferSize = sizeof(model.indices[0]) * model.indices.size();

		//std::cout << "drawn voxel: " << model.indices.size() << "size in byte: " << sizeof(model.indices[0]) * model.indices.size() << "size in mb: " << static_cast<double>(sizeof(model.indices[0]) * model.indices.size()) / (1024 * 1024) << std::endl;
		
		totalMemInBytes += sizeof(model.indices[0]) * model.indices.size();

		//gameObject.modelId = VOXEL_ENV;
		gameObject.modelId = chunkNum;
		gameObject.position = glm::vec3(0.0);
		GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);

		//std::cout <<  " total size in byte: " << totalMemInBytes << " size in mb: " << static_cast<double>(totalMemInBytes) / (1024 * 1024) << std::endl;
		
		ModelManager::GetInstance()->AppendModelToMap(model, chunkNum);

		//std::cout << "chunkNum: " << chunkNum << std::endl;
	}
}

void VoxelMesh::UpdateVoxelMesh(Chunk& chunk) {

	std::cout << "create Voxel Mesh" << std::endl;

	std::vector<Vertex> cubeArr;
	ChunkManager* chunkManagerRef = ChunkManager::GetInstance();
	int size = voxelGrid.size();
	Model& model = ModelManager::GetInstance()->GetModelFromQueue(chunkManagerRef->ChunkPositionToId(chunk.position));

	std::cout << "vert before: " << model.vertices.size();
	std::cout << "indice before: " << model.indices.size();

	voxelDrawSides.clear();
	indiceDrawSides.clear();


	int num = 0, chunkArrSize = chunkManagerRef->GetChunkArrSize();

	glm::vec3 chunkPosition = (chunkManagerRef->ChunkIdToPosition(chunkManagerRef->ChunkPositionToId(chunk.position)));
	size_t chunkId = chunkManagerRef->ChunkPositionToId(chunk.position);
	size_t offset = 0;

	for (size_t i = 0; i < chunk.voxelCount; i++) {

		if (chunk.voxelGrid[i] > 0) {

			num++;

			glm::vec3 chunkGridPos = ChunkArrayIndexToRoomPosition(chunk, i);

			glm::vec3 relGridPos = ChunkArrayIndexToPosition(chunk, i);

			if (relGridPos.x - 1 >= 0) {
				glm::vec3 checkPos = chunkGridPos;
				checkPos.x--;

				if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideLeft(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}
			else if ((relGridPos.x - 1 < 0) && (chunkPosition.x - 1) >= 0) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.x = CHUNK_LENGTH - 1;
				glm::vec3 neighborChunk = chunkPosition;
				neighborChunk.x--;

				if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideLeft(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}



			if (relGridPos.x + 1 < chunk.length) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.x += 1;

				if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideRight(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}
			else if ((relGridPos.x + 1 >= chunk.length) && (chunkPosition.x + 1) < chunkManagerRef->GetLength()) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.x = 0;
				glm::vec3 neighborChunk = chunkPosition;
				neighborChunk.x++;

				if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideRight(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}


			if (relGridPos.y - 1 >= 0) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.y -= 1;

				if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideTop(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);

				}
			}
			else if (relGridPos.y - 1 < 0 && (chunkPosition.y - 1) >= 0) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.y = CHUNK_HEIGHT - 1;
				glm::vec3 neighborChunk = chunkPosition;
				neighborChunk.y--;

				if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideTop(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}


			if (relGridPos.y + 1 < chunk.height) {
				glm::vec3 checkPos = chunkGridPos;
				checkPos.y++;

				if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideBottom(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);

				}
			}
			else if (relGridPos.y + 1 >= chunk.length && (chunkPosition.y + 1) < chunkManagerRef->GetHeigth()) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.y = 0;
				glm::vec3 neighborChunk = chunkPosition;
				neighborChunk.y++;

				if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideBottom(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}


			if (relGridPos.z - 1 >= 0) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.z -= 1;

				if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {

					DrawCubeSideFront(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);

				}
			}
			else if (relGridPos.z - 1 < 0 && chunkPosition.z - 1 >= 0) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.z = CHUNK_DEPTH - 1;
				glm::vec3 neighborChunk = chunkPosition;
				neighborChunk.z--;

				if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideFront(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}


			if (relGridPos.z + 1 < chunk.depth) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.z += 1;

				if (chunk.voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideBack(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);

				}
			}
			else if (relGridPos.z + 1 >= chunk.length && (chunkPosition.z + 1) < chunkManagerRef->GetDepth()) {

				glm::vec3 checkPos = chunkGridPos;
				checkPos.z = 0;
				glm::vec3 neighborChunk = chunkPosition;
				neighborChunk.z++;

				if (chunkManagerRef->GetChunkFromChunkArr(chunkManagerRef->ChunkPositionToId(neighborChunk)).voxelGrid[RelativeChunkPositionToArrayIndex(chunk, checkPos)] == 0) {
					DrawCubeSideBack(chunk, chunkGridPos, offset, chunk.voxelGrid[i]);
				}
			}

		}

	}
	

	int sizeInBytes = chunk.voxelGrid.size() * sizeof(Vertex);

	std::cout << "drawn voxel: " << num << "size in byte: " << sizeInBytes << "size in mb: " << static_cast<double>(sizeInBytes) / (1024 * 1024) << std::endl;

	std::cout << "vert after: " << voxelDrawSides.size() << std::endl;
	std::cout << "indice after: " << indiceDrawSides.size() << std::endl;

	// TODO chunk muss korrekt geupdated werden


	int totalMemInBytes = 0;

	totalMemInBytes += sizeInBytes;

	model.vertices = chunk.voxelDrawSides;

	chunk.voxelDrawSides.clear();
	chunk.voxelDrawSides.shrink_to_fit();

	//std::cout << "vertexbuffer: " << voxelDrawSides.size() << "size in byte: " << voxelDrawSides.size() * sizeof(model.vertices[0]) << "size in mb: " << static_cast<double>(voxelDrawSides.size() * sizeof(model.vertices[0])) / (1024 * 1024) << std::endl;

	totalMemInBytes += voxelDrawSides.size() * sizeof(model.vertices[0]);

	model.indices = chunk.indiceDrawSides;
	model.indiceCount = model.indices.size();

	chunk.indiceDrawSides.clear();
	chunk.indiceDrawSides.shrink_to_fit();

	//std::cout << "vertexbuffer: " << indiceDrawSides.size() << "size in byte: " << indiceDrawSides.size() * sizeof(model.vertices[0]) << "size in mb: " << static_cast<double>(indiceDrawSides.size() * sizeof(model.vertices[0])) / (1024 * 1024) << std::endl;

	totalMemInBytes += indiceDrawSides.size() * sizeof(model.vertices[0]);

	model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);

	model.vertices.clear();
	model.vertices.shrink_to_fit();

	model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);

	model.indices.clear();
	model.indices.shrink_to_fit();

	vertexBufferSize = sizeof(model.vertices[0]) * model.vertices.size();

	//std::cout << "vertexbuffer: " << model.vertices.size() << "size in byte: " << model.vertices.size() * sizeof(model.vertices[0]) << "size in mb: " << static_cast<double>(model.vertices.size() * sizeof(model.vertices[0])) / (1024 * 1024) << std::endl;

	totalMemInBytes += model.vertices.size() * sizeof(model.vertices[0]);



	indexBufferSize = sizeof(model.indices[0]) * model.indices.size();

	//std::cout << "drawn voxel: " << model.indices.size() << "size in byte: " << sizeof(model.indices[0]) * model.indices.size() << "size in mb: " << static_cast<double>(sizeof(model.indices[0]) * model.indices.size()) / (1024 * 1024) << std::endl;

	totalMemInBytes += sizeof(model.indices[0]) * model.indices.size();


	model.vertices = voxelDrawSides;
	model.indices = indiceDrawSides;

}

void VoxelMesh::DrawCubeSideLeft(glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::LEFT, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideRight(glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::RIGHT, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideFront(glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::FRONT, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBack(glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::BACK, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBottom(glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::BOTTOM, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideTop(glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::TOP, gridPos,type);
	voxelDrawSides.insert(voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	indiceDrawSides.insert(indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}





void VoxelMesh::DrawCubeSideLeft(Chunk& chunk, glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::LEFT, gridPos, type);
	chunk.voxelDrawSides.insert(chunk.voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	chunk.indiceDrawSides.insert(chunk.indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideRight(Chunk& chunk, glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::RIGHT, gridPos, type);
	chunk.voxelDrawSides.insert(chunk.voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	chunk.indiceDrawSides.insert(chunk.indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideFront(Chunk& chunk, glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::FRONT, gridPos, type);
	chunk.voxelDrawSides.insert(chunk.voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	chunk.indiceDrawSides.insert(chunk.indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBack(Chunk& chunk, glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::BACK, gridPos, type);
	chunk.voxelDrawSides.insert(chunk.voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	chunk.indiceDrawSides.insert(chunk.indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideBottom(Chunk& chunk, glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::BOTTOM, gridPos, type);
	chunk.voxelDrawSides.insert(chunk.voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, offset };
	chunk.indiceDrawSides.insert(chunk.indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}

void VoxelMesh::DrawCubeSideTop(Chunk& chunk, glm::vec3 gridPos, size_t& offset, uint8_t type)
{
	auto sideData = Cube::getSideAsVertexArray(Cube::TOP, gridPos, type);
	chunk.voxelDrawSides.insert(chunk.voxelDrawSides.end(), sideData.begin(), sideData.end());
	auto indiceData = { offset, 3 + offset, 2 + offset, 2 + offset, 1 + offset, offset };
	chunk.indiceDrawSides.insert(chunk.indiceDrawSides.end(), indiceData.begin(), indiceData.end());
	offset += sideData.size();
}




size_t VoxelMesh::PositionToArrayIndex(glm::vec3 position) {

	return ((size_t)position.x * gridDepth * gridHeight) + ((size_t)position.y * gridDepth) + (size_t)position.z;

}

size_t VoxelMesh::PositionToArrayIndex(Chunk& chunk, glm::vec3 position) {

	return ((size_t)position.x * chunk.depth * chunk.height) + ((size_t)position.y * chunk.depth) + (size_t)position.z;

}

size_t VoxelMesh::RelativeChunkPositionToArrayIndex(Chunk& chunk, glm::vec3 position) {

	return ((size_t)((size_t)position.x %chunk.length ) * chunk.depth * chunk.height)
		+ ((size_t)((size_t)position.y % chunk.height ) * chunk.depth)
		+ (size_t)((size_t)position.z % chunk.depth);

}

// 3D Room position of voxel
glm::vec3 VoxelMesh::ChunkArrayIndexToRoomPosition(Chunk& chunk, int32_t arrayIndex) {

	int32_t x = arrayIndex / (chunk.depth * chunk.height);
	arrayIndex -= (x * chunk.depth * chunk.height);
	int32_t y = arrayIndex / chunk.depth;
	arrayIndex -= (y * chunk.depth);
	int32_t z = arrayIndex;

	return glm::vec3(chunk.position.x*chunk.length + x, chunk.position.y * chunk.height + y, chunk.position.z * chunk.depth + z);
}

// Position relative to chunk grid
glm::vec3 VoxelMesh::ChunkArrayIndexToPosition(Chunk& chunk, int32_t arrayIndex) {

	int32_t x = arrayIndex / (chunk.depth * chunk.height);
	arrayIndex -= (x * chunk.depth * chunk.height);
	int32_t y = arrayIndex / chunk.depth;
	arrayIndex -= (y * chunk.depth);
	int32_t z = arrayIndex;

	return glm::vec3( x, y, z);
}

glm::vec3 VoxelMesh::ArrayIndexToPosition(int32_t arrayIndex) {

	int32_t x = arrayIndex / (gridDepth * gridHeight);
	arrayIndex -= (x * gridDepth * gridHeight);
	int32_t y = arrayIndex / gridDepth;
	arrayIndex -= (y * gridDepth);
	int32_t z = arrayIndex;

	return glm::vec3(x, y, z);
}

void VoxelMesh::ChangeVoxelAtIndex(size_t index, uint8_t val) {

	voxelGrid[index] = val;
}

size_t VoxelMesh::ChangeVoxelAtIndex(glm::vec3 indexVector, uint8_t val) {

	voxelGrid[PositionToArrayIndex(indexVector)] = val;

	Model& model = ModelManager::GetInstance()->GetModelFromQueue(VOXEL_ENV);

	auto dat = (void*)&voxelGrid[PositionToArrayIndex(indexVector)];

	//UpdateVoxelMesh();

	auto bmanagerRef = BufferManager::GetInstance();

	if (vertexBufferSize < sizeof(model.vertices[0]) * model.vertices.size()) {
		bmanagerRef->DeleteVertexBuffer(model.verticeBufferId);
		model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices, sizeof(model.vertices[0]) * model.vertices.size());
		vertexBufferSize = sizeof(model.vertices[0]) * model.vertices.size();
		std::cout << "Create New VertexBuffer with size: " << vertexBufferSize << std::endl;
	}
	else if(model.vertices.size() > 0){
		BufferManager::GetInstance()->UpdateVertexBuffer(model.verticeBufferId, (void*)&model.vertices[0], sizeof(model.vertices[0]) * model.vertices.size());
		std::cout << "Update VertexBuffer" << std::endl;
	}

	if (indexBufferSize < sizeof(model.indices[0]) * model.indices.size()) {
		bmanagerRef->DeleteIndexBuffer(model.indexBufferId);
		model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices, sizeof(model.indices[0]) * model.indices.size());
		indexBufferSize = sizeof(model.indices[0]) * model.indices.size();
		std::cout << "Create New IndexBuffer with size: " << indexBufferSize << std::endl;
	}
	else if(model.indices.size() > 0) {
		BufferManager::GetInstance()->UpdateIndexBuffer(model.indexBufferId, (void*)&model.indices[0], sizeof(model.indices[0]) * model.indices.size());
		std::cout << "Update IndexBuffer with size: " << sizeof(model.indices[0])* model.indices.size() << std::endl;
	}

	return PositionToArrayIndex(indexVector);
}



size_t VoxelMesh::ChangeVoxelAtIndex(Chunk& chunk, glm::vec3 indexVector, uint8_t val) {

	chunk.voxelGrid[PositionToArrayIndex(chunk, indexVector)] = val;

	Model& model = ModelManager::GetInstance()->GetModelFromQueue(ChunkManager::GetInstance()->ChunkPositionToId(chunk.position));

	auto dat = (void*)&voxelGrid[PositionToArrayIndex(indexVector)];

	UpdateVoxelMesh(chunk);

	auto bmanagerRef = BufferManager::GetInstance();

	if (vertexBufferSize < sizeof(model.vertices[0]) * model.vertices.size()) {
		bmanagerRef->DeleteVertexBuffer(model.verticeBufferId);
		model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices, sizeof(model.vertices[0]) * model.vertices.size());
		vertexBufferSize = sizeof(model.vertices[0]) * model.vertices.size();
		std::cout << "Create New VertexBuffer with size: " << vertexBufferSize << std::endl;
	}
	else if (model.vertices.size() > 0) {
		BufferManager::GetInstance()->UpdateVertexBuffer(model.verticeBufferId, (void*)&model.vertices[0], sizeof(model.vertices[0]) * model.vertices.size());
		std::cout << "Update VertexBuffer" << std::endl;
	}

	if (indexBufferSize < sizeof(model.indices[0]) * model.indices.size()) {
		bmanagerRef->DeleteIndexBuffer(model.indexBufferId);
		model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices, sizeof(model.indices[0]) * model.indices.size());
		indexBufferSize = sizeof(model.indices[0]) * model.indices.size();
		std::cout << "Create New IndexBuffer with size: " << indexBufferSize << std::endl;
	}
	else if (model.indices.size() > 0) {
		BufferManager::GetInstance()->UpdateIndexBuffer(model.indexBufferId, (void*)&model.indices[0], sizeof(model.indices[0]) * model.indices.size());
		std::cout << "Update IndexBuffer with size: " << sizeof(model.indices[0]) * model.indices.size() << std::endl;
	}

	return PositionToArrayIndex(indexVector);
}

size_t VoxelMesh::GetVoxelAtIndex(Chunk& chunk, glm::vec3 indexVector) {
	return chunk.voxelGrid[PositionToArrayIndex(chunk, indexVector)];
}

void VoxelMesh::AddCubeToCubeSide(glm::vec3 resIndex, uint8_t cubeSide) {


		switch (cubeSide)
		{

		case TOP:

			resIndex.y += 1;
			ChangeVoxelAtIndex(resIndex, 1);
			std::cout << "TOP" << std::endl;
			break;

		case BOTTOM:
			resIndex.y -= 1;
			ChangeVoxelAtIndex(resIndex, 1);
			std::cout << "BOTTOM" << std::endl;
			break;

		case FRONT:
			resIndex.z -= 1;
			ChangeVoxelAtIndex(resIndex, 1);
			std::cout << "FRONT" << std::endl;
			break;

		case BACK:

			resIndex.z += 1;
			ChangeVoxelAtIndex(resIndex, 1);
			std::cout << "BACK" << std::endl;
			break;

		case RIGHT:

			resIndex.x -= 1;
			ChangeVoxelAtIndex(resIndex, 1);
			std::cout << "RIGHT" << std::endl;
			break;

		case LEFT:
			resIndex.x += 1;
			ChangeVoxelAtIndex(resIndex, 1);
			std::cout << "LEFT" << std::endl;
			break;

		default:
			break;
		}

}

void VoxelMesh::AddCubeToCubeSide(Chunk& chunk, glm::vec3 resIndex, uint8_t cubeSide) {


	switch (cubeSide)
	{

	case TOP:

		resIndex.y += 1;
		ChangeVoxelAtIndex(chunk, resIndex, 1);
		std::cout << "TOP" << std::endl;
		break;

	case BOTTOM:
		resIndex.y -= 1;
		ChangeVoxelAtIndex(chunk, resIndex, 1);
		std::cout << "BOTTOM" << std::endl;
		break;

	case FRONT:
		resIndex.z -= 1;
		ChangeVoxelAtIndex(chunk, resIndex, 1);
		std::cout << "FRONT" << std::endl;
		break;

	case BACK:

		resIndex.z += 1;
		ChangeVoxelAtIndex(chunk, resIndex, 1);
		std::cout << "BACK" << std::endl;
		break;

	case RIGHT:

		resIndex.x -= 1;
		ChangeVoxelAtIndex(chunk, resIndex, 1);
		std::cout << "RIGHT" << std::endl;
		break;

	case LEFT:
		resIndex.x += 1;
		ChangeVoxelAtIndex(chunk, resIndex, 1);
		std::cout << "LEFT" << std::endl;
		break;

	default:
		break;
	}

}