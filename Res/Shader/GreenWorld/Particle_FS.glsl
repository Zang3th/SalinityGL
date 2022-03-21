#version 450 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D textureAtlas;
uniform vec4      color;

void main()
{
    fragColor = texture(textureAtlas, texCoords);
}