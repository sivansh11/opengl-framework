#version 460 core

layout (location = 0) in vec3 inCol; 
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inPos;
layout (location = 3) in vec2 inUv;

layout (location = 0) out vec4 fragCol;

layout(std430, binding = 1) buffer lightBuffer
{
    vec3 lightPos;
    vec3 lightCol;
};

void main()
{
    vec3 normal = normalize(inNormal);
    vec3 lightDir = normalize(lightPos - inPos);

    float diffuse = max(dot(normal, lightDir), 0);


    fragCol = vec4(lightCol, 1) * diffuse;
}
