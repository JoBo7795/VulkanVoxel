#ifndef CUBELOOKUP_H
#define CUBELOOKUP_H

#include <array>
#include <map>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Defs.h"

namespace Cube {
    enum CubeSides {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    extern const std::array<glm::vec3, 8> cube;
    extern const std::map<CubeSides, std::array<glm::vec3, 4>> cubeSides;
    extern const std::map<CubeSides, glm::vec3> cubeNormals;
    extern const std::map<CubeSides, std::array<glm::vec2, 4>> cubeSideTexCoords;
    extern const std::map<CubeSides, std::array<std::array<int, 3>, 2>> cubeSideIndices;

    std::vector<glm::vec3> getSide(CubeSides side, glm::vec3 position);
    std::vector<glm::vec2> getTexCoords(CubeSides side);
    std::vector<Vertex> getSideAsVertexArray(CubeSides side, glm::vec3 position, uint8_t type);
    std::vector<uint32_t> getSideIndices(CubeSides side);
}

#endif // CUBELOOKUP_H