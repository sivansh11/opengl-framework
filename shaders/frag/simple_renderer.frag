#version 460 core

layout(location = 0) in vec3 col;

out vec4 fragCol;

void main()
{
    fragCol = vec4(col, 1);
}

