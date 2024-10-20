#include "ChunkManager.h"

ChunkManager* ChunkManager::instance = nullptr;


ChunkManager::ChunkManager()
{

	length = 1;
	height = 1;
	depth = 1;

}

ChunkManager::~ChunkManager() {
	delete instance;
	std::cout << "delete ChunkManager instance" << std::endl;
}

ChunkManager* ChunkManager::GetInstance() {

	if (instance == nullptr) {
		instance = new ChunkManager();
	}

	return instance;
}

glm::vec3 ChunkManager::AppendChunkToChunkArr(Chunk& chunk) {
	chunkArr.reserve(1);
	chunkArr.push_back(chunk);
	return ChunkIdToPosition((chunkArr.size()-1));
}

Chunk& ChunkManager::GetChunkFromChunkArr(size_t chunkId) {
	return chunkArr[chunkId];
}

Chunk& ChunkManager::GetChunkFromChunkArr(glm::vec3 chunkPosition)
{
	return chunkArr[ChunkPositionToId(chunkPosition)];
}

size_t ChunkManager::GetChunkArrSize()
{
	return chunkArr.size();
}

size_t ChunkManager::GetLength()
{
	return length;
}

size_t ChunkManager::GetHeigth()
{
	return height;
}

size_t ChunkManager::GetDepth()
{
	return depth;
}

void ChunkManager::SetLength(size_t in_length)
{
	length = in_length;
}

void ChunkManager::SetHeigth(size_t in_height)
{
	height = in_height;
}

void ChunkManager::SetDepth(size_t in_depth)
{
	depth = in_depth;
}

glm::vec3 ChunkManager::ChunkIdToPosition(size_t chunkId)
{
	int32_t x = chunkId / (depth * height);
	chunkId -= (x * depth * height);
	int32_t y = chunkId / depth;
	chunkId -= (y * depth);
	int32_t z = chunkId;

	return glm::vec3(x, y, z);
}

size_t ChunkManager::ChunkPositionToId(glm::vec3 chunkPosition)
{	
	return ((size_t)chunkPosition.x * depth * height) + ((size_t)chunkPosition.y * depth) + (size_t)chunkPosition.z;
}



