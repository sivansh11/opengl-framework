#version 460 core

layout (location = 0) in vec3 inPos; 
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec4 fragCol;

#ifdef light
layout (location = 3) in vec4 inPosLight;
uniform mat4 lightSpace;
#endif

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasNormalMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
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

void main()
{
    // ambient
    vec3 ambient;
    if (material.hasDiffuseMap)
        ambient = light.ambient * material.ambient * texture(material.diffuseMap, inUV).xyz;
    else
        ambient = light.ambient * material.ambient;
    
    // diffuse
    vec3 norm;
    if (material.hasNormalMap)
        norm = normalize(texture(material.normalMap, inUV).xyz * 2.f - 1.f);
    else
        norm = normalize(inNor);
    vec3 lightDir = normalize(light.lightPos - inPos);
    float diff = max(dot(norm, lightDir), 0.f);

    vec3 diffuse;
    if (material.hasDiffuseMap)
        diffuse = light.diffuse * material.diffuse * diff * texture(material.diffuseMap, inUV).xyz;
    else
        diffuse = light.diffuse * material.diffuse * diff;

    // specular
    vec3 viewDir = normalize(cameraPos - inPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular;
    if (material.hasSpecularMap)
        specular = light.specular * material.specular * spec * texture(material.specularMap, inUV).xyz;
    else
        specular = light.specular * material.specular * spec;

    vec3 result = ambient + diffuse + specular;
    result = pow(result, vec3(1.0/2.2));
    fragCol = vec4(result, 1.f);

}
