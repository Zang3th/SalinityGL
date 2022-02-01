#version 450 core

in vec2 texCoords;
in vec2 texCoordsTiled;
in vec3 normals;
in vec4 fragPos;
in vec4 fragPosLightSpace;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D colorMap;
uniform sampler2D shadowMap;
uniform vec3 viewPos;

const float ambientStrength = 0.3;
const float diffuseStrength = 0.6;
const float specularStrength = 0.4;
const float shininess = 2.0;
const vec3  lightPos = vec3(150.0, 100.0, -30.0);
const vec3  lightColor = vec3(1.0, 1.0, 1.0);

float calculateShadow(vec4 fragPosInLightSpace, vec3 normal, vec3 lightDir)
{
    //Perform perspective division
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

vec3 calculateLight(vec3 textureColor, vec3 fragmentPosition)
{
    vec3 normal_n   = normalize(normals);
    vec3 lightDir   = normalize(lightPos - fragmentPosition);
    vec3 viewDir    = normalize(viewPos  - fragmentPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    //Ambient
    vec3 ambient = ambientStrength * lightColor * textureColor;

    //Diffuse
    float diff    = max(dot(lightDir, normal_n), 0.0);
    vec3  diffuse = diffuseStrength * diff * lightColor * textureColor;

    //Specular
    vec3  reflectDir = reflect(-lightDir, normal_n);
    float spec       = pow(max(dot(normal_n, halfwayDir), 0.0), shininess);
    vec3  specular   = specularStrength * spec * lightColor * textureColor;

    //Shadow-Value
    float shadow = calculateShadow(fragPosLightSpace, normal_n, lightDir);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main()
{
    vec4 texColor = texture(diffuseTexture, texCoordsTiled);
    vec4 texColorMap = texture(colorMap, texCoords);

    if(texColor.a < 0.5)
        discard;

    texColor = mix(texColor, texColorMap, 0.7);
    fragColor = vec4(calculateLight(texColor.rgb, fragPos.rgb), texColor.a);
}