#include "CubeData.h"

namespace Cube {
    const std::array<glm::vec3, 8> cube = { {
        {glm::vec3(0, 0, 0)},  // Punkt 1
        {glm::vec3(1, 0, 0)},  // Punkt 2
        {glm::vec3(1, 1, 0)},  // Punkt 3
        {glm::vec3(0, 1, 0)},  // Punkt 4
        {glm::vec3(0, 0, 1)},  // Punkt 5
        {glm::vec3(1, 0, 1)},  // Punkt 6
        {glm::vec3(1, 1, 1)},  // Punkt 7
        {glm::vec3(0, 1, 1)}   // Punkt 8
    } };

    const std::map<CubeSides, std::array<glm::vec3, 4>> cubeSides = {
        {FRONT, {cube[0], cube[1], cube[2], cube[3]}},
        {BACK, {cube[4], cube[5], cube[6], cube[7]}},
        {LEFT, {cube[0], cube[4], cube[7], cube[3]}},
        {RIGHT, {cube[1], cube[5], cube[6], cube[2]}},
        {TOP, {cube[4], cube[5], cube[1], cube[0]}},
        {BOTTOM, {cube[7], cube[6], cube[2], cube[3]}}
    };

    const std::map<CubeSides, glm::vec3> cubeNormals = {
        {FRONT, glm::vec3(0, 0, -1)},
        {BACK, glm::vec3(0, 0, 1)},
        {LEFT, glm::vec3(-1, 0, 0)},
        {RIGHT, glm::vec3(1, 0, 0)},
        {TOP, glm::vec3(0, 1, 0)},
        {BOTTOM, glm::vec3(0, -1, 0)}
    };

    const std::map<CubeSides, std::array<glm::vec2, 4>> cubeSideTexCoords = {
        {FRONT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {BACK, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {LEFT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {RIGHT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {TOP, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
        {BOTTOM, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }}
    };

    const std::map<CubeSides, std::array<std::array<int, 3>, 2>> cubeSideIndices = {
        {TOP, {{{0, 1, 2}, {2, 3, 0}}}},
        {BOTTOM, {{{4, 5, 6}, {6, 7, 4}}}},
        {FRONT, {{{0, 1, 5}, {5, 4, 0}}}},
        {BACK, {{{3, 2, 6}, {6, 7, 3}}}},
        {LEFT, {{{0, 3, 7}, {7, 4, 0}}}},
        {RIGHT, {{{1, 2, 6}, {6, 5, 1}}}}
    };

    std::vector<glm::vec3> getSide(CubeSides side, glm::vec3 position) {
        std::vector<glm::vec3> sideCoordinates;
        for (const auto& vertices : cubeSides.at(side)) {
            sideCoordinates.push_back(vertices + position);
        }
        return sideCoordinates;
    }

    std::vector<glm::vec2> getTexCoords(CubeSides side) {
        std::vector<glm::vec2> texCoords;
        for (const auto& texCoord : cubeSideTexCoords.at(side)) {
            texCoords.push_back(texCoord);
        }
        return texCoords;
    }

    std::vector<Vertex> getSideAsVertexArray(CubeSides side, glm::vec3 position, uint8_t type) {
        std::vector<Vertex> cubeArr;

        auto sideVertArr = getSide(side, position);
        auto sideTexArr = getTexCoords(side);
        auto size = sideVertArr.size();

        for (int i = 0; i < size; i++) {
            cubeArr.push_back(Vertex());

            cubeArr.back().pos = sideVertArr[i];
            //cubeArr.back().color = glm::vec3(1, 0, 0);
            //cubeArr.back().normal = cubeNormals.at(side);
            cubeArr.back().texCoord = sideTexArr[i];
            cubeArr.back().texId = type;
        }

        return cubeArr;
    }

    std::vector<uint32_t> getSideIndices(CubeSides side) {
        std::vector<uint32_t> sideIndices;
        for (const auto& indexArr : cubeSideIndices.at(side)) {
            for (const auto& indice : indexArr) {
                sideIndices.push_back(indice);
            }
        }
        return sideIndices;
    }
}
