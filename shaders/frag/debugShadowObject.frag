#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;

layout (location = 0) out vec4 fragCol;

uniform vec3 lightCol;
uniform vec3 lightPos;

void main()
{
    vec3 ambient = lightCol * .1 ;

    // diffuse
    vec3 norm = normalize(inNor);
    vec3 lightDir = normalize(lightPos - inPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightCol * diff;    

    fragCol = vec4(ambient + diffuse, 1.0);
}
