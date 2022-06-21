#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec3 nor;
layout (location = 3) in vec2 uv;

layout (location = 0) out vec3 outCol; 
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outPos;
layout (location = 3) out vec2 outUv;

layout(std430, binding = 0) buffer cam_model
{
    mat4 proj;
    mat4 view;
    mat4 model;
};

void main()
{
    outPos = vec3(model * vec4(pos, 1.0f));
    gl_Position = proj * view * vec4(outPos, 1.0f);
    outCol = col;
    outUv = uv;
    outNormal = nor;
}
