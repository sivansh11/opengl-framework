#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inTex;

layout (location = 0) out vec4 fragCol;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 lightPos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
};
uniform Light light;

uniform vec3 cameraPos;

uniform sampler2D diffuse1;
uniform sampler2D normal1;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(texture(normal1, inTex).xyz * 2 - 1);
    vec3 lightDir = normalize(light.lightPos - inPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse) * texture(diffuse1, inTex).xyz;    
    // vec3 diffuse = light.diffuse * (diff * material.diffuse);    

    // specular
    vec3 viewDir = normalize(cameraPos - inPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    // vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    fragCol = vec4(result, 1.0);
}