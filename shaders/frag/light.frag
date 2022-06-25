#version 460 core

layout (location = 0) out vec4 fragCol;

uniform vec3 lightCol;

void main()
{
    fragCol = vec4(lightCol, 1.0);
}
