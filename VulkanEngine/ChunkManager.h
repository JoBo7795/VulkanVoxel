#pragma once
#include "Chunk.h"


class ChunkManager
{

private:
	static ChunkManager* instance;
	std::vector<Chunk> chunkArr;

	// defines the number of chunks in every direction
	size_t length, height, depth;

	ChunkManager();
	~ChunkManager();

public:
	static ChunkManager* GetInstance();

	glm::vec3 AppendChunkToChunkArr(Chunk& chunk);
	Chunk& GetChunkFromChunkArr(size_t chunkId);
	Chunk& GetChunkFromChunkArr(glm::vec3 chunkPosition);
	size_t GetChunkArrSize();

	size_t GetLength();
	size_t GetHeigth();
	size_t GetDepth();

	void SetLength(size_t length);
	void SetHeigth(size_t height);
	void SetDepth(size_t depth);

	glm::vec3 ChunkIdToPosition(size_t chunkId);
	size_t ChunkPositionToId(glm::vec3 chunkPosition);


};

