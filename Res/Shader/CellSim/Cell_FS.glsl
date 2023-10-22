#version 450 core

in vec3 normals;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

const float ambientStrength = 0.3;
const float diffuseStrength = 0.6;
const float specularStrength = 0.4;
const float shininess = 2.0;
const vec3  waterColor = vec3(0.2, 0.4, 0.9);

vec3 calculateLight(vec3 textureColor)
{
    vec3 normal_n   = normalize(normals);
    vec3 lightDir   = normalize(lightPos - fragPos);
    vec3 viewDir    = normalize(viewPos  - fragPos);
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

    return (ambient + diffuse + specular);
}

void main()
{
    fragColor = vec4(calculateLight(waterColor), 1.0);
}