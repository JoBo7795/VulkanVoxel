#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) flat in int inTexIndex;

layout(location = 4) in vec3 fragPos;

layout(location = 5) in vec3 fragLightPos[3];
layout(location = 8) in vec3 fragViewPos[3];
layout(location = 11) in vec3 fragLightColor[3];
layout(location = 15) flat in int LightCount;

layout(binding = 1) uniform sampler texSampler;

layout(set = 0, binding = 2) uniform texture2D textureArray[2];

layout(location = 0) out vec4 outColor;

void main() {
    vec4 objectColor = vec4(1.0); // Initialisiere objectColor

    if (inTexIndex > 0 && inTexIndex <= 2) {
        objectColor = texture(sampler2D(textureArray[inTexIndex - 1], texSampler), fragTexCoord);
    }

    if (inTexIndex == 0) {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        outColor = vec4(0.0); // Initialisiere outColor

        for (int i = 0; i < LightCount; i++) {
            // Ambient Lighting
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * fragLightColor[i];

            // Diffuse Lighting
            vec3 norm = normalize(fragNormal);
            vec3 lightDir = normalize(fragLightPos[i] - fragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * fragLightColor[i];

            // Specular Lighting
            float specularStrength = 0.5;
            vec3 viewDir = normalize(fragViewPos[i] - fragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * fragLightColor[i];

            // Kombiniere alle Beleuchtungskomponenten
            outColor += (vec4((ambient + diffuse + specular), 1.0) * objectColor) / float(LightCount);
        }
    }
}