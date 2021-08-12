#version 450 core

in vec3 texCoords;

out vec4 fragColor;

uniform samplerCube textureSampler;

void main()
{
    fragColor = texture(textureSampler, texCoords) * vec4(0.88, 0.88, 1.0, 1.0) * 1.2;
}