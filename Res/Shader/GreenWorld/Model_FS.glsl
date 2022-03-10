#version 450 core

in vec2 texCoords;
in vec3 normals;
in vec3 fragPos;
in vec4 fragPosLightSpace;
in vec3 tangents;

out vec4 fragColor;

uniform sampler2D   diffuseTexture;
uniform sampler2D   normalMap;
uniform sampler2D   shadowMap;
uniform int         gotNormalMap;
uniform vec3        viewPos;
uniform vec3        lightPos;
uniform vec3        lightColor;

const float ambientStrength = 0.3;
const float diffuseStrength = 0.6;
const float specularStrength = 0.4;
const float shininess = 2.0;

float calculateShadow(vec4 fragPosInLightSpace, vec3 normal, vec3 lightDir)
{
    //Perform perspective division
    vec3 projCoords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;

    //Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float shadow        = 0.0;
    float currentDepth  = projCoords.z;
    float bias          = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    vec2 texelSize      = 1.0 / textureSize(shadowMap, 0) * 2.0;

    if(projCoords.z > 1.0)
    {
        return shadow;
    }
    else
    {
        for (int x = -5; x < 5; ++x)
        {
            for (int y = -5; y < 5; ++y)
            {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }

        return shadow / 100.0;
    }
}

vec3 calculateLight(vec3 textureColor, vec3 normal)
{
    vec3 lightDir   = normalize(lightPos - fragPos);
    vec3 viewDir    = normalize(viewPos  - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    //Ambient
    vec3 ambient = ambientStrength * lightColor * textureColor;

    //Diffuse
    float diff    = max(dot(lightDir, normal), 0.0);
    vec3  diffuse = diffuseStrength * diff * lightColor * textureColor;

    //Specular
    vec3  reflectDir = reflect(-lightDir, normal);
    float spec       = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3  specular   = specularStrength * spec * lightColor * textureColor;

    //Shadow-Value
    float shadow = calculateShadow(fragPosLightSpace, normal, lightDir);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 calculateBumpedNormal(vec3 normal)
{
    vec3 tangent_n = normalize(tangents);

    //Re-orthogonalize tangent with respect to the normal
    tangent_n = normalize(tangent_n - dot(tangent_n, normal) * normal);

    //Calculate bitangent
    vec3 bitangent = cross(tangent_n, normal);

    //Fetch normal map
    vec3 bumpMapNormal = texture(normalMap, texCoords).xyz;
    bumpMapNormal      = bumpMapNormal * 2.0 - 1.0;

    //Create TBN-Matrix and transform bumpMapNormal into world space
    vec3 newNormal;
    mat3 TBN  = mat3(tangent_n, bitangent, normal);
    newNormal = TBN * bumpMapNormal;

    return normalize(newNormal);
}

void main()
{
    vec4 texColor = texture(diffuseTexture, texCoords);

    if(texColor.a < 0.5)
        discard;

    vec3 normal_n = normalize(normals);

    if(gotNormalMap == 1)
    {
        normal_n = calculateBumpedNormal(normal_n);
    }

    fragColor = vec4(calculateLight(texColor.rgb, normal_n), texColor.a);
}