#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    texCoords = inUV;
}
