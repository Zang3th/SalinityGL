#version 450 core

in vec2 texCoords;
in vec3 normals;

out vec4 fragColor;

uniform sampler2D image;
uniform vec3 color;
uniform vec3 viewPos;

void main()
{
    fragColor = vec4(color, 1.0f) * texture(image, texCoords);
}