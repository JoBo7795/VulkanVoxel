#version 450

#define NUM_LIGHTS 3

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;

    vec4 lightPos[NUM_LIGHTS];
    vec4 viewPos[NUM_LIGHTS];
    vec4 lightColor[NUM_LIGHTS];


} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in int inTexIndex;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out int fragTexIndex;

layout(location = 4) out vec3 fragPos;

layout(location = 5) out vec3 fragLightPos[3];
layout(location = 8) out vec3 fragViewPos[3];
layout(location = 11) out vec3 fragLightColor[3];
layout(location = 15) out int LightCount;

void main() {

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = inNormal;
    fragTexIndex = inTexIndex;
    fragPos = vec3(ubo.model * vec4(inPosition, 1.0));

    for(int i = 0; i < NUM_LIGHTS; i++){
        fragLightPos[i] = ubo.lightPos[i].xyz;
        fragViewPos[i] = ubo.viewPos[i].xyz;
        fragLightColor[i] = ubo.lightColor[i].xyz;
    }

    LightCount = NUM_LIGHTS;
}