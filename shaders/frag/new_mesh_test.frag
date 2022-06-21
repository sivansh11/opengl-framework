#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inTex;

layout (location = 0) out vec4 fragCol;

layout (std430, binding = 0) buffer lightBuffer
{
    vec3 lightPos;
    vec3 lightCol;
};
layout (std430, binding = 1) buffer cameraBuffer
{
    vec3 cameraPos;
};

uniform sampler2D diffuse1;
uniform sampler2D specular1;

void main()
{
    // ambient
    float ambient = 0.1;

    // diffuse
    vec3 norm = normalize(inNor);
    vec3 lightDir = normalize(lightPos - inPos);
    float diffuse = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 viewDir = normalize(cameraPos - inPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specular = pow(max(dot(cameraPos, reflectDir), 0.0), 32) * 0.5;

    fragCol = (ambient + diffuse + specular) * vec4(1, 1, 1, 1) * texture(diffuse1, inTex) * texture(specular1, inTex);
}

