#version 450 core

in vec2 texCoords;
in vec3 normals;
in vec3 fragPos;
in vec4 fragPosLightSpace;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform int  gotNormalMap;
uniform vec3 viewPos;

const float ambientStrength = 0.3;
const float diffuseStrength = 0.6;
const float specularStrength = 0.4;
const float shininess = 2.0;
const vec3  lightPos = vec3(143.0, 175.0, -15.0);
const vec3  lightColor = vec3(1.0, 1.0, 1.0);

float calculateShadow(vec4 fragPosInLightSpace, vec3 normal, vec3 lightDir)
{
    //Perform perspective divide
    vec3 projCoords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;
    //Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 calculateLight(vec3 texColor)
{
    vec3 normal_n   = normalize(normals);
    vec3 lightDir   = normalize(lightPos - fragPos);
    vec3 viewDir    = normalize(viewPos  - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    //Ambient
    vec3 ambient = ambientStrength * lightColor * texColor;

    //Diffuse
    float diff    = max(dot(lightDir, normal_n), 0.0);
    vec3  diffuse = diffuseStrength * diff * lightColor * texColor;

    //Specular
    vec3  reflectDir = reflect(-lightDir, normal_n);
    float spec       = pow(max(dot(normal_n, halfwayDir), 0.0), shininess);
    vec3  specular   = specularStrength * spec * lightColor * texColor;

    //Shadow-Value
    float shadow = calculateShadow(fragPosLightSpace, normal_n, lightDir);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main()
{
    vec4 texColor = texture(diffuseTexture, texCoords);

    if(texColor.a < 0.5)
        discard;

    fragColor = vec4(calculateLight(texColor.rgb), texColor.a);
}