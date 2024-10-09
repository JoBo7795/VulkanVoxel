#version 450

#define NUM_LIGHTS 3

vec3 cubeNormals[6] = {
        vec3(0, 0, -1),
        vec3(0, 0, 1),
        vec3(-1, 0, 0),
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, -1, 0)
};

//Texturekoordinate lookup
const vec2 cubeSideTexCoords[4] = {   

    vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1),
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;

    vec4 lightPos[NUM_LIGHTS];
    vec4 viewPos[NUM_LIGHTS];
    vec4 lightColor[NUM_LIGHTS];

} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 2) in ivec2 cubeSideTexIndex;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoords;
layout(location = 3) out int fragTexIndex;


layout(location = 4) out vec3 fragPos;

layout(location = 5) out vec3 fragLightPos[3];
layout(location = 8) out vec3 fragViewPos[3];
layout(location = 11) out vec3 fragLightColor[3];
layout(location = 15) out int lightCount;

void main() {
    
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    fragColor = vec3(1,0,0);
    fragTexCoords = cubeSideTexCoords[gl_VertexIndex % 4];
    fragNormal = cubeNormals[int(cubeSideTexIndex.x)];
    fragTexIndex = int(cubeSideTexIndex.y);
    fragPos = vec3(ubo.model * vec4(inPosition, 1.0));

    for(int i = 0; i < NUM_LIGHTS; i++){
        fragLightPos[i] = ubo.lightPos[i].xyz;
        fragViewPos[i] = ubo.viewPos[i].xyz;
        fragLightColor[i] = ubo.lightColor[i].xyz;
    }

    lightCount = NUM_LIGHTS;
}