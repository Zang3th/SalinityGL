#version 450 core

in vec2 texCoords;
in vec3 normals;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform int gotNormalMap;
uniform vec3 viewPos;

void main()
{
    fragColor = texture(diffuseTexture, texCoords);

    if(fragColor.a < 0.5)
        discard;
}