#pragma once

#include "Defs.h"
#include <array>
#include <map>
#include <algorithm>
#include "Buffers/Buffers.h"
#include "ModelManager.h"
#include "GameObjectManager.h"
#include "../PerlinNoise/PerlinNoise.h"
#include "Renderer.h"
#include "CubeData.h"

#define VOXEL_GRID_LENGTH 1500//(257 - 1)*2
#define VOXEL_GRID_HEIGHT 25
#define VOXEL_GRID_DEPTH 1500//(257- 1)*2 
#define VOXEL_BOX_DIM_SIZE 1.0
#define VOXEL_GRID_SIZE VOXEL_GRID_LENGTH * VOXEL_GRID_HEIGHT * VOXEL_GRID_DEPTH


class VoxelMesh
{
private:
    size_t gridLength, gridHeight, gridDepth;
    std::array<uint8_t, VOXEL_GRID_SIZE> voxelGrid;
    std::vector<Vertex> voxelDrawSides;
    std::vector<uint32_t> indiceDrawSides;
    void DrawCubeSideLeft(glm::vec3 gridPos, size_t& offset, uint8_t type);
    void DrawCubeSideRight(glm::vec3 gridPos, size_t& offset, uint8_t type);
    void DrawCubeSideFront(glm::vec3 gridPos, size_t& offset, uint8_t type);
    void DrawCubeSideBack(glm::vec3 gridPos, size_t& offset, uint8_t type);
    void DrawCubeSideBottom(glm::vec3 gridPos, size_t& offset, uint8_t type);
    void DrawCubeSideTop(glm::vec3 gridPos, size_t& offset, uint8_t type);
    size_t PositionToArrayIndex(glm::vec3 position);
    glm::vec3 ArrayIndexToPosition(int32_t arrayIndex);


    int vertexBufferSize = 0;
    int indexBufferSize = 0;
    
public:

    VoxelMesh();

    size_t GetGridLength();
    size_t GetGridHeigth();
    size_t GetGridDepth();

	void LoadVoxelMesh();
    void UpdateVoxelMesh();
    void ChangeVoxelAtIndex(size_t index, uint8_t val);
    int32_t ChangeVoxelAtIndex(glm::vec3 indexVector, uint8_t val);
    size_t GetVoxelAtIndex(glm::vec3 indexVector);
    void AddCubeToCubeSide(glm::vec3 resIndex, uint8_t cubeSide);
};