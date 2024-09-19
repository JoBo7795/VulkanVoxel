#pragma once

#include "Includes.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define BUFFER_SIZE 1
#define SCROLL_INPUT 2
#define MOUSE_INPUT 3
#define MOUSE_CLICK 4
#define KEY_INPUT 5

#define MODEL_AFFE 0
#define MODEL_VIKING 1
#define VOXEL_ENV 2
#define MARKER_SPHERE 3
#define DEFAULT_CUBE 4



const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::string MODEL_PATH = "Resources/Meshes/affe.obj";
const std::string VIKING_MODEL_PATH = "Resources/Meshes/viking_room.obj";
const std::string MARKER_SPHERE_MODEL_PATH = "Resources/Meshes/markerSphere.obj";
const std::string DEFAULT_CUBE_MODEL_PATH = "Resources/Meshes/defaultCube.obj";
const std::string TEXTURE_PATH = "Resources/Textures/viking_room.png";
const std::string TEXTURE_PATH2 = "Resources/Textures/ErdeOberfläche.png";

const int MAX_FRAMES_IN_FLIGHT = 2;



const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};



struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    uint8_t texId;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R16_SINT;
        attributeDescriptions[3].offset = offsetof(Vertex, texId);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};



struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}


struct ObjModelData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

};

enum CubeSides {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

// namespace Cube {
// 
//     std::array<glm::vec3, 8> cube = { {
//         {glm::vec3(0, 0, 0)},  // Punkt 1
//         {glm::vec3(1, 0, 0)},  // Punkt 2
//         {glm::vec3(1, 1, 0)},  // Punkt 3
//         {glm::vec3(0, 1, 0)},  // Punkt 4
//         {glm::vec3(0, 0, 1)},  // Punkt 5
//         {glm::vec3(1, 0, 1)},  // Punkt 6
//         {glm::vec3(1, 1, 1)},  // Punkt 7
//         {glm::vec3(0, 1, 1)}   // Punkt 8
//     } };
// 
//     std::map<CubeSides, std::array<glm::vec3, 4>> cubeSides = {
//         {FRONT, {cube[0], cube[1], cube[2], cube[3]}},
//         {BACK, {cube[4], cube[5], cube[6], cube[7]}},
//         {LEFT, {cube[0], cube[4], cube[7], cube[3]}},
//         {RIGHT, {cube[1], cube[5], cube[6], cube[2]}},
//         {TOP, {cube[4], cube[5], cube[1], cube[0]}},
//         {BOTTOM, {cube[7], cube[6], cube[2], cube[3]}}
//     };
// 
//     std::map<CubeSides, std::array<glm::vec2, 4>> cubeSideTexCoords = {
//         {FRONT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
//         {BACK, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
//         {LEFT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
//         {RIGHT, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
//         {TOP, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }},
//         {BOTTOM, { {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1)} }}
//     };
// 
//     std::map<CubeSides, std::array<std::array<int, 3>, 2>> cubeSideIndices = {
//         {TOP, {{{0, 1, 2}, {2, 3, 0}}}},
//         {BOTTOM, {{{4, 5, 6}, {6, 7, 4}}}},
//         {FRONT, {{{0, 1, 5}, {5, 4, 0}}}},
//         {BACK, {{{3, 2, 6}, {6, 7, 3}}}},
//         {LEFT, {{{0, 3, 7}, {7, 4, 0}}}},
//         {RIGHT, {{{1, 2, 6}, {6, 5, 1}}}}
//     };
// 
//     std::map<CubeSides, glm::vec3> cubeNormals = {
//     {FRONT, glm::vec3(0, 0, -1)},
//     {BACK, glm::vec3(0, 0, 1)},
//     {LEFT, glm::vec3(-1, 0, 0)},
//     {RIGHT, glm::vec3(1, 0, 0)},
//     {TOP, glm::vec3(0, 1, 0)},
//     {BOTTOM, glm::vec3(0, -1, 0)}
//     };
// 
//     std::vector<glm::vec3> getSide(CubeSides side, glm::vec3 position) {
//         std::vector<glm::vec3> sideCoordinates;
//         for (const auto& vertices : cubeSides[side]) {
//             sideCoordinates.push_back(vertices + position);
//         }
//         return sideCoordinates;
//     }
// 
//     std::vector<glm::vec2> getTexCoords(CubeSides side) {
//         std::vector<glm::vec2> texCoords;
//         for (const auto& texCoord : cubeSideTexCoords[side]) {
//             texCoords.push_back(texCoord);
//         }
//         return texCoords;
//     }
// 
//     std::vector<Vertex> getSideAsVertexArray(CubeSides side, glm::vec3 position, uint8_t type) {
//         std::vector<Vertex> cubeArr;
// 
//         auto sideVertArr = getSide(side, position);
//         auto size = sideVertArr.size();
//         auto sideTexArr = getTexCoords(side);
// 
//         for (int i = 0; i < size; i++) {
// 
//             cubeArr.push_back(Vertex());
// 
//             cubeArr.back().pos = sideVertArr[i];
//             cubeArr.back().color = glm::vec3(1, 0, 0);
//             cubeArr.back().texCoord = sideTexArr[i];
// 
//             cubeArr.back().texId = type;
//         }
// 
//         return cubeArr;
// 
//     }
// 
//     std::vector<uint32_t> getSideIndices(CubeSides side) {
//         std::vector<uint32_t> sideIndices;
//         for (const auto& indexArr : cubeSideIndices[side]) {
//             for (const auto& indice : indexArr)
//             {
//                 sideIndices.push_back(indice);
//             }
// 
//         }
//         return sideIndices;
//     }
// 
// };