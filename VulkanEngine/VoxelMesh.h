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

#define VOXEL_GRID_LENGTH 50
#define VOXEL_GRID_HEIGHT 50
#define VOXEL_GRID_DEPTH 50
#define VOXEL_BOX_DIM_SIZE 1.0


class VoxelMesh
{
private:
    uint32_t gridLength, gridHeight, gridDepth;
    std::vector<uint8_t> voxelGrid;
    std::vector<Vertex> voxelDrawSides;
    std::vector<uint32_t> indiceDrawSides;
    void DrawCubeSideLeft(glm::vec3 gridPos, uint32_t& offset, uint8_t type);
    void DrawCubeSideRight(glm::vec3 gridPos, uint32_t& offset, uint8_t type);
    void DrawCubeSideFront(glm::vec3 gridPos, uint32_t& offset, uint8_t type);
    void DrawCubeSideBack(glm::vec3 gridPos, uint32_t& offset, uint8_t type);
    void DrawCubeSideBottom(glm::vec3 gridPos, uint32_t& offset, uint8_t type);
    void DrawCubeSideTop(glm::vec3 gridPos, uint32_t& offset, uint8_t type);
    int32_t PositionToArrayIndex(glm::vec3 position);
    glm::vec3 ArrayIndexToPosition(int32_t arrayIndex);


    int vertexBufferSize = 0;
    int indexBufferSize = 0;
    
public:

    VoxelMesh();

    uint32_t GetGridLength();
    uint32_t GetGridHeigth();
    uint32_t GetGridDepth();

	void LoadVoxelMesh();
    void UpdateVoxelMesh();
    void ChangeVoxelAtIndex(uint32_t index, uint8_t val);
    int32_t ChangeVoxelAtIndex(glm::vec3 indexVector, uint8_t val);
    uint8_t GetVoxelAtIndex(glm::vec3 indexVector);
    void AddCubeToCubeSide(glm::vec3 resIndex, uint8_t cubeSide);
};