#version 460 core

layout (location = 0) in vec2 inTex;

layout (location = 0) out vec4 fragCol;

uniform vec3 lightCol;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

void main()
{
    fragCol = vec4(material.ambient + material.specular + material.diffuse, 1.0) + material.shininess * 0.000001 + vec4(lightCol, 1);
}
