#version 460 core

layout (location = 0) in vec3 inPos; 
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec4 inPosLight;

layout (location = 0) out vec4 fragCol;

uniform vec3 cameraPos;

uniform vec3 lightVec;

uniform sampler2D shadowMap;

void main()
{
    float ambientStrength = .1;
}