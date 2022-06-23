#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inPosLight;


layout (location = 0) out vec4 fragCol;

uniform vec3 lightCol;
uniform vec3 lightPos;

uniform sampler2D shadowMap;

void main()
{
    vec3 ambient = lightCol * .1 ;

    float shadow = 0;
    vec3 lightCoords = inPosLight.xyz / inPosLight.w;
    if (lightCoords.z <= 1)
    {
        lightCoords = (lightCoords + 1) / 2;
        float closestDepth = texture(shadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;
        if (currentDepth > closestDepth)
        {
            shadow = 1;
        }
    }

    // diffuse
    vec3 norm = normalize(inNor);
    vec3 lightDir = normalize(lightPos - inPos);
    float diff = max(dot(norm, lightDir), 0.0) * (1 - shadow);
    vec3 diffuse = lightCol * diff;    

    fragCol = vec4(ambient + diffuse, 1.0);
}
