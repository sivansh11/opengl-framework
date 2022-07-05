#version 460 core

layout (location = 0) in vec3 inPos;

uniform mat4 lightSpace;
uniform mat4 model;

void main()
{
    gl_Position = lightSpace * model * vec4(inPos, 1.0f);
}
