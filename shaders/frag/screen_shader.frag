#version 460 core
layout (location = 0) in vec2 uv;
out vec4 fragCol;
uniform sampler2D screen;

void main()
{
    fragCol = texture(screen, uv);
}

