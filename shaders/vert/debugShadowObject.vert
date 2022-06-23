#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    outPos = vec3(model * vec4(inPos, 1));
    outNor = mat3(transpose(inverse(model))) * inNor;
	gl_Position = proj * view * vec4(outPos, 1);	
}