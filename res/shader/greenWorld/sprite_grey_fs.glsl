#version 450 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D image;
uniform vec3 color;

void main()
{
    float depthValue = texture(image, texCoords).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}