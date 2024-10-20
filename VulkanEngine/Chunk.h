#pragma once

#include "Defs.h"

#define CHUNK_LENGTH 50
#define CHUNK_HEIGHT 30
#define CHUNK_DEPTH 50
#define CHUNK_SIZE CHUNK_LENGTH * CHUNK_HEIGHT * CHUNK_DEPTH

struct Chunk {
    glm::vec3 position;
    size_t voxelCount;
    size_t length, height, depth;
    std::vector<Vertex> voxelDrawSides;
    std::vector<uint32_t> indiceDrawSides;
    std::array<uint8_t, CHUNK_SIZE> voxelGrid;


    Chunk(glm::vec3 pos, size_t vCount, size_t d, size_t l, size_t h)
        : position(pos), voxelCount(vCount), length(l), height(h), depth(d)
    {
        voxelGrid.fill(0);
    }
};