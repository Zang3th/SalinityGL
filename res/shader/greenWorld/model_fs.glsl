#version 450 core

in vec2 texCoords;
in vec3 normals;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform int gotNormalMap;
uniform vec3 viewPos;

const float ambientStrength = 0.3;
const float diffuseStrength = 0.7;
const float specularStrength = 0.5;
const vec3 lightPos = vec3(140.0, 60.0, 60.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float shininess = 2.0;

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

    return (ambient + diffuse + specular);
}

void main()
{
    vec4 texColor = texture(diffuseTexture, texCoords);

    if(texColor.a < 0.5)
        discard;

    fragColor = vec4(calculateLight(texColor.rgb), texColor.a);
}