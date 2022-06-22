#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inTex;

layout (location = 0) out vec4 fragCol;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 cameraPos;

uniform sampler2D diffuse1;
uniform sampler2D specular1;

void main()
{
    float ambient = 0.10f;

    vec3 normal = normalize(inNor);
    vec3 lightDirection = normalize(lightPos - inPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(cameraPos - inPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 8);
    float specular = specAmount * specularLight;

    vec4 zero = texture(specular1, inTex) * 0.000001;

    fragCol = texture(diffuse1, inTex) * vec4(lightCol * (diffuse + ambient + specular), 1) + zero;
}

