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

const int MAX_FRAMES_IN_FLIGHT = 1;



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
    glm::vec3 pos;      // 12 bytes
    glm::vec2 texCoord; // 8 bytes
    uint8_t cubeSide;// 1 byte
    uint8_t texId;// 1 byte


    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);


        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R8G8_SINT;
        attributeDescriptions[2].offset = offsetof(Vertex, cubeSide);


        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && /*color == other.color && normal == other.normal && */ texCoord == other.texCoord && cubeSide == other.cubeSide && texId == other.texId;
    }
};



struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

    glm::vec4 lightPos[3];
    glm::vec4 viewPos[3];
    glm::vec4 lightColor[3];


};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                //(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1)));
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
