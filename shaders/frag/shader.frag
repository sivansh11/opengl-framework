#version 460 core

layout (location = 0) in vec3 inPos; 
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec4 inPosLight;


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

const int POINTLIGHT = 0;
const int DIRECTIONALLIGHT = 1;
const int SPOTLIGHT = 2;

struct Light
{
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int type;

    float linear;
    float quadratic;
    vec3 lightDir;
};
uniform Light light;

uniform vec3 cameraPos;

uniform sampler2D shadowMap;

vec3 pointLight();
vec3 directionalLight();
float shadowCalc();

void main()
{
    vec3 result;
    if (light.type == POINTLIGHT)
    {
        result = pointLight();
    }
    if (light.type == DIRECTIONALLIGHT)
    {
        result = directionalLight();
    }   
    result = pow(result, vec3(1.0/2.2));
    fragCol = vec4(result, 1.f);
}

vec3 pointLight()
{
    vec3 lightVec = light.lightPos - inPos;
    float dist = length(lightVec);
    float inten = 1.0f / (light.quadratic * dist * dist + light.linear * dist + 1.0f);

    float shadow = shadowCalc();

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
    vec3 lightDir = normalize(lightVec);
    float diff = max(dot(norm, lightDir), 0.f) * inten;
    vec3 diffuse;
    if (material.hasDiffuseMap)
        diffuse = light.diffuse * material.diffuse * diff * texture(material.diffuseMap, inUV).xyz;
    else
        diffuse = light.diffuse * material.diffuse * diff;

    // specular
    vec3 viewDir = normalize(cameraPos - inPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * inten;
    vec3 specular;
    if (material.hasSpecularMap)
        specular = light.specular * material.specular * spec * texture(material.specularMap, inUV).xyz;
    else
        specular = light.specular * material.specular * spec;

    vec3 result = ambient + ((diffuse + specular) * (1 - shadow));
    return result;
}

float shadowCalc()
{
    vec3 projCoords = inPosLight.xyz / inPosLight.w;
    projCoords = projCoords * 2 - 1;
    float closestDepth = texture(shadowMap, projCoords.xy).r;   
    float currentDepth = projCoords.z;  
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;  
    return shadow;
}

vec3 directionalLight()
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
    vec3 lightDir = normalize(light.lightDir);
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
    return result;
}
