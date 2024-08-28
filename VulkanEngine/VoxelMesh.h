#pragma once

#include "Includes.h"
#include <array>
#include <map>
#include <algorithm>
#include "Buffers/Buffers.h"
#include "ModelManager.h"
#include "GameObjectManager.h"
#include "../PerlinNoise/PerlinNoise.h"
#include "Renderer.h"

#define VOXEL_GRID_LENGTH 5
#define VOXEL_GRID_HEIGHT 5
#define VOXEL_GRID_DEPTH 5
#define VOXEL_BOX_DIM_SIZE 1.0

struct Cube {

    enum CubeSides {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    std::array<glm::vec3, 8> cube = { {
        {glm::vec3(0, 0, 0)},  // Punkt 1
        {glm::vec3(1, 0, 0)},  // Punkt 2
        {glm::vec3(1, 1, 0)},  // Punkt 3
        {glm::vec3(0, 1, 0)},  // Punkt 4
        {glm::vec3(0, 0, 1)},  // Punkt 5
        {glm::vec3(1, 0, 1)},  // Punkt 6
        {glm::vec3(1, 1, 1)},  // Punkt 7
        {glm::vec3(0, 1, 1)}   // Punkt 8
    } };

    std::map<CubeSides, std::array<glm::vec3, 4>> cubeSides = {
        {FRONT, {cube[0], cube[1], cube[2], cube[3]}},
        {BACK, {cube[4], cube[5], cube[6], cube[7]}},
        {LEFT, {cube[0], cube[4], cube[7], cube[3]}},
        {RIGHT, {cube[1], cube[5], cube[6], cube[2]}},
        {TOP, {cube[4], cube[5], cube[1], cube[0]}},
        {BOTTOM, {cube[7], cube[6], cube[2], cube[3]}}
    };

    std::map<CubeSides, std::array<glm::vec2, 4>> cubeSideTexCoords = {
        {FRONT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {BACK, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {LEFT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {RIGHT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {TOP, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {BOTTOM, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }}
    };

    std::map<CubeSides, std::array<std::array<int, 3>, 2>> cubeSideIndices = {
        {TOP, {{{0, 1, 2}, {2, 3, 0}}}},
        {BOTTOM, {{{4, 5, 6}, {6, 7, 4}}}},
        {FRONT, {{{0, 1, 5}, {5, 4, 0}}}},
        {BACK, {{{3, 2, 6}, {6, 7, 3}}}},
        {LEFT, {{{0, 3, 7}, {7, 4, 0}}}},
        {RIGHT, {{{1, 2, 6}, {6, 5, 1}}}}
    };



    std::vector<glm::vec3> getSide(CubeSides side, glm::vec3 position) {
        std::vector<glm::vec3> sideCoordinates;
        for (const auto& vertices : cubeSides[side]) {
            sideCoordinates.push_back(vertices + position);
        }
        return sideCoordinates;
    }

    std::vector<glm::vec2> getTexCoords(CubeSides side) {
        std::vector<glm::vec2> texCoords;
        for (const auto& texCoord : cubeSideTexCoords[side]) {
            texCoords.push_back(texCoord);
        }
        return texCoords;
    }

    std::vector<Vertex> getSideAsVertexArray(CubeSides side, glm::vec3 position, uint8_t type) {
        std::vector<Vertex> cubeArr;

        auto sideVertArr = getSide(side, position);
        auto size = sideVertArr.size();
        auto sideTexArr = getTexCoords(side);

        for (int i = 0; i < size; i++ ) {
            
            cubeArr.push_back(Vertex());

            cubeArr.back().pos = sideVertArr[i];
            cubeArr.back().color = glm::vec3(1, 0, 0);
            cubeArr.back().texCoord = sideTexArr[i];

            cubeArr.back().texId = type;
        }

        return cubeArr;
        
    }

    std::vector<uint32_t> getSideIndices(CubeSides side) {
        std::vector<uint32_t> sideIndices;
        for (const auto& indexArr : cubeSideIndices[side]) {
            for (const auto& indice : indexArr)
            {
                sideIndices.push_back(indice);
            }
            
        }
        return sideIndices;
    }

};

class VoxelMesh
{
private:
    uint32_t gridLength, gridHeight, gridDepth;
    std::vector<uint8_t> voxelGrid;
    Cube cube;
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
};