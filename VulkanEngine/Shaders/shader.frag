#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int inTexIndex;
layout(binding = 1) uniform sampler texSampler;

layout(set = 0, binding = 2) uniform texture2D textureArray[2];

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(sampler2D(textureArray[inTexIndex - 1],texSampler), fragTexCoord);


    //if(inTexIndex == 1)
    //    outColor = vec4(1.0, 0.0, 0.0, 1.0);
    //else if(inTexIndex == 2)
    //    outColor = vec4(0.0, float(inTexIndex), 0.0, 1.0);
}