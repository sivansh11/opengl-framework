#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

layout (location = 0) out vec3 outCol;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1.0f);
    outCol = col; 
}